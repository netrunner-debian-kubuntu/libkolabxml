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

#ifndef SHAREDCONVERSIONS_H
#define SHAREDCONVERSIONS_H

#include <xsd/cxx/tree/date-time.hxx>
#include "kolabcontainers.h"
#include <boost/shared_ptr.hpp>
#include "utils.h"
#include <bindings/iCalendar-params.hxx>

namespace Kolab {
    namespace Shared {
typedef boost::shared_ptr<cDateTime> cDateTimePtr;

typedef ::xsd::cxx::tree::type type;
#if (XSD_INT_VERSION >= 4000000L)
typedef ::xsd::cxx::tree::simple_type< char, type > simple_type;
#else
typedef ::xsd::cxx::tree::simple_type< type > simple_type;
#endif
typedef ::xsd::cxx::tree::date< char, simple_type > date;
typedef ::xsd::cxx::tree::date_time< char, simple_type > date_time;

using namespace Utils;

cDateTimePtr toDate(const date &dt)
{
    cDateTimePtr date(new cDateTime());
    date->setDate(dt.year(), dt.month(), dt.day());
    return date;
}

cDateTimePtr toDate(const date_time &dt)
{
    cDateTimePtr date(new cDateTime());
    date->setDate(dt.year(), dt.month(), dt.day());
    date->setTime(dt.hours(), dt.minutes(), convertToInt(dt.seconds()));
    if (dt.zone_present()) {
        date->setUTC(true);
    }
    return date;
}

date_time fromDateTime(const cDateTime &d)
{
    date_time date(d.year(), fromInt<unsigned short>(d.month()), fromInt<unsigned short>(d.day()), fromInt<unsigned short>(d.hour()), fromInt<unsigned short>(d.minute()), fromInt<double>(d.second()));
    if (d.isUTC()) {
        //Setting both zone hours/and zone minutes to zero results in a Z being appended to indicate UTC
        date.zone_hours(0);
        date.zone_minutes(0);
    }
    return date;
}

date fromDate(const cDateTime &d)
{
    date de(fromInt<unsigned short>(d.year()), fromInt<unsigned short>(d.month()), fromInt<unsigned short>(d.day()));
    return de;
}

std::string toURN(const std::string &uid)
{
    if (uid.substr(0, 9) == std::string("urn:uuid:")) {
        return uid;
    }
    return std::string("urn:uuid:")+uid;
}

std::string fromURN(const std::string &uri)
{
    if (uri.substr(0, 9) != std::string("urn:uuid:")) {
        LOG("not a urn");
        return uri;
    }
    return uri.substr(9);
}

Kolab::ContactReference toContactReference(const std::string &uri) {
    std::string name;
    if (uri.substr(0, 9) == std::string("urn:uuid:")) {
        return Kolab::ContactReference(Kolab::ContactReference::UidReference, fromURN(uri));
    }
    const std::string &email = fromMailto(uri, name);
    return Kolab::ContactReference(Kolab::ContactReference::EmailReference, email, name);
}

    } //Namespace
} //Namespace

#endif
