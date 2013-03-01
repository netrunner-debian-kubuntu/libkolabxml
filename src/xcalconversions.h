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

#ifndef XCALCONVERSIONS_H
#define XCALCONVERSIONS_H

#include "global_definitions.h"
#include "libkolabxml-version.h"

#include <bindings/kolabformat-xcal.hxx>
#include <bindings/iCalendar-props.hxx>
#include <compiled/XMLParserWrapper.h>

#include <boost/shared_ptr.hpp>
#include <boost/numeric/conversion/converter_policies.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/foreach.hpp>

#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include "kolabcontainers.h"
#include "kolabtodo.h"
#include "kolabevent.h"
#include "kolabjournal.h"
#include <kolabfreebusy.h>
#include "utils.h"
#include "base64.h"
#include "shared_conversions.h"

namespace Kolab {
    namespace XCAL {
        
std::string global_xCalVersion;

const char* const XCAL_VERSION = "2.0";
const char* const XCAL_NAMESPACE = "urn:ietf:params:xml:ns:icalendar-2.0";

const char* const TZ_PREFIX = "/kolab.org/";

const char* const THISANDFUTURE = "THISANDFUTURE";

const char* const BASE64 = "BASE64";

const char* const NEEDSACTION = "NEEDS-ACTION";
const char* const COMPLETED = "OPAQUE";
const char* const INPROCESS = "IN-PROCESS";
const char* const CANCELLED = "CANCELLED";
const char* const TENTATIVE = "TENTATIVE";
const char* const CONFIRMED = "CONFIRMED";
const char* const DRAFT = "DRAFT";
const char* const FINAL = "FINAL";

const char* const CONFIDENTIAL = "CONFIDENTIAL";
const char* const PRIVATE = "PRIVATE";
const char* const PUBLIC = "PUBLIC";

const char* const PARTACCEPTED = "ACCEPTED";
const char* const PARTDECLINED = "DECLINED";
const char* const PARTDELEGATED = "DELEGATED";
const char* const PARTNEEDSACTION = "NEEDS-ACTION";
const char* const PARTTENTATIVE = "TENTATIVE";

const char* const CHAIR = "CHAIR";
const char* const NONPARTICIPANT = "NON-PARTICIPANT";
const char* const OPTIONAL = "OPT-PARTICIPANT";
const char* const REQUIRED = "REQ-PARTICIPANT";

const char* const DISPLAYALARM = "DISPLAY";
const char* const EMAILALARM = "EMAIL";
const char* const AUDIOALARM = "AUDIO";

const char* const TRANSPARENT = "TRANSPARENT";
const char* const OPAQUE = "OPAQUE";

const char* const MO = "MO";
const char* const TU = "TU";
const char* const WE = "WE";
const char* const TH = "TH";
const char* const FR = "FR";
const char* const SA = "SA";
const char* const SU = "SU";

const char* const GROUP = "GROUP";
const char* const INDIVIDUAL = "INDIVIDUAL";
const char* const RESOURCE = "RESOURCE";
const char* const UNKNOWN = "UNKNOWN";
const char* const ROOM = "ROOM";

//Alarms
const char* const START = "START";
const char* const END = "END";

//Freebusy
const char* const BUSY = "BUSY";
const char* const BUSY_TENTATIVE = "BUSY-TENTATIVE";
const char* const BUSY_OUTOFOFFICE = "X-OUT-OF-OFFICE";

using namespace Kolab::Utils;
using namespace Kolab::Shared;

//=== Generic Conversions ===
    

int toInt(const icalendar_2_0::IntegerPropertyType &prop)
{
    return convertToInt<icalendar_2_0::IntegerPropertyType::integer_type>(prop.integer());
}

std::vector<std::string> toStringList(const icalendar_2_0::TextListPropertyType &s)
{
    std::vector<std::string> d;
    std::copy(s.text().begin(), s.text().end(), std::back_inserter(d));
    return d;
}

template <typename T>
std::auto_ptr<T> fromStringList(const std::vector<std::string> &list)
{
    std::auto_ptr<T> ptr(new T());
    std::copy(list.begin(), list.end(), std::back_inserter(ptr->text()));
    return ptr;
}

//TODO doesn't seem very useful after all, remove
std::string toString(const icalendar_2_0::TextPropertyType &s)
{
    return s.text();
}




std::string fromDayPos(const Kolab::DayPos &d)
{   
    std::string s;
    if (d.occurence() != 0) {
        s.append(boost::lexical_cast<std::string>(d.occurence()));
    }
    switch (d.weekday()) {
        case Kolab::Monday:
            s.append(MO);
            break;
        case Kolab::Tuesday:
            s.append(TU);
            break;
        case Kolab::Wednesday:
            s.append(WE);
            break;
        case Kolab::Thursday:
            s.append(TH);
            break;
        case Kolab::Friday:
            s.append(FR);
            break;
        case Kolab::Saturday:
            s.append(SA);
            break;
        case Kolab::Sunday:
            s.append(SU);
            break;
    }
    return s;
}

Kolab::DayPos toDayPos(const std::string &s)
{
    std::string number;
    bool gotOccurrence = false;
    int occurrence = 0;
    for (std::string::const_iterator it = s.begin(); it != s.end(); it++) {
        switch(*it) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '+':
            case '-':
                number.push_back(*it);
                break;
            default:
                if (!gotOccurrence && !number.empty()) {
                    try {
                        occurrence = boost::lexical_cast<int>(number);
                    } catch(boost::bad_lexical_cast &) {
                        ERROR("failed to convert: " + number);
                        return DayPos();
                    }
                    number.clear();
                }
                gotOccurrence = true;
                number.push_back(*it);
                break;
        }
    }

    if (number == MO) {
        return DayPos(occurrence, Kolab::Monday);
    } else if (number == TU) {
        return DayPos(occurrence, Kolab::Tuesday);
    } else if (number == WE) {
        return DayPos(occurrence, Kolab::Wednesday);
    } else if (number == TH) {
        return DayPos(occurrence, Kolab::Thursday);
    } else if (number == FR) {
        return DayPos(occurrence, Kolab::Friday);
    } else if (number == SA) {
        return DayPos(occurrence, Kolab::Saturday);
    } else if (number == SU) {
        return DayPos(occurrence, Kolab::Sunday);
    }
    return DayPos();
}

std::string fromDuration(const Kolab::Duration &d)
{
    std::string s;
    if (!d.isValid()) {
        return s;
    }
    if (d.isNegative()) {
        s.push_back('-');
    }
    s.push_back('P');
    try {
        if (d.weeks() > 0) {
            s.append(boost::lexical_cast<std::string>(d.weeks()));
            s.push_back('W');
        }
        if (d.days() > 0) {
            s.append(boost::lexical_cast<std::string>(d.days()));
            s.push_back('D');
        }
        if (d.hours() > 0 || d.minutes() > 0 || d.seconds() > 0) {
            s.push_back('T');
            if (d.hours() > 0) {
                s.append(boost::lexical_cast<std::string>(d.hours()));
                s.push_back('H');
            }
            if (d.minutes() > 0) {
                s.append(boost::lexical_cast<std::string>(d.minutes()));
                s.push_back('M');
            }
            if (d.seconds() > 0) {
                s.append(boost::lexical_cast<std::string>(d.seconds()));
                s.push_back('S');
            }
        }
        
    } catch(boost::bad_lexical_cast &) { 
        ERROR("failed to convert duration");
        return std::string();
    }
    return s;
}

Kolab::Duration toDuration(const icalendar_2_0::DurationValueType &d)
{
    int weeks = 0;
    int days = 0;
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    bool negative = false;

    std::string number;

    for (std::string::const_iterator it = d.begin(); it != d.end(); it++) {
        switch(*it) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                number.push_back(*it);
                break;
            case 'H':
                try {
                    hours = boost::lexical_cast<int>(number);
                } catch(boost::bad_lexical_cast &) {
                    ERROR("failed to convert: " +  number);
                    return Duration();
                }
                number.clear();
                break;
            case 'M':
                try {
                    minutes = boost::lexical_cast<int>(number);
                } catch(boost::bad_lexical_cast &) {
                    ERROR("failed to convert: " + number);
                    return Duration();
                }
                number.clear();
                break;
            case 'S':
                try {
                    seconds = boost::lexical_cast<int>(number);
                } catch(boost::bad_lexical_cast &) { 
                    ERROR("failed to convert: " + number);
                    return Duration();
                }
                number.clear();
                break;
            case 'T':
                break;
            case 'W':
                try {
                    weeks = boost::lexical_cast<int>(number);
                } catch(boost::bad_lexical_cast &) { 
                    ERROR("failed to convert: " + number);
                    return Duration();
                }
                return Duration(weeks, negative);
            case 'D':
                try {
                    days = boost::lexical_cast<int>(number);
                } catch(boost::bad_lexical_cast &) { 
                    ERROR("failed to convert: " + number);
                    return Duration();
                }
                number.clear();
                break;
            case '+':
                break;
            case '-':
                negative = true;
                break;
            case 'P':
                break;
            default:
                ERROR("failed to convert duration: " + *it);
                return Duration();
        }
    }
    return Duration(days, hours, minutes, seconds, negative);
}

template <typename T>
T fromContactReference(const Kolab::ContactReference &c) {
    T organizer(toMailto(c.email()));
    
    typename T::parameters_type p;
    if (!c.name().empty()) {
        icalendar_2_0::CnParamType name(c.name());
        p.baseParameter().push_back(name);
    }
    if (!c.uid().empty()) {
        icalendar_2_0::DirParamType dir(toURN(c.uid()));
        p.baseParameter().push_back(dir);
    }
    organizer.parameters(p);
    return organizer;
}

Kolab::ContactReference toContactReference(const icalendar_2_0::CalAddressPropertyType &cal)
{
    const std::string &email = fromMailto(cal.cal_address());;
    std::string name;
    std::string uid;
    if (cal.parameters()) {
        for (icalendar_2_0::ArrayOfParameters::baseParameter_const_iterator it((*cal.parameters()).baseParameter().begin()); it != (*cal.parameters()).baseParameter().end(); it++) {
            if (const icalendar_2_0::CnParamType * tz = dynamic_cast<const icalendar_2_0::CnParamType*> (&*it)) {
                name = tz->text();
                continue;
            }
            if (const icalendar_2_0::DirParamType * tz = dynamic_cast<const icalendar_2_0::DirParamType*> (&*it)) {
                uid = fromURN(tz->uri());
                continue;
            }
        }
    }
    return Kolab::ContactReference(email, name, uid);
}


template <typename T>
Kolab::Attachment toAttachment(T aProp)
{
    Kolab::Attachment a;
    std::string mimetype;
    if (aProp.parameters()) {
        const icalendar_2_0::AttachPropType ::parameters_type &parameters = *aProp.parameters();
        for (icalendar_2_0::AttachPropType::parameters_type::baseParameter_const_iterator it(parameters.baseParameter().begin()); it != parameters.baseParameter().end(); it++) {
            if (const icalendar_2_0::FmttypeParamType *p = dynamic_cast<const icalendar_2_0::FmttypeParamType*> (&*it)) {
                mimetype = p->text();
            }
            if (const icalendar_2_0::EncodingParamType *p = dynamic_cast<const icalendar_2_0::EncodingParamType*> (&*it)) {
                if (p->text() != BASE64) {
                    ERROR("wrong encoding");
                    return Kolab::Attachment();
                }
            }
            if (const icalendar_2_0::XlabelParamType *p = dynamic_cast<const icalendar_2_0::XlabelParamType*> (&*it)) {
                a.setLabel(p->text());
            }
        }
    }
    if (mimetype.empty()) {
        ERROR("no mimetype");
    }

    if (aProp.uri()) {
        a.setUri(*aProp.uri(), mimetype);
    } else if (aProp.binary()) {
        a.setData(base64_decode(*aProp.binary()), mimetype);
    } else {
        ERROR("not uri and no data available");
    }
    return a;
}

icalendar_2_0::AttachPropType fromAttachment(const Kolab::Attachment &a)
{
    icalendar_2_0::AttachPropType attachment;
    icalendar_2_0::AttachPropType::parameters_type p;
    p.baseParameter().push_back(icalendar_2_0::FmttypeParamType(a.mimetype()));
    if (!a.label().empty()) {
        p.baseParameter().push_back(icalendar_2_0::XlabelParamType(a.label()));
    }
    
    if (!a.uri().empty()) {
        attachment.uri(a.uri());
    } else  if (!a.data().empty()) {
        attachment.binary(base64_encode(reinterpret_cast<const unsigned char*>(a.data().c_str()), static_cast<unsigned int>(a.data().length())));
        p.baseParameter().push_back(icalendar_2_0::EncodingParamType(BASE64));
    } else {
        ERROR("no uri and no data");
    }
    
    attachment.parameters(p);
    return attachment;
}

//==== cDateTime ====


std::string getTimezone(const icalendar_2_0::ArrayOfParameters &parameters) {
    for (icalendar_2_0::DateDatetimePropertyType::parameters_type::baseParameter_const_iterator it(parameters.baseParameter().begin()); it != parameters.baseParameter().end(); it++) {
        if (const icalendar_2_0::TzidParamType* tz = dynamic_cast<const icalendar_2_0::TzidParamType*> (&*it)) {
            std::string tzid = tz->text();
            if (tzid.find(TZ_PREFIX) != std::string::npos) {
                tzid.erase(0, strlen(TZ_PREFIX));
            } else {
                WARNING("/kolab.org/ timezone prefix is missing");
            }
            return tzid;
        }
    }
    return std::string();
}

cDateTimePtr toDate(const icalendar_2_0::DateDatetimePropertyType &dtProperty)
{
    cDateTimePtr date;
    if (dtProperty.date_time()) {
        date = Shared::toDate(*dtProperty.date_time());
    } else if (dtProperty.date()) {
        date = Shared::toDate(*dtProperty.date());
    }

    if (dtProperty.parameters()) {
        const std::string &tzid = getTimezone(*dtProperty.parameters());
        if (tzid.size()) {
            date->setTimezone(tzid);
        }
    }
    return date;
}


cDateTimePtr toDate(const icalendar_2_0::UtcDatetimePropertyType &dtProperty)
{
    cDateTimePtr date;
    if (dtProperty.date_time()) {
        date = Shared::toDate(*dtProperty.date_time());
    } else { //The utc-date-time element shouldn't even exist
        date = cDateTimePtr(new cDateTime());
        ERROR("This element shouldn't even be existing");
        //TODO Implement anyways?
        return date;
    }
    date->setUTC(true);
    return date;
}

template <typename I>
std::auto_ptr<I> fromDate(const cDateTime &dt)
{
    std::auto_ptr<I> ptr(new I);
    if (dt.isDateOnly()) {
        ptr->date(Shared::fromDate(dt));
    } else {
        ptr->date_time(Shared::fromDateTime(dt));

        const std::string &timezone = dt.timezone();
        if (timezone.size() != 0) {
            std::string tz(TZ_PREFIX);
            tz.append(timezone);
            icalendar_2_0::TzidParamType tzidParam(tz);
            icalendar_2_0::ArrayOfParameters parameters;
            parameters.baseParameter().push_back(tzidParam);
            ptr->parameters(parameters);
        }
    }
    return ptr;
}


template <typename I>
std::vector<cDateTime> toDateTimeList(I datelistProperty)
{
    std::vector<cDateTime>  list;
    
    std::string tzid;
    if (datelistProperty.parameters()) {
        tzid = getTimezone(*datelistProperty.parameters());
    }
    if (!datelistProperty.date().empty()) {
        BOOST_FOREACH(const xml_schema::date &d, datelistProperty.date()) {
            list.push_back(*Shared::toDate(d));
        }
    } else if (!datelistProperty.date_time().empty()) {
        BOOST_FOREACH(const xml_schema::date_time &d, datelistProperty.date_time()) {
            cDateTimePtr date = Shared::toDate(d);
            if (tzid.size()) {
                date->setTimezone(tzid);
            }
            list.push_back(*date);
        }
    }
    return list;
}

template <typename I>
std::auto_ptr<I> fromDateTimeList(const std::vector<cDateTime> &dtlist)
{
    
    std::auto_ptr<I> ptr(new I);
    BOOST_FOREACH(const cDateTime &dt, dtlist) {
        if (dt.isDateOnly()) {
            ptr->date().push_back(Shared::fromDate(dt));
        } else {
            ptr->date_time().push_back(Shared::fromDateTime(dt));
        }
        //TODO handle utc
        //TODO check for equality of timezones?
    }
    
    if (!dtlist.empty() && !dtlist.at(0).timezone().empty()) {
        const std::string &timezone = dtlist.at(0).timezone();
        if (timezone.size() != 0) {
            std::string tz(TZ_PREFIX);
            tz.append(timezone);
            icalendar_2_0::TzidParamType tzidParam(tz);
            icalendar_2_0::ArrayOfParameters parameters;
            parameters.baseParameter().push_back(tzidParam);
            ptr->parameters(parameters);
        }
    }
    
    return ptr;
}


//---- cDateTime ----



//=== Attendee ===

std::string mapPartStat(PartStatus status)
{
    switch (status) {
        case PartAccepted:
            return PARTACCEPTED;
        case PartDeclined:
            return PARTDECLINED;
        case PartDelegated:
            return PARTDELEGATED;
        case PartNeedsAction:
            return PARTNEEDSACTION;
        case PartTentative:
            return PARTTENTATIVE;
    }
    ERROR("PartStat not handled: " + status);
    return std::string();
}

PartStatus mapPartStat(const std::string &status)
{
    if (status == PARTACCEPTED) {
        return PartAccepted;
    } else if (status == PARTDECLINED) {
        return PartDeclined;
    } else if (status == PARTDELEGATED) {
        return PartDelegated;
    } else if (status == PARTNEEDSACTION) {
        return PartNeedsAction;
    } else if (status == PARTTENTATIVE) {
        return PartTentative;
    }
    ERROR("PartStat not handled: " + status);
    return PartNeedsAction;
}

std::string mapRole(Role status)
{
    switch (status) {
        case Chair:
            return std::string(CHAIR);
        case NonParticipant:
            return NONPARTICIPANT;
        case Optional:
            return OPTIONAL;
        case Required:
            return REQUIRED;
    }
    ERROR("PartStat not handled: " + status);
    return std::string();
}

Role mapRole(const std::string &status)
{
    if (status == CHAIR) {
        return Chair;
    } else if (status == NONPARTICIPANT) {
        return NonParticipant;
    } else if (status == OPTIONAL) {
        return Optional;
    } else if (status == REQUIRED) {
        return Required;
    }
    ERROR("Unhandled Role " + status);
    return Required;
}





//----------------

//=== Recurrence Rule ===

    typedef std::auto_ptr<RecurrenceRule> RecurrencePtr;


    RecurrenceRule::Frequency mapRecurrenceFrequency(const icalendar_2_0::RecurType::freq_type &freq)
    {
        using namespace icalendar_2_0;
        
        switch (freq) {
            case FreqRecurType::YEARLY:
                return RecurrenceRule::Yearly;
            case FreqRecurType::MONTHLY:
                return RecurrenceRule::Monthly;
            case FreqRecurType::WEEKLY:
                return RecurrenceRule::Weekly;
            case FreqRecurType::DAILY:
                return RecurrenceRule::Daily;
            case FreqRecurType::HOURLY:
                return RecurrenceRule::Hourly;
            case FreqRecurType::MINUTELY:
                return RecurrenceRule::Minutely;
            case FreqRecurType::SECONDLY:
                return RecurrenceRule::Secondly;
            default:
                ERROR("invalid unhandled recurrenc type" + freq);
        }
        return RecurrenceRule::FreqNone;
    }
    
    icalendar_2_0::RecurType::freq_type mapRecurrenceFrequency(RecurrenceRule::Frequency freq)
    {
        using namespace icalendar_2_0;

        switch (freq) {
            case RecurrenceRule::Yearly:
                return FreqRecurType::YEARLY;
            case RecurrenceRule::Monthly:
                return FreqRecurType::MONTHLY;
            case RecurrenceRule::Weekly:
                return FreqRecurType::WEEKLY;
            case RecurrenceRule::Daily:
                return FreqRecurType::DAILY;
            case RecurrenceRule::Hourly:
                return FreqRecurType::HOURLY;
            case RecurrenceRule::Minutely:
                return FreqRecurType::MINUTELY;
            case RecurrenceRule::Secondly:
                return FreqRecurType::SECONDLY;
            default:
                ERROR("invalid unhandled recurrenc type");
        }
        return 0;
    }
    
    static void setWeekStart(RecurrencePtr &r, const icalendar_2_0::RecurType::wkst_type &wkst)
    {
        using namespace icalendar_2_0;

        switch (wkst) {
            case WeekdayRecurType::MO:
                r->setWeekStart(Kolab::Monday);
                break;
            case WeekdayRecurType::TU:
                r->setWeekStart(Kolab::Tuesday);
                break;
            case WeekdayRecurType::WE:
                r->setWeekStart(Kolab::Wednesday);
                break;
            case WeekdayRecurType::TH:
                r->setWeekStart(Kolab::Thursday);
                break;
            case WeekdayRecurType::FR:
                r->setWeekStart(Kolab::Friday);
                break;
            case WeekdayRecurType::SA:
                r->setWeekStart(Kolab::Saturday);
                break;
            case WeekdayRecurType::SU:
                r->setWeekStart(Kolab::Sunday);
                break;
            default:
                ERROR("invalid unhandled weekday" + wkst);
        }
    }

    
    static void setByday(RecurrencePtr &r, const icalendar_2_0::RecurType::byday_sequence &list)
    {
        std::vector<DayPos> by;
        for (icalendar_2_0::RecurType::byday_const_iterator it(list.begin()); it != list.end(); it++) {
            by.push_back(toDayPos(*it));
        }
        r->setByday(by);
    }

    template <typename T, typename I>
    std::vector<int> bylist(const xsd::cxx::tree::sequence <T> &list)
    { 
        std::vector<int> by;
        BOOST_FOREACH(const T i, list) {
            by.push_back(convertToInt<I>(i));
        }
        return by;
    }
    
RecurrencePtr toRRule(const icalendar_2_0::RecurType &rrule)
{
    using namespace icalendar_2_0;

    RecurrencePtr r(new RecurrenceRule());
    r->setFrequency(mapRecurrenceFrequency(rrule.freq()));
    if (rrule.until()) {
        cDateTimePtr date;
        if ((*rrule.until()).date_time()) {
            date = Shared::toDate(*(*rrule.until()).date_time());
        } else if ((*rrule.until()).date()) {
            date = Shared::toDate(*(*rrule.until()).date());
        }
        r->setEnd(*date);
    } else if (rrule.count()) {
        r->setCount(toInt(*rrule.count()));
    }
    if (rrule.interval()) {
        r->setInterval(toInt(*rrule.interval()));
    } else {
        r->setInterval(1);
    }
    r->setBysecond(bylist<RecurType::bysecond_type, xml_schema::non_negative_integer>(rrule.bysecond()));
    r->setByminute(bylist<RecurType::byminute_type, xml_schema::non_negative_integer>(rrule.byminute()));
    r->setByhour(bylist<RecurType::byhour_type, xml_schema::non_negative_integer>(rrule.byhour()));
    setByday(r, rrule.byday());
    r->setBymonthday(bylist<RecurType::bymonthday_type, xml_schema::integer>(rrule.bymonthday()));
    r->setByyearday(bylist<RecurType::byyearday_type, xml_schema::integer>(rrule.byyearday()));
    r->setByweekno(bylist<RecurType::byweekno_type, xml_schema::integer>(rrule.byweekno()));
    r->setBymonth(bylist<RecurType::bymonth_type, xml_schema::integer>(rrule.bymonth()));
    if (rrule.wkst()) {
        setWeekStart(r, *rrule.wkst());
    }
    return r;
}
    
//--- Recurrence Rule ---

template <typename I, typename T>
void setIncidenceProperties(I &inc, const T &prop)
{    
    inc.setUid(toString(prop.uid()));
    inc.setCreated(*toDate(prop.created()));
    inc.setLastModified(*toDate(prop.dtstamp()));

    if (prop.sequence()) {
        inc.setSequence(toInt(*prop.sequence()));
    }
    
    if (prop.class_()) {
        std::string string(toString(*prop.class_()));
        Kolab::Classification sec = ClassPublic;
        if (string == PRIVATE) {
            sec = ClassPrivate;
        } else if (string == CONFIDENTIAL) {
            sec = ClassConfidential;
        }
        inc.setClassification(sec);
    }
    
    if (prop.categories()) {
        inc.setCategories(toStringList(*prop.categories()));
    }
    
    if (prop.dtstart()) {
        const cDateTimePtr date = toDate(*prop.dtstart());
        inc.setStart(*date);
    }

    if (prop.summary()) {
        inc.setSummary(toString(*prop.summary()));
    }

    if (prop.description()) {
        inc.setDescription(toString(*prop.description()));
    }

    if (prop.status()) {
        const std::string &status =  toString(*prop.status());
        if (status == NEEDSACTION) {
            inc.setStatus(StatusNeedsAction);
        } else if (status == COMPLETED) {
            inc.setStatus(StatusCompleted);
        } else if (status == INPROCESS) {
            inc.setStatus(StatusInProcess);
        } else if (status == CANCELLED) {
            inc.setStatus(StatusCancelled);
        } else if (status == TENTATIVE) {
            inc.setStatus(StatusTentative);
        } else if (status == CONFIRMED) {
            inc.setStatus(StatusConfirmed);
        } else if (status == DRAFT) {
            inc.setStatus(StatusDraft);
        } else if (status == FINAL) {
            inc.setStatus(StatusFinal);
        } else {
            ERROR("Unhandled status");
        }
    }
    
    if (prop.attendee().size()) {
        std::vector<Kolab::Attendee> attendees;
        BOOST_FOREACH(typename T::attendee_type aProp, prop.attendee()) {
            Kolab::Attendee a;
            std::string name;
            if (aProp.parameters()) {
                const icalendar_2_0::AttendeePropType::parameters_type &parameters = *aProp.parameters();
                for (icalendar_2_0::AttendeePropType::parameters_type::baseParameter_const_iterator it(parameters.baseParameter().begin()); it != parameters.baseParameter().end(); it++) {
                    if (const icalendar_2_0::CnParamType * p = dynamic_cast<const icalendar_2_0::CnParamType*> (&*it)) {
                        name = p->text();
                    }
                    if (const icalendar_2_0::PartstatParamType * p = dynamic_cast<const icalendar_2_0::PartstatParamType*> (&*it)) {
                        PartStatus s = mapPartStat(p->text());
                        if (s != PartNeedsAction) {
                            a.setPartStat(s);
                        }
                    }
                    if (const icalendar_2_0::RoleParamType * p = dynamic_cast<const icalendar_2_0::RoleParamType*> (&*it)) {
                        Role s = mapRole(p->text());
                        if (s != Required) {
                            a.setRole(s);
                        }
                    }
                    if (const icalendar_2_0::RsvpParamType * p = dynamic_cast<const icalendar_2_0::RsvpParamType*> (&*it)) {
                        a.setRSVP(p->boolean());
                    }
                    if (const icalendar_2_0::DelegatedToParamType * p = dynamic_cast<const icalendar_2_0::DelegatedToParamType*> (&*it)) {
                        std::vector<ContactReference> list;
                        BOOST_FOREACH(const icalendar_2_0::CalAddressListParamType::cal_address_type &adr, p->cal_address()) {
                            list.push_back(Shared::toContactReference(adr));
                        }
                        a.setDelegatedTo(list);
                    }
                    if (const icalendar_2_0::DelegatedFromParamType * p = dynamic_cast<const icalendar_2_0::DelegatedFromParamType*> (&*it)) {
                        std::vector<ContactReference> list;
                        BOOST_FOREACH(const icalendar_2_0::CalAddressListParamType::cal_address_type &adr, p->cal_address()) {
                            list.push_back(Shared::toContactReference(adr));
                        }
                        a.setDelegatedFrom(list);
                    }
                    if (const icalendar_2_0::CutypeParamType * p = dynamic_cast<const icalendar_2_0::CutypeParamType*> (&*it)) {
                        if (p->text() == RESOURCE) {
                            a.setCutype(CutypeResource);
                        } else if (p->text() == INDIVIDUAL) {
                            a.setCutype(CutypeIndividual);
                        } else if (p->text() == GROUP) {
                            a.setCutype(CutypeGroup);
                        } else if (p->text() == ROOM) {
                            a.setCutype(CutypeGroup);
                        } else if (p->text() == UNKNOWN) {
                            a.setCutype(CutypeGroup);
                        } else {
                            WARNING("Invalid attendee cutype");
                        }
                    }

                }
            }
            Kolab::ContactReference ref = toContactReference(aProp);
            a.setContact(ref);
            attendees.push_back(a);
        }
        inc.setAttendees(attendees);
    }
    
    if (prop.attach().size()) {
        std::vector<Kolab::Attachment> attachments;
        BOOST_FOREACH(typename T::attach_type aProp, prop.attach()) {
            const Kolab::Attachment &a = toAttachment<typename T::attach_type>(aProp);
            if (!a.isValid()) {
                ERROR("invalid attachment");
                continue;
            }
            attachments.push_back(a);
        }
        inc.setAttachments(attachments);
    }
    
    if (prop.x_custom().size()) {
        std::vector<Kolab::CustomProperty> customProperties;
        BOOST_FOREACH(typename T::x_custom_type p, prop.x_custom()) {
            customProperties.push_back(CustomProperty(p.identifier(), p.value()));
        }
        inc.setCustomProperties(customProperties);
    }

}

template <typename I, typename T>
void setTodoEventProperties(I &inc, const T &prop)
{

    if (prop.rrule()) {
       RecurrencePtr rrule = toRRule(prop.rrule()->recur());
       inc.setRecurrenceRule(*rrule);
    }

    if (prop.rdate()) {
        inc.setRecurrenceDates(toDateTimeList<icalendar_2_0::KolabEvent::properties_type::rdate_type>(*prop.rdate()));
        if (!prop.rdate()->period().empty()) {
            ERROR("the period element must not be used, ignored.");
        }
    }

    if (prop.exdate()) {
       inc.setExceptionDates(toDateTimeList<icalendar_2_0::KolabEvent::properties_type::exdate_type>(*prop.exdate()));
    }
    
    if (prop.recurrence_id()) {
        bool thisandfuture = false;
        if (prop.recurrence_id()->parameters()) {
            const icalendar_2_0::RecurrenceIdPropType::parameters_type &parameters = *prop.recurrence_id()->parameters();
            for (icalendar_2_0::RecurrenceIdPropType::parameters_type::baseParameter_const_iterator it(parameters.baseParameter().begin()); it != parameters.baseParameter().end(); it++) {
                if (dynamic_cast<const icalendar_2_0::RangeParamType*> (&*it)) {
                    thisandfuture = true;
                }
            }
        }
        inc.setRecurrenceID(*toDate(*prop.recurrence_id()), thisandfuture);
    }

    if (prop.priority()) {
        inc.setPriority(toInt(*prop.priority()));
    }

    if (prop.location()) {
        inc.setLocation(toString(*prop.location()));
    }
    
    if (prop.organizer()) {
        inc.setOrganizer(toContactReference(*prop.organizer()));
    }
    
}


template <typename T, typename I>
T fromList(const std::vector<int> &input) {
    T list;
    BOOST_FOREACH(int i, input) {
        list.push_back(convertToInt<I>(i));
    }
    return list;
}

std::auto_ptr< icalendar_2_0::RrulePropType > recurrenceProperty(const RecurrenceRule &r)
{
    using namespace icalendar_2_0;
        
    std::auto_ptr< RrulePropType > rruleProp(new RrulePropType(mapRecurrenceFrequency(r.frequency())));
    
    RecurPropertyType::recur_type &recur = rruleProp->recur();
    const cDateTime &endDate = r.end();
    if (endDate.isValid()) {
        RecurPropertyType::recur_type::until_type until;
        if (endDate.isDateOnly()) {
            until.date(Shared::fromDate(endDate));
        } else {
            until.date_time(Shared::fromDateTime(endDate));
        }
        recur.until(until);
    } else if (r.count() > 0) {
        recur.count(fromInt<RecurType::count_type>(r.count()));
    }
    
    if (r.interval() > 1) {
        recur.interval(fromInt<RecurType::interval_type>(r.interval()));
    }
    
    if (!r.bysecond().empty()) {
        recur.bysecond(fromList<RecurType::bysecond_sequence, xml_schema::non_negative_integer>(r.bysecond()));
    }
    
    if (!r.byminute().empty()) {
        recur.byminute(fromList<RecurType::byminute_sequence, xml_schema::non_negative_integer>(r.byminute()));
    }
    
    if (!r.byhour().empty()) {
        recur.byhour(fromList<RecurType::byhour_sequence, xml_schema::non_negative_integer>(r.byhour()));
    }
    
    if (!r.byday().empty()) {
        RecurType::byday_sequence byday;
        const std::vector<Kolab::DayPos> &l = r.byday(); 
        BOOST_FOREACH(Kolab::DayPos daypos, l) {
            byday.push_back(fromDayPos(daypos));
        }
        recur.byday(byday);
    }
    
    if (!r.bymonthday().empty()) {
        recur.bymonthday(fromList<RecurType::bymonthday_sequence, xml_schema::integer>(r.bymonthday()));
    }
    
    if (!r.byyearday().empty()) {
        recur.byyearday(fromList<RecurType::byyearday_sequence, xml_schema::integer>(r.byyearday()));
    }
    
    if (!r.byweekno().empty()) {
        recur.byweekno(fromList<RecurType::byweekno_sequence, xml_schema::integer>(r.byweekno()));
    }
    
    if (!r.bymonth().empty()) {
        recur.bymonth(fromList<RecurType::bymonth_sequence, xml_schema::integer>(r.bymonth()));
    }

    return rruleProp;
}




template <typename T, typename I>
void getIncidenceProperties(T &prop, const I &inc)
{
    using namespace icalendar_2_0;
    
    typedef T properties; 
    
    prop.sequence(fromInt<xml_schema::integer>(inc.sequence()));
    
    switch (inc.classification()) {
        case Kolab::ClassConfidential:
            prop.class_(typename properties::class_type(CONFIDENTIAL));
            break;
        case Kolab::ClassPrivate:
            prop.class_(typename properties::class_type(PRIVATE));
            break;
        default:
            prop.class_(typename properties::class_type(PUBLIC));
            break;
    }
    
    if (!inc.categories().empty()) {
        prop.categories(*fromStringList<typename properties::categories_type>(inc.categories()));
    }

    if (inc.start().isValid()) {
        prop.dtstart(fromDate<typename properties::dtstart_type>(inc.start()));
    }

    if (!inc.summary().empty()) {
        prop.summary(typename properties::summary_type(inc.summary()));
    }
    
    if (!inc.description().empty()) {
        prop.description(typename properties::description_type(inc.description()));
    }

    if (inc.status() != StatusUndefined) {
        switch (inc.status()) {
            case StatusNeedsAction:
                prop.status(typename properties::status_type(NEEDSACTION));
                break;
            case StatusCompleted:
                prop.status(typename properties::status_type(COMPLETED));
                break;
            case StatusInProcess:
                prop.status(typename properties::status_type(INPROCESS));
                break;
            case StatusCancelled:
                prop.status(typename properties::status_type(CANCELLED));
                break;
            case StatusTentative:
                prop.status(typename properties::status_type(TENTATIVE));
                break;
            case StatusConfirmed:
                prop.status(typename properties::status_type(CONFIRMED));
                break;
            case StatusDraft:
                prop.status(typename properties::status_type(DRAFT));
                break;
            case StatusFinal:
                prop.status(typename properties::status_type(FINAL));
                break;
            default:
                ERROR("unhandled status " + inc.status());
        }

    }

    if (!inc.attendees().empty()) {        
        const std::vector<Kolab::Attendee> &l = inc.attendees();
        BOOST_FOREACH(const Kolab::Attendee &a, l) {
            const Kolab::ContactReference &c = a.contact();
            
            typename properties::attendee_type attendee = fromContactReference<typename properties::attendee_type>(c);
            
            typename properties::attendee_type::parameters_type &p = *attendee.parameters();
            
            std::string stat = mapPartStat(a.partStat());
            if (!stat.empty()) {
                p.baseParameter().push_back(icalendar_2_0::PartstatParamType(stat));
            }
            
            std::string r = mapRole(a.role());
            if (!r.empty()) {
                p.baseParameter().push_back(icalendar_2_0::RoleParamType(r));
            }
            
            if (a.rsvp()) {
                p.baseParameter().push_back(icalendar_2_0::RsvpParamType(true));
            }

            if (!a.delegatedTo().empty()) {
                icalendar_2_0::DelegatedToParamType delegatedTo;
                BOOST_FOREACH(const Kolab::ContactReference &ref, a.delegatedTo()) {
                    delegatedTo.cal_address().push_back(CalAddressListParamType::cal_address_type(toMailto(ref.email(), ref.name())));
                }
                p.baseParameter().push_back(delegatedTo);
            }

            if (!a.delegatedFrom().empty()) {
                icalendar_2_0::DelegatedFromParamType delegatedFrom;
                BOOST_FOREACH(const Kolab::ContactReference &ref, a.delegatedFrom()) {
                    delegatedFrom.cal_address().push_back(CalAddressListParamType::cal_address_type(toMailto(ref.email(), ref.name())));
                }
                p.baseParameter().push_back(delegatedFrom);
            }

            if (a.cutype() != CutypeIndividual) {
                std::string type;
                switch (a.cutype()) {
                    case CutypeGroup:
                        type = GROUP;
                        break;
                    case CutypeResource:
                        type = RESOURCE;
                        break;
                    case CutypeRoom:
                        type = ROOM;
                        break;
                    case CutypeUnknown:
                        type = UNKNOWN;
                        break;
                    default:
                        WARNING("unknown cutype");
                        type = INDIVIDUAL;
                        break;
                }
                p.baseParameter().push_back(icalendar_2_0::CutypeParamType(type));
            }

            prop.attendee().push_back(attendee);

        }
    }
    
    if (!inc.attachments().empty()) {
        const std::vector<Kolab::Attachment> &l = inc.attachments();
        BOOST_FOREACH(const Kolab::Attachment &a, l) {
            prop.attach().push_back(fromAttachment(a));
        }
    }
    
    if (!inc.customProperties().empty()) {
        const std::vector<Kolab::CustomProperty> &l = inc.customProperties();
        BOOST_FOREACH(const Kolab::CustomProperty &a, l) {
            prop.x_custom().push_back(typename properties::x_custom_type(a.identifier, a.value));
        }
    }

}

template <typename T, typename I>
void getTodoEventProperties(T &prop, const I &inc)
{
    using namespace icalendar_2_0;
    typedef T properties; 
    
    if (inc.recurrenceRule().isValid()) {
        const RecurrenceRule &r = inc.recurrenceRule();
        prop.rrule(recurrenceProperty(r));
        //TODO check if startdate is allDay if recurrence is allDay
        //TODO check if startdate matches the one of the event (it MUST)
    }
    
    if (!inc.recurrenceDates().empty()) {
        prop.rdate(fromDateTimeList<typename properties::rdate_type>(inc.recurrenceDates()));
    }
    
    if (!inc.exceptionDates().empty()) {
        prop.exdate(fromDateTimeList<typename properties::exdate_type>(inc.exceptionDates()));
    }
    
    if (inc.recurrenceID().isValid()) {
        std::auto_ptr<typename properties::recurrence_id_type> recurrenceId = fromDate<typename properties::recurrence_id_type>(inc.recurrenceID());
        if (inc.thisAndFuture()) {
            if (!recurrenceId->parameters()) {
                recurrenceId->parameters(typename properties::recurrence_id_type::parameters_type());
            }
            typename properties::recurrence_id_type::parameters_type &parameters = *recurrenceId->parameters(); //There is maybe already a timezone set
            icalendar_2_0::RangeParamType range(THISANDFUTURE);
            parameters.baseParameter().push_back(range);
        }
        
        prop.recurrence_id(recurrenceId);
    }
    
    if (inc.priority() != 0) {
        prop.priority(typename properties::priority_type(fromInt<typename properties::priority_type::integer_type>(inc.priority())));
    }
    
    if (!inc.location().empty()) {
        prop.location(typename properties::location_type(inc.location()));
    }
    
    if (inc.organizer().isValid()) {
        prop.organizer(fromContactReference<typename properties::organizer_type>(inc.organizer()));
    }
}

//=== Alarms ===

template <typename KolabType, typename IncidenceType>
void setAlarms(typename KolabType::components_type& components, const IncidenceType &incidence)
{
    const std::vector<Kolab::Alarm> &alarms = incidence.alarms();
    BOOST_FOREACH(const Kolab::Alarm &alarm, alarms) {
        typedef icalendar_2_0::ValarmType::properties_type PropType;
        
        PropType::trigger_type trigger;
        if (alarm.start().isValid()) {
            if (!alarm.start().isUTC()) {
                ERROR("alarm start date is not UTC but MUST be UTC");
                continue;
            }
            trigger.date_time(fromDateTime(alarm.start()));
        } else {
            if (!alarm.relativeStart().isValid()) {
                ERROR("no start and no relativeStart");
                continue;
            }
            trigger.duration(PropType::trigger_type::duration_type(fromDuration(alarm.relativeStart())));
            icalendar_2_0::ArrayOfParameters parameters;
            if (alarm.relativeTo() == Kolab::End) {
                parameters.baseParameter().push_back(icalendar_2_0::RelatedParamType(END));
            } else {
                parameters.baseParameter().push_back(icalendar_2_0::RelatedParamType(START));
            }
            trigger.parameters(parameters);
        }

        std::auto_ptr<PropType> p;
        switch(alarm.type()) {
            case Kolab::Alarm::DisplayAlarm:
                p = std::auto_ptr<PropType>(new PropType(PropType::action_type(DISPLAYALARM), trigger));
                p->description(PropType::description_type(alarm.description()));
                break;
            case Kolab::Alarm::EMailAlarm: {
                p = std::auto_ptr<PropType>(new PropType(PropType::action_type(EMAILALARM), trigger));
                p->summary(PropType::summary_type(alarm.summary()));
                p->description(PropType::description_type(alarm.description()));
                const std::vector<Kolab::ContactReference> &l = alarm.attendees();
                BOOST_FOREACH(const Kolab::ContactReference &attendee, l) {
                    p->attendee().push_back(icalendar_2_0::ContactType(toMailto(attendee.email(), attendee.name())));
                }
                break;
            }
            case Kolab::Alarm::AudioAlarm:
                p = std::auto_ptr<PropType>(new PropType(PropType::action_type(AUDIOALARM), trigger));
                p->description(PropType::description_type(alarm.description()));
                p->attach(fromAttachment(alarm.audioFile()));
                break;
            default:
                ERROR("invalid alarm");
                continue;
        }
        if (alarm.duration().isValid()) {
            p->duration(PropType::duration_type(fromDuration(alarm.duration())));
            p->repeat(PropType::repeat_type(fromInt<PropType::repeat_type::integer_type>(alarm.numrepeat())));
        }

        components.valarm().push_back(icalendar_2_0::ValarmType(p));
    }
}



template <typename IncidenceType, typename KolabType>
void getAlarms(IncidenceType &incidence, const typename KolabType::components_type &components)
{
    typedef icalendar_2_0::ValarmType::properties_type PropType;
    std::vector<Kolab::Alarm> alarms;
    BOOST_FOREACH(const typename KolabType::components_type::valarm_type &valarm, components.valarm()) {
        const icalendar_2_0::ValarmType::properties_type &prop = valarm.properties();

        Kolab::Alarm alarm;
        if (prop.action().text() == DISPLAYALARM) {
            if (!prop.description()) {
                ERROR("description is missing");
                continue;
            }
            alarm = Kolab::Alarm((*prop.description()).text());
        } else if (prop.action().text() == EMAILALARM) {
            std::vector<Kolab::ContactReference> attendees;
            if (prop.attendee().empty()) {
                WARNING("No receipents for email alarm");
            }
            for (typename PropType::attendee_const_iterator at(prop.attendee().begin()); at != prop.attendee().end(); at++) {
                std::string name;
                const std::string &email = fromMailto((*at).cal_address(), name);
                attendees.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, email, name));
            }
            if (!prop.description() || !prop.summary()) {
                ERROR("description or summary is missing");
                continue;
            }
            alarm = Kolab::Alarm((*prop.summary()).text(), (*prop.description()).text(), attendees);
        } else if (prop.action().text() == AUDIOALARM) {
            if (!prop.attach()) {
                ERROR("audio file is missing");
                continue;
            }
            const Kolab::Attachment &attach = toAttachment<icalendar_2_0::properties::attach_type>(*prop.attach());
            if (!attach.isValid()) {
                ERROR("audio file is invalid");
                continue;
            }
            alarm = Kolab::Alarm(attach);
        } else {
            ERROR("unknown alarm type " + prop.action().text());
            continue;
        }
        
        if (prop.trigger().date_time()) {
            alarm.setStart(*Shared::toDate(*prop.trigger().date_time()));
            if (!alarm.start().isUTC()) {
                ERROR("The start date time must be in UTC ");
                continue;
            }
        } else if (prop.trigger().duration()) {
            Kolab::Relative relativeTo = Kolab::Start;
            if (prop.trigger().parameters()) {
                BOOST_FOREACH(const icalendar_2_0::ArrayOfParameters::baseParameter_type &param, (*prop.trigger().parameters()).baseParameter()) {
                    if (const icalendar_2_0::RelatedParamType *rel = dynamic_cast<const icalendar_2_0::RelatedParamType*> (&param)) {
                        if (rel->text() == START) {
                            relativeTo = Kolab::Start;
                        } else if (rel->text() == END) {
                            relativeTo = Kolab::End;
                        } else {
                            LOG("relativeTo not specified, default to start ");
                        }
                    }
                }
            }
            
            alarm.setRelativeStart(toDuration(*prop.trigger().duration()), relativeTo);
        } else {
            ERROR("no duration and not starttime ");
            continue;
        }
        if (prop.duration()) {
            int repeat = 0;
            if (prop.repeat()) {
                repeat = toInt(*prop.repeat());
            }
            alarm.setDuration(toDuration((*prop.duration()).duration()), repeat); //TODO check duration?
        }
        alarms.push_back(alarm);
    }
    incidence.setAlarms(alarms);
}

//--- Alarms ---

///Trait for incidence properties specialized for Event/Todo/Journal
template <typename T> struct IncidenceTrait;

template < > struct IncidenceTrait <Kolab::Event>
{
    typedef icalendar_2_0::KolabEvent KolabType;
    typedef Kolab::Event IncidenceType;
    typedef boost::shared_ptr<Kolab::Event> IncidencePtr;

    static void writeIncidence(icalendar_2_0::KolabEvent& vevent, const Kolab::Event &event)
    {
        KolabType::components_type eventComponents;
        setAlarms<icalendar_2_0::KolabEvent, IncidenceType>(eventComponents, event);
        if (!eventComponents.valarm().empty()) {
            vevent.components(eventComponents);
        }
        
        icalendar_2_0::KolabEvent::properties_type &prop = vevent.properties();

        getIncidenceProperties<icalendar_2_0::KolabEvent::properties_type>(prop, event);
        getTodoEventProperties<icalendar_2_0::KolabEvent::properties_type>(prop, event);

        if (!event.start().isValid()) {
            ERROR("Start date is missing, but is mandatory for events");
        }

        if (event.end().isValid()) {
            prop.dtend(fromDate<icalendar_2_0::KolabEvent::properties_type::dtend_type>(event.end()));
        } else if (event.duration().isValid()) {
            prop.duration(icalendar_2_0::KolabEvent::properties_type::duration_type(fromDuration(event.duration())));
        }
        
        if (event.transparency()) {
            prop.transp( icalendar_2_0::KolabEvent::properties_type::transp_type(TRANSPARENT));
        }
    }
    
    static void addIncidence(icalendar_2_0::VcalendarType::components_type &components, icalendar_2_0::KolabEvent inc) //TODO to base trait
    {
        components.vevent().push_back(inc);
    }

    static void readIncidence(Kolab::Event &event, const icalendar_2_0::KolabEvent& vevent)
    {
        const icalendar_2_0::KolabEvent::properties_type &prop = vevent.properties();

        if (!prop.dtstart()) {
            ERROR("Start date is missing, but is mandatory for events");
        }

        setIncidenceProperties<Kolab::Event, icalendar_2_0::KolabEvent::properties_type>(event, prop);
        setTodoEventProperties<Kolab::Event, icalendar_2_0::KolabEvent::properties_type>(event, prop);

        if (prop.dtend()) {
            event.setEnd(*toDate(*prop.dtend()));
            if (event.end().isUTC() != event.end().isUTC() && 
                event.end().timezone() != event.end().timezone() &&
                event.end().isDateOnly() != event.end().isDateOnly()) {
                ERROR("dtEnd has wrong timespec");
            }
        } else if (prop.duration()) {
            event.setDuration(toDuration((*prop.duration()).duration()));
        }

        if (prop.transp()) {
            if (toString(*prop.transp()) == TRANSPARENT) {
                event.setTransparency(true);
            } else {
                event.setTransparency(false);
                if (toString(*prop.transp()) != OPAQUE) {
                    ERROR("wrong transparency value " + toString(*prop.transp()));
                }
            }
        }
        if (vevent.components()) {
            getAlarms<Kolab::Event, icalendar_2_0::KolabEvent>(event, *vevent.components());
        }
    }

    static icalendar_2_0::VcalendarType::components_type::vevent_const_iterator begin(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vevent().begin();
    }
    
    static icalendar_2_0::VcalendarType::components_type::vevent_const_iterator end(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vevent().end();
    }

    static IncidencePtr resolveExceptions(const std::vector<IncidencePtr> &list)
    {
        IncidencePtr incidence = *list.begin();
        std::vector<IncidenceType> exceptions;
        for (std::vector < IncidencePtr >::const_iterator it = list.begin()+1; it != list.end(); it++) {
            exceptions.push_back(**it);
        }
        incidence->setExceptions(exceptions);
        return incidence;
    }
    
    static void addExceptions(icalendar_2_0::VcalendarType::components_type &components, const Kolab::Event &event, KolabType::properties_type props)
    {
        BOOST_FOREACH(const Kolab::Event &exception, event.exceptions()) {
            KolabType ex(props);
            writeIncidence(ex, exception);
            addIncidence(components, ex);
        }
    }
    
};

template < > struct IncidenceTrait <Kolab::Todo>
{
    typedef icalendar_2_0::KolabTodo KolabType;
    typedef Kolab::Todo IncidenceType;
    typedef boost::shared_ptr<Kolab::Todo> IncidencePtr;

    static void writeIncidence(icalendar_2_0::KolabTodo& vevent, const Kolab::Todo &todo)
    {
        KolabType::components_type eventComponents;
        setAlarms<icalendar_2_0::KolabTodo, IncidenceType>(eventComponents, todo);
        if (!eventComponents.valarm().empty()) {
            vevent.components(eventComponents);
        }

        icalendar_2_0::KolabTodo::properties_type &prop = vevent.properties();

        getIncidenceProperties<icalendar_2_0::KolabTodo::properties_type>(prop, todo);
        getTodoEventProperties<icalendar_2_0::KolabTodo::properties_type>(prop, todo);

        if (!todo.relatedTo().empty()) {
            icalendar_2_0::KolabTodo::properties_type::related_to_sequence list;
            const std::vector<std::string> &l = todo.relatedTo();
            BOOST_FOREACH(const std::string &relatedTo, l) {
                list.push_back(icalendar_2_0::KolabTodo::properties_type::related_to_type(relatedTo));
            }
            prop.related_to(list);
        }
        if (todo.due().isValid()) {
            prop.due(fromDate<icalendar_2_0::KolabTodo::properties_type::due_type>(todo.due()));
        }
        if (todo.percentComplete() > 0) {
            prop.percent_complete(icalendar_2_0::KolabTodo::properties_type::percent_complete_type(fromInt<icalendar_2_0::IntegerPropertyType::integer_type>(todo.percentComplete())));
        }
    }
    
    static void addIncidence(icalendar_2_0::VcalendarType::components_type &components, icalendar_2_0::KolabTodo inc) //TODO to base trait
    {
        components.vtodo().push_back(inc);
    }

    static void readIncidence(Kolab::Todo &todo, const icalendar_2_0::KolabTodo& vevent)
    {
        const icalendar_2_0::KolabTodo::properties_type &prop = vevent.properties();

        setIncidenceProperties<Kolab::Todo, icalendar_2_0::KolabTodo::properties_type>(todo, prop);
        setTodoEventProperties<Kolab::Todo, icalendar_2_0::KolabTodo::properties_type>(todo, prop);

        if (!prop.related_to().empty()) {
            BOOST_FOREACH(icalendar_2_0::KolabTodo::properties_type::related_to_type p, prop.related_to()) {
                todo.addRelatedTo(p.text());
            }
        }
        if (prop.due()) {
            todo.setDue(*toDate(*prop.due()));
        }
        if (prop.percent_complete()) {
            todo.setPercentComplete(toInt(*prop.percent_complete()));
        }
        if (vevent.components()) {
            getAlarms<Kolab::Todo, icalendar_2_0::KolabTodo>(todo, *vevent.components());
        }
    }

    static icalendar_2_0::VcalendarType::components_type::vevent_const_iterator begin(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vtodo().begin();
    }
    
    static icalendar_2_0::VcalendarType::components_type::vevent_const_iterator end(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vtodo().end();
    }
    
    static IncidencePtr resolveExceptions(const std::vector<IncidencePtr> &list)
    {
        IncidencePtr incidence = *list.begin();
        std::vector<IncidenceType> exceptions;
        for (std::vector < IncidencePtr >::const_iterator it = list.begin()+1; it != list.end(); it++) {
            exceptions.push_back(**it);
        }
        incidence->setExceptions(exceptions);
        return incidence;
    }
    
    static void addExceptions(icalendar_2_0::VcalendarType::components_type &components, const Kolab::Todo &event, KolabType::properties_type props)
    {
        BOOST_FOREACH(const Kolab::Todo &exception, event.exceptions()) {
            KolabType ex(props);
            writeIncidence(ex, exception);
            addIncidence(components, ex);
        }
    }
    
};

template < > struct IncidenceTrait <Kolab::Journal>
{
    typedef icalendar_2_0::KolabJournal KolabType;
    typedef Kolab::Journal IncidenceType;
    typedef boost::shared_ptr<Kolab::Journal> IncidencePtr;

    static void writeIncidence(icalendar_2_0::KolabJournal& vjournal, const Kolab::Journal &journal)
    {
        icalendar_2_0::KolabJournal::properties_type &prop = vjournal.properties();
        getIncidenceProperties<icalendar_2_0::KolabJournal::properties_type>(prop, journal);
    }
    
    static void addIncidence(icalendar_2_0::VcalendarType::components_type &components, icalendar_2_0::KolabJournal inc)
    {
        components.vjournal().push_back(inc);
    }

    static void readIncidence(Kolab::Journal &journal, const icalendar_2_0::KolabJournal& vjournal)
    {
        const icalendar_2_0::KolabJournal::properties_type &prop = vjournal.properties();
        setIncidenceProperties<Kolab::Journal, icalendar_2_0::KolabJournal::properties_type>(journal, prop);
    }

    static icalendar_2_0::VcalendarType::components_type::vjournal_const_iterator begin(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vjournal().begin();
    }
    
    static icalendar_2_0::VcalendarType::components_type::vjournal_const_iterator end(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vjournal().end();
    }
    
    static IncidencePtr resolveExceptions(const std::vector<IncidencePtr> &list)
    {
        return *list.begin();
    }
    
    static void addExceptions(icalendar_2_0::VcalendarType::components_type &, const Kolab::Journal &, KolabType::properties_type)
    {
    }
    
};

template < > struct IncidenceTrait <Kolab::Freebusy>
{
    typedef icalendar_2_0::KolabFreebusy KolabType;
    typedef Kolab::Freebusy IncidenceType;
    typedef boost::shared_ptr<Kolab::Freebusy> IncidencePtr;

    static void writeIncidence(icalendar_2_0::KolabFreebusy& vfreebusy, const Kolab::Freebusy &fb)
    {
        icalendar_2_0::KolabFreebusy::properties_type &prop = vfreebusy.properties();

        if (fb.start().isValid() && fb.end().isValid()) {
            if ((fb.start().isUTC() || fb.start().isDateOnly()) && (fb.end().isUTC() || fb.end().isDateOnly())) {
                prop.dtstart(fromDate<icalendar_2_0::KolabFreebusy::properties_type::dtstart_type>(fb.start()));
                prop.dtend(fromDate<icalendar_2_0::KolabFreebusy::properties_type::dtend_type>(fb.end()));
            } else {
                WARNING("Start/end is not in UTC, but it MUST be, skipping");
            }
        }

        if (fb.organizer().isValid()) {
            prop.organizer(fromContactReference<icalendar_2_0::KolabFreebusy::properties_type::organizer_type>(fb.organizer()));
        }
        if (!fb.periods().empty()) {
            BOOST_FOREACH (const Kolab::FreebusyPeriod &fbPeriod, fb.periods()) {
                icalendar_2_0::KolabFreebusy::properties_type::freebusy_type fb;
                
                icalendar_2_0::BasePropertyType::parameters_type params;
                std::string fbtype;
                switch (fbPeriod.type()) {
                    case FreebusyPeriod::Busy:
                        fbtype = BUSY;
                        break;
                    case FreebusyPeriod::Tentative:
                        fbtype = BUSY_TENTATIVE;
                        break;
                    case FreebusyPeriod::OutOfOffice:
                        fbtype = BUSY_OUTOFOFFICE;
                        break;
                    case FreebusyPeriod::Invalid:
                    default:
                        WARNING("Invalid fb type");
                        continue;
                }
                params.baseParameter().push_back(icalendar_2_0::FbtypeParamType(fbtype));
                if (!fbPeriod.eventUid().empty() || fbPeriod.eventSummary().empty() || fbPeriod.eventLocation().empty()) {
                    params.baseParameter().push_back(icalendar_2_0::XFBevent(fbPeriod.eventUid(), fbPeriod.eventSummary(), fbPeriod.eventLocation()));
                }
                fb.parameters(params);

                BOOST_FOREACH (const Kolab::Period &period, fbPeriod.periods()) {
                    if (period.start.isDateOnly() || period.end.isDateOnly()) {
                        WARNING("Period is date-only but must be date-time");
                        continue;
                    }
                    if (!period.start.isUTC() || !period.end.isUTC()) {
                        WARNING("Period is not in UTC, but it MUST be, skipping");
                        continue;
                    }
                    icalendar_2_0::PeriodType p(Shared::fromDateTime(period.start));
                    p.end(Shared::fromDateTime(period.end));
                    fb.period().push_back(p);
                } 

                prop.freebusy().push_back(fb);
            }
        }
    }

    static void addIncidence(icalendar_2_0::VcalendarType::components_type &components, icalendar_2_0::KolabFreebusy inc)
    {
        components.vfreebusy().push_back(inc);
    }

    static void readIncidence(Kolab::Freebusy &freebusy, const icalendar_2_0::KolabFreebusy& vfreebusy)
    {
        const icalendar_2_0::KolabFreebusy::properties_type &prop = vfreebusy.properties();

        freebusy.setUid(toString(prop.uid()));
        freebusy.setTimestamp(*toDate(prop.dtstamp()));

        if (prop.dtstart()) {
            freebusy.setStart(*toDate(*prop.dtstart()));
        }
        if (prop.dtend()) {
            freebusy.setEnd(*toDate(*prop.dtend()));
        }
        if (prop.organizer()) {
            freebusy.setOrganizer(toContactReference(*prop.organizer()));
        }
        if (!prop.freebusy().empty()) {
            std::vector<Kolab::FreebusyPeriod> fbperiods;


            BOOST_FOREACH(icalendar_2_0::FreebusyPropType aProp, prop.freebusy()) {
                Kolab::FreebusyPeriod fbPeriod;
                fbPeriod.setType(Kolab::FreebusyPeriod::Busy);
                if (aProp.parameters()) {
                    const icalendar_2_0::FreebusyPropType::parameters_type &parameters = *aProp.parameters();
                    for (icalendar_2_0::FreebusyPropType::parameters_type::baseParameter_const_iterator it(parameters.baseParameter().begin()); it != parameters.baseParameter().end(); it++) {
                        if (const icalendar_2_0::FbtypeParamType * p = dynamic_cast<const icalendar_2_0::FbtypeParamType*> (&*it)) {
                            if (p->text() == BUSY) {
                                fbPeriod.setType(Kolab::FreebusyPeriod::Busy);
                            } else if (p->text() == BUSY_OUTOFOFFICE) {
                                fbPeriod.setType(Kolab::FreebusyPeriod::OutOfOffice);
                            } else if (p->text() == BUSY_TENTATIVE) {
                                fbPeriod.setType(Kolab::FreebusyPeriod::Tentative);
                            } else {
                                WARNING("Invalid fb type, default to busy");
                            }
                        }
                        if (const icalendar_2_0::XFBevent * p = dynamic_cast<const icalendar_2_0::XFBevent*> (&*it)) {
                            fbPeriod.setEvent(p->uid(), p->summary(), p->location());
                        }

                    }
                }

                std::vector <Kolab::Period > periods;
                BOOST_FOREACH(icalendar_2_0::FreebusyPropType::period_type period, aProp.period()) {
                    if (!period.end()) {
                        WARNING("Period end date is required and duration is not supported, skipping period");
                        continue;
                    }
                    periods.push_back(Kolab::Period(*Shared::toDate(period.start()), *Shared::toDate(*period.end())));
                }
                fbPeriod.setPeriods(periods);

                fbperiods.push_back(fbPeriod);
            }
            

            freebusy.setPeriods(fbperiods);
        }

    }

    static icalendar_2_0::VcalendarType::components_type::vfreebusy_const_iterator begin(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vfreebusy().begin();
    }

    static icalendar_2_0::VcalendarType::components_type::vfreebusy_const_iterator end(const icalendar_2_0::VcalendarType::components_type &components)
    {
        return components.vfreebusy().end();
    }
    
    static IncidencePtr resolveExceptions(const std::vector<IncidencePtr> &list)
    {
        return *list.begin();
    }
    
    static void addExceptions(icalendar_2_0::VcalendarType::components_type &, const Kolab::Freebusy &, KolabType::properties_type)
    {
    }

};


//////////////////////////////////=========================================


template <typename T>
std::string serializeIncidence(const typename T::IncidenceType &incidence, const std::string productid = std::string()) {
    
    using namespace icalendar_2_0;
    typedef typename T::KolabType KolabType;
    
    clearErrors();

    try {

        typename KolabType::properties_type::uid_type uid( getUID(incidence.uid()));
        setCreatedUid(uid.text());
        typename KolabType::properties_type::dtstamp_type dtstamp;
        if (incidence.lastModified().isValid()) {
            dtstamp.date_time(fromDateTime(incidence.lastModified()));
        } else {
            dtstamp.date_time(fromDateTime(timestamp()));
        }

        typename KolabType::properties_type::created_type created;
        if (incidence.created().isValid()) {
            created.date_time(fromDateTime(incidence.created()));
        } else {
            created.date_time(fromDateTime(timestamp()));
        }
        typename KolabType::properties_type eventProps(uid, created, dtstamp);
        
        KolabType inc(eventProps);
        
        T::writeIncidence(inc, incidence);

        VcalendarType::components_type components;
        T::addIncidence(components, inc);

        T::addExceptions(components, incidence, eventProps);
        
        VcalendarType::properties_type::prodid_type prodid(getProductId(productid));
        VcalendarType::properties_type::version_type version(XCAL_VERSION);
        VcalendarType::properties_type::x_kolab_version_type x_kolab_version(KOLAB_FORMAT_VERSION);
        
        VcalendarType::properties_type properties(prodid, version, x_kolab_version);
        
        VcalendarType vcalendar(properties, components);
        
        IcalendarType icalendar(vcalendar);

        xml_schema::namespace_infomap map;
        map[""].name = XCAL_NAMESPACE;
        
        std::ostringstream ostringstream;
        icalendar_2_0::icalendar(ostringstream, icalendar, map);
        return ostringstream.str();
    } catch  (const xml_schema::exception& e) {
        CRITICAL("failed to write Incidence");
    } catch (...) {
        CRITICAL("Unhandled exception");
    }
    return std::string();
}


template <typename T>
typename T::IncidencePtr deserializeIncidence(const std::string& s, bool isUrl)
{
    using namespace icalendar_2_0;
    typedef typename T::IncidencePtr IncidencePtr;
    typedef typename T::IncidenceType IncidenceType;
    typedef typename T::KolabType KolabType;
    
    clearErrors();
    try {
        std::auto_ptr<icalendar_2_0::IcalendarType> icalendar;
        if (isUrl) {
            xsd::cxx::xml::dom::auto_ptr <xercesc::DOMDocument > doc = XMLParserWrapper::inst().parseFile(s);
            if (doc.get()) {
                icalendar = icalendar_2_0::icalendar(doc);
            }
        } else {
            xsd::cxx::xml::dom::auto_ptr <xercesc::DOMDocument > doc = XMLParserWrapper::inst().parseString(s);
            if (doc.get()) {
                icalendar = icalendar_2_0::icalendar(doc);
            }
        }
        
        if (!icalendar.get()) {
            CRITICAL("Failed to parse calendar!");
            return IncidencePtr();
        }

        const icalendar_2_0::VcalendarType &vcalendar = icalendar->vcalendar();

        std::vector < IncidencePtr > incidences;
        for (typename xsd::cxx::tree::sequence< KolabType >::const_iterator it(T::begin(vcalendar.components())); it != T::end(vcalendar.components()); it++) {
            IncidencePtr e = IncidencePtr(new IncidenceType);
            const KolabType &event = *it;
            T::readIncidence(*e, event);
            incidences.push_back(e);
        }
        
        setProductId( vcalendar.properties().prodid().text() );
        global_xCalVersion = vcalendar.properties().version().text();
        setKolabVersion( vcalendar.properties().x_kolab_version().text() );

        if (incidences.empty()) {
            CRITICAL("no incidence in object");
            return IncidencePtr();
        }
        return T::resolveExceptions(incidences);
    } catch  (const xml_schema::exception& e) {
        std::cerr <<  e << std::endl;
    } catch (...) {
        CRITICAL("Unhandled exception");
    }
    CRITICAL("Failed to read incidence!");
    return IncidencePtr();
}

template <typename T>
std::string serializeFreebusy(const Kolab::Freebusy &incidence, const std::string productid = std::string()) {

    using namespace icalendar_2_0;
    typedef typename T::KolabType KolabType;

    clearErrors();

    try {

        typename KolabType::properties_type::uid_type uid( getUID(incidence.uid()));
        setCreatedUid(uid.text());
        typename KolabType::properties_type::dtstamp_type dtstamp;
        dtstamp.date_time(fromDateTime(timestamp()));
        typename KolabType::properties_type eventProps(uid, dtstamp);

        KolabType inc(eventProps);

        T::writeIncidence(inc, incidence);

        VcalendarType::components_type components;
        T::addIncidence(components, inc);

        VcalendarType::properties_type::prodid_type prodid(getProductId(productid));
        VcalendarType::properties_type::version_type version(XCAL_VERSION);
        VcalendarType::properties_type::x_kolab_version_type x_kolab_version(KOLAB_FORMAT_VERSION);

        VcalendarType::properties_type properties(prodid, version, x_kolab_version);

        VcalendarType vcalendar(properties, components);

        IcalendarType icalendar(vcalendar);

        xml_schema::namespace_infomap map;
        map[""].name = XCAL_NAMESPACE;

        std::ostringstream ostringstream;
        icalendar_2_0::icalendar(ostringstream, icalendar, map);
        return ostringstream.str();
    } catch  (const xml_schema::exception& e) {
        std::cerr <<  e << std::endl;
    } catch (...) {
        CRITICAL("Unhandled exception");
    }
    CRITICAL("failed to write Incidence");
    return std::string();
}

    }
}//Namespace

#endif

