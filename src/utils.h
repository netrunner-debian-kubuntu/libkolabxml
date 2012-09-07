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

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "kolabcontainers.h"
#include "global_definitions.h"
#include <boost/numeric/conversion/cast.hpp>

namespace Kolab {

    namespace Utils {
/**
 * Returns a new globally unique UID if the parameter is empty
 */
std::string getUID(const std::string & = std::string());

void logMessage(const std::string &,const std::string &, int, ErrorSeverity s);
     
#define LOG(message) logMessage(message,__FILE__, __LINE__, NoError);
#define WARNING(message) logMessage(message,__FILE__, __LINE__, Warning);
#define ERROR(message) logMessage(message,__FILE__, __LINE__, Error);
#define CRITICAL(message) logMessage(message,__FILE__, __LINE__, Critical);

void logMessage(const std::string &, ErrorSeverity s = Warning);


/**
 * The following values must be updated by the serialization/deserialization functions
 */


/**
 * The error state after serialization/deserialization
 */
void clearErrors();
ErrorSeverity getError();
std::string getErrorMessage();

/**
 * The uid of the last serialized object
 */
void setCreatedUid(const std::string &);
std::string createdUid();

/**
 * The productId of the last deserialized object
 */
void setProductId(const std::string &);
std::string productId();

/**
 * The Kolab Format Version of the last deserialized object
 */
void setKolabVersion(const std::string &);
std::string kolabVersion();

/**
 * A timestamp which overrides the one normally used.
 */
void setOverrideTimestamp(const cDateTime &);
/**
 * From system time or overrideTimestamp if set.
 */
cDateTime timestamp();

/**
 * Helper functions for save conversion of integer types (so we can catch overflows)
 */

template <typename T>
int convertToInt(T integer)
{
    try {
        return boost::numeric_cast<int>(integer);
    } catch(boost::numeric::negative_overflow& e) {
        ERROR(e.what());
    } catch(boost::numeric::positive_overflow& e) {
        ERROR(e.what());
    } catch(boost::numeric::bad_numeric_cast& e) {
        ERROR(e.what());
    }
    return 0;
}

template <typename T> 
T fromInt(int integer)
{
    try {
        return boost::numeric_cast<T>(integer);
    } catch(boost::numeric::negative_overflow& e) {
        ERROR(e.what());
    } catch(boost::numeric::positive_overflow& e) {
        ERROR(e.what());
    } catch(boost::numeric::bad_numeric_cast& e) {
        ERROR(e.what());
    }
    return 0;
}

std::string uriInlineEncoding(const std::string &, const std::string &mime);
std::string uriInlineDecoding(const std::string &s, std::string &mimetype);

std::string toMailto(const std::string &email, const std::string &name = std::string());
std::string fromMailto(const std::string &mailtoUri, std::string &name);
std::string fromMailto(const std::string &mailtoUri);

/**
 * Appends the libkolabxml productid and returns the string
 */
std::string getProductId(const std::string &clientProdid);

    } //Namespace

} //Namespace

#endif

