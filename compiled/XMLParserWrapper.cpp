/*
 * Copyright (C) 2011  Christian Mollekopf <mollekopf@kolabsys.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "XMLParserWrapper.h"

#include <memory>   // std::auto_ptr
#include <fstream>
#include <iostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLUniDefs.hpp> // chLatin_*
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/validators/common/Grammar.hpp> // xercesc::Grammar
#include <xercesc/framework/Wrapper4InputSource.hpp>

#if _XERCES_VERSION >= 30000
#  include <xercesc/framework/XMLGrammarPoolImpl.hpp>
#else
#  include <xercesc/internal/XMLGrammarPoolImpl.hpp>
#endif

#include <xsd/cxx/xml/string.hxx>
#include <xsd/cxx/xml/dom/auto-ptr.hxx>
#include <xsd/cxx/xml/dom/bits/error-handler-proxy.hxx>
#include <xsd/cxx/xml/sax/std-input-source.hxx>

#include <xsd/cxx/tree/error-handler.hxx>

#include <boost/thread.hpp>

#include "kolabformat-xcal-schema.hxx"
#include "grammar-input-stream.hxx"

XMLParserWrapper::XMLParserWrapper()
:   ehp(eh),
    parser(0),
    gp(0)
{
    // We need to initialize the Xerces-C++ runtime because we
    // are doing the XML-to-DOM parsing ourselves.
    //
    xercesc::XMLPlatformUtils::Initialize ();
    init();
}


XMLParserWrapper::~XMLParserWrapper()
{
    delete parser;
    delete gp;
    xercesc::XMLPlatformUtils::Terminate ();

}

boost::thread_specific_ptr<XMLParserWrapper> ptr;

XMLParserWrapper& XMLParserWrapper::inst()
{
    XMLParserWrapper *t = ptr.get();
    if (!t) {
        t = new XMLParserWrapper();
        ptr.reset(t);
    }
    return *t;
}


void XMLParserWrapper::init()
{
    using namespace std;
    
    if (parser) {
        return;
    }
    try
    {
        using namespace xercesc;
        namespace xml = xsd::cxx::xml;
        namespace tree = xsd::cxx::tree;
        // Create and load the grammar pool.
        //
        MemoryManager* mm (XMLPlatformUtils::fgMemoryManager);

        gp = new XMLGrammarPoolImpl (mm);

        try
        {
            grammar_input_stream is (iCalendar_schema, sizeof (iCalendar_schema));
            gp->deserializeGrammars(&is);
        }
        catch(const XSerializationException& e)
        {
            cerr << "unable to load schema: " << xml::transcode<char> (e.getMessage ()) << endl;
            return;
        }

        // Lock the grammar pool. This is necessary if we plan to use the
        // same grammar pool in multiple threads (this way we can reuse the
        // same grammar in multiple parsers). Locking the pool disallows any
        // modifications to the pool, such as an attempt by one of the threads
        // to cache additional schemas.
        //
        gp->lockPool ();

        // Get an implementation of the Load-Store (LS) interface.
        //
        const XMLCh ls_id [] = {chLatin_L, chLatin_S, chNull};

        DOMImplementation* impl (
        DOMImplementationRegistry::getDOMImplementation (ls_id));

    #if _XERCES_VERSION >= 30000

        // Xerces-C++ 3.0.0 and later.
        //
        parser = impl->createLSParser (
            DOMImplementationLS::MODE_SYNCHRONOUS, 0, mm, gp);

        DOMConfiguration* conf (parser->getDomConfig ());

        // Discard comment nodes in the document.
        //
        conf->setParameter (XMLUni::fgDOMComments, false);

        // Enable datatype normalization.
        //
        conf->setParameter (XMLUni::fgDOMDatatypeNormalization, true);

        // Do not create EntityReference nodes in the DOM tree. No
        // EntityReference nodes will be created, only the nodes
        // corresponding to their fully expanded substitution text
        // will be created.
        //
        conf->setParameter (XMLUni::fgDOMEntities, false);

        // Perform namespace processing.
        //
        conf->setParameter (XMLUni::fgDOMNamespaces, true);

        // Do not include ignorable whitespace in the DOM tree.
        //
        conf->setParameter (XMLUni::fgDOMElementContentWhitespace, false);

        // Enable validation.
        //
        conf->setParameter (XMLUni::fgDOMValidate, true);
        conf->setParameter (XMLUni::fgXercesSchema, true);
        conf->setParameter (XMLUni::fgXercesSchemaFullChecking, false);

        // Xerces-C++ 3.1.0 is the first version with working multi import
        // support.
        //
    #if _XERCES_VERSION >= 30100
        conf->setParameter (XMLUni::fgXercesHandleMultipleImports, true);
    #endif

        // Use the loaded grammar during parsing.
        //
        conf->setParameter (XMLUni::fgXercesUseCachedGrammarInParse, true);

        // Disable loading schemas via other means (e.g., schemaLocation).
        //
        conf->setParameter (XMLUni::fgXercesLoadSchema, false);

        // We will release the DOM document ourselves.
        //
        conf->setParameter (XMLUni::fgXercesUserAdoptsDOMDocument, true);
        
        

        // Set error handler.
        //
//         tree::error_handler<char> eh;
//         xml::dom::bits::error_handler_proxy<char> ehp (eh);
        conf->setParameter (XMLUni::fgDOMErrorHandler, &ehp); //TODO does conf take a copy or should the ehp object live on?

    #else // _XERCES_VERSION >= 30000

        // Same as above but for Xerces-C++ 2 series.
        //
        parser = impl->createDOMBuilder(
            DOMImplementationLS::MODE_SYNCHRONOUS, 0, mm, gp);


        parser->setFeature (XMLUni::fgDOMComments, false);
        parser->setFeature (XMLUni::fgDOMDatatypeNormalization, true);
        parser->setFeature (XMLUni::fgDOMEntities, false);
        parser->setFeature (XMLUni::fgDOMNamespaces, true);
        parser->setFeature (XMLUni::fgDOMWhitespaceInElementContent, false);
        parser->setFeature (XMLUni::fgDOMValidation, true);
        parser->setFeature (XMLUni::fgXercesSchema, true);
        parser->setFeature (XMLUni::fgXercesSchemaFullChecking, false);
        parser->setFeature (XMLUni::fgXercesUseCachedGrammarInParse, true);
        parser->setFeature (XMLUni::fgXercesUserAdoptsDOMDocument, true);

        //tree::error_handler<char> eh;
//         xml::dom::bits::error_handler_proxy<char> ehp (eh);
        parser->setErrorHandler (&ehp);

    #endif // _XERCES_VERSION >= 30000

        // Parse XML documents.
        //
        
    }
    catch (const xml_schema::exception& e) {
        cout << "schema exception" << endl;
        cerr << e << endl;
    } catch (const std::ios_base::failure&) {
        cerr << ": unable to open or read failure" << endl;
    }
    
}

xsd::cxx::xml::dom::auto_ptr< xercesc::DOMDocument > XMLParserWrapper::parseFile(const std::string& url)
{
    try {
        std::ifstream ifs;
        ifs.exceptions (std::ifstream::badbit | std::ifstream::failbit); //TODO handle exceptions
        ifs.open (url.c_str());
        return parse(ifs, url);
    } catch (const std::ios_base::failure&)
    {
        std::cerr << ": unable to open or read failure" << std::endl;
    }
    return xsd::cxx::xml::dom::auto_ptr< xercesc::DOMDocument >();
}

xsd::cxx::xml::dom::auto_ptr< xercesc::DOMDocument > XMLParserWrapper::parseString(const std::string& s)
{
    std::istringstream is(s);
    return parse(is, ""); //TODO set identifier?
}


xml_schema::dom::auto_ptr<xercesc::DOMDocument> XMLParserWrapper::parse(std::istream &ifs, const std::string &name)
{
    using namespace std;

    try
    {
        using namespace xercesc;
        namespace xml = xsd::cxx::xml;
        namespace tree = xsd::cxx::tree;

        // Parse XML documents.
        //
        {
            // Wrap the standard input stream.
            //
            xml::sax::std_input_source isrc (ifs, name);
            Wrapper4InputSource wrap (&isrc, false);

            // Parse XML to DOM.
            //
        #if _XERCES_VERSION >= 30000
            xml_schema::dom::auto_ptr<DOMDocument> doc (parser->parse (&wrap));
        #else
            xml_schema::dom::auto_ptr<DOMDocument> doc (parser->parse (wrap));
        #endif

            eh.throw_if_failed<xml_schema::parsing> ();
            return doc;
        }
    }
    catch (const xml_schema::exception& e)
    {
        cerr << "schema exception" << endl;
        cerr << e << endl;
    }
    catch (const std::ios_base::failure&)
    {
        cerr << ": unable to open or read failure" << endl;
    }
    catch (...)
    {
        cerr << ": unknown exception thrown" << endl;
    }
    eh.reset();
    return xml_schema::dom::auto_ptr<xercesc::DOMDocument>();
}
