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

#include "utils.h"
#include <iostream>
#include <string>

#include <boost/version.hpp>

#if BOOST_VERSION >= 104300
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#else
#include <uuid.h>
#endif

#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <time.h>
#include "base64.h"
#include "uriencode.h"
#include "libkolabxml-version.h"

namespace Kolab {
    
namespace Utils {

/**
 * We have to store our global variables thread-local to ensure thread-safety.
 */
struct Global {
    std::string createdUID;
    std::string productId;
    std::string xKolabVersion;
    
    ErrorSeverity errorBit;
    std::string errorMessage;
    cDateTime overrideTimestamp;
};

boost::thread_specific_ptr<Global> ptr;
class ThreadLocal
{
public:
    static Global &inst()
    {
        Global *t = ptr.get();
//         std::cout << "inst " << boost::this_thread::get_id() << std::endl;
        if (!t) {
//             std::cout << "initialize Global" << std::endl;
            t = new Global();
            ptr.reset(t);
        }
        return *t;
    }
};

void setKolabVersion(const std::string &s)
{
    ThreadLocal::inst().xKolabVersion = s;
}

std::string kolabVersion()
{
    return ThreadLocal::inst().xKolabVersion;
}

void setProductId(const std::string &s)
{
    ThreadLocal::inst().productId = s;
}

std::string productId()
{
    return ThreadLocal::inst().productId;
}


void setCreatedUid(const std::string &s)
{
    ThreadLocal::inst().createdUID = s;
}

std::string createdUid()
{
    return ThreadLocal::inst().createdUID;
}


std::string getUID(const std::string &s)
{
    if (!s.empty()) {
        return s;
    }
#if BOOST_VERSION >= 104300

    //Required because boost::uuids::uuid is implemented as POD type
    class uuid_class : public boost::uuids::uuid
    {
    public:
        uuid_class()
            : boost::uuids::uuid(boost::uuids::random_generator()())
        {}

        operator boost::uuids::uuid() const {
            return static_cast<boost::uuids::uuid const&>(*this);
        }
    };

    uuid_class u; // initialize uuid
    return  boost::uuids::to_string(u);
#else
    uuid_t *uuid;
    //to avoid the "dereferencing type-punned pointer will break strict-aliasing rules" warning
    char * __attribute__((__may_alias__)) str = 0;
    uuid_create(&uuid);
    uuid_make(uuid, UUID_MAKE_V1);
    uuid_export(uuid, UUID_FMT_STR, (void**)&str, 0);
    uuid_destroy(uuid);

    return std::string(str, 36); //We don't need the terminating \0
#endif
}

cDateTime getCurrentTime()
{
    time_t rawtime;
    struct tm * ptm;
    time(&rawtime);
    ptm = gmtime(&rawtime);
    return cDateTime(ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec, true);
}

void setOverrideTimestamp(const cDateTime &dt)
{
    ThreadLocal::inst().overrideTimestamp = dt;
}

cDateTime timestamp()
{
    const cDateTime &dt = ThreadLocal::inst().overrideTimestamp;
    if (dt.isValid()) {
        LOG("Timestamp overridden"); 
        return dt;
    }
    return getCurrentTime();
}



void logMessage(const std::string &m, ErrorSeverity s)
{
    switch (s) {
        case NoError:
            std::cout << "Debug: " << m << std::endl;
            break;
        case Warning:
            std::cerr << "Warning: " << m << std::endl;
            if (ThreadLocal::inst().errorBit < Warning) {
                ThreadLocal::inst().errorBit = Warning;
                ThreadLocal::inst().errorMessage = m;
            }
            break;
        case Error:
            std::cerr << "Error: " << m << std::endl;
            if (ThreadLocal::inst().errorBit < Error) {
                ThreadLocal::inst().errorBit = Error;
                ThreadLocal::inst().errorMessage = m;
            }
            break;
        case Critical:
        default:
            std::cerr << "Critical: " << m << std::endl;
            if (ThreadLocal::inst().errorBit < Critical) {
                ThreadLocal::inst().errorBit = Critical;
                ThreadLocal::inst().errorMessage = m;
            }
    }
}

void logMessage(const std::string &message, const std::string &file, int line, ErrorSeverity s)
{
   logMessage(file+" "+boost::lexical_cast<std::string>(line)+": " + "  " + message, s);
}

void clearErrors()
{
    ThreadLocal::inst().errorBit = NoError;
    ThreadLocal::inst().errorMessage.clear();
}

ErrorSeverity getError()
{
    return ThreadLocal::inst().errorBit;
}

std::string getErrorMessage()
{
    return ThreadLocal::inst().errorMessage;
}


std::string uriInlineEncoding(const std::string &s, const std::string &mimetype)
{
    return std::string("data:")+mimetype+std::string(";base64,")+base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), static_cast<unsigned int>(s.length()));
}

std::string uriInlineDecoding(const std::string &s, std::string &mimetype)
{
    if (s.substr(0, 5) != std::string("data:")) {
        ERROR("wrong picture encoding");
        std::cout << s <<  "      " << s.substr(0, 5) << std::endl;
        return std::string();
    }
    std::size_t pos = s.find(";",5);
    if (pos == std::string::npos) {
        ERROR("wrong picture encoding");
        std::cout << s <<  "      " << s.substr(0, 5) << std::endl;
        return std::string();
    }
    mimetype = s.substr(5, pos-5);
    if (s.substr(pos+1, 6) != std::string("base64")) {
        ERROR("wrong picture encoding");
        std::cout << s <<  "      " << s.substr(pos+1, 6) << std::endl;
        return std::string();
    }
    return base64_decode(s.substr(pos+8, s.size()-(pos+8)));
}

std::string toMailto(const std::string &email, const std::string &name)
{
    std::string mailto;
    if (!name.empty()) {
        mailto.append(name);
    }
    mailto.append("<");
    mailto.append(email);
    mailto.append(">");
    return std::string("mailto:")+uriEncode(mailto);
}

std::string fromMailto(const std::string &mailtoUri, std::string &name)
{
    const std::string &decoded = uriDecode(mailtoUri);
    if (decoded.substr(0, 7).compare("mailto:")) {
        WARNING("no mailto address");
        std::cout << decoded << std::endl;
        return mailtoUri;
    }
    std::size_t begin = decoded.find('<',7);
    if (begin == std::string::npos) {
        WARNING("no mailto address");
        std::cout << decoded << std::endl;
        return mailtoUri;
    }
    std::size_t end = decoded.find('>', begin);
    if (end == std::string::npos) {
        WARNING("no mailto address");
        std::cout << decoded << std::endl;
        return mailtoUri;
    }
    name = decoded.substr(7, begin-7);
    const std::string &email = decoded.substr(begin+1, end-begin-1);
    return email;
}

std::string fromMailto(const std::string &mailtoUri)
{
    std::string n;
    return fromMailto(mailtoUri, n);
}

std::string getProductId(const std::string& clientProdid)
{
    if (clientProdid.empty()) {
        return KOLAB_LIB_VERSION_STRING;
    }
    return clientProdid + " " + KOLAB_LIB_VERSION_STRING;
}


} //Namespace

} //Namespace
