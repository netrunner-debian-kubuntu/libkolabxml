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

#ifndef XMLPARSER_WRAPPER_H
#define XMLPARSER_WRAPPER_H

#include <string>
#include <memory>
#include <bindings/kolabformat-xcal.hxx>
#include <xsd/cxx/tree/error-handler.hxx>
#include <boost/scoped_ptr.hpp>
#include <xsd/cxx/xml/dom/bits/error-handler-proxy.hxx>

#if _XERCES_VERSION >= 30000
#  include <xercesc/dom/DOMLSParser.hpp>
#  include <xercesc/dom/DOMLSException.hpp>
#else
#  include <xercesc/dom/DOMBuilder.hpp>
#endif

#include <xercesc/framework/XMLGrammarPool.hpp>

/**
 * This wrapper controls the lifetime of the parser object.
 * 
 * Initializing the parser is much more expensive than parsing a single XML document, therefore the parser should be reused if possible.
 * 
 * It might make sense to use a singleton internally to keep the parser alive between usages. Alternatively this object can be kept alive for as long as it makes sense.
 * 
 * This class also encapsulates the initialization of the whole parser, which must be done manually because precomiled schemas are used (which greatly improves the initialization performance).
 * 
 * Writing the document is static and doesn't need any initialization and is therefore not wrapped by this object.
 * 
 */

class XMLParserWrapper {
public:
    XMLParserWrapper();
    ~XMLParserWrapper();
    
    /**
     * Threadsafe singleton. One Xerces instance is created per thread (threadlocal).
     * Access via singleton to reuse parser.
     */
    static XMLParserWrapper &inst();
    
    xml_schema::dom::auto_ptr<xercesc::DOMDocument> parseFile(const std::string &url);
    xml_schema::dom::auto_ptr<xercesc::DOMDocument> parseString(const std::string &s);
    xml_schema::dom::auto_ptr<xercesc::DOMDocument> parse(std::istream &ifs, const std::string &name);
private:
    void init();
    xsd::cxx::tree::error_handler<char> eh;
    xsd::cxx::xml::dom::bits::error_handler_proxy<char> ehp;

    #if _XERCES_VERSION >= 30000
        xercesc::DOMLSParser  *parser;
    #else
        xercesc::DOMBuilder  *parser;
    #endif
    xercesc::XMLGrammarPool *gp;
};


#endif











