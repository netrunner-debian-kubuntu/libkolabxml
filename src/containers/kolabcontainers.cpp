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

#include "kolabcontainers.h"
#include "incidence_p.h"

namespace Kolab {
    
struct cDateTime::Private {
    Private()
    : year(-1),
    month(-1),
    day(-1),
    hour(-1),
    minute(-1),
    second(-1),
    isUtc(false){}

    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    bool isUtc;
    std::string timezone;
};

cDateTime::cDateTime()
: d(new cDateTime::Private())
{

}

cDateTime::cDateTime(int year, int month, int day, int hour, int minute, int second, bool isUtc)
: d(new cDateTime::Private())
{
    d->year = year;
    d->month = month;
    d->day = day;
    d->hour = hour;
    d->minute = minute;
    d->second = second;
    d->isUtc = isUtc;
}

cDateTime::cDateTime(const std::string& timezone, int year, int month, int day, int hour, int minute, int second)
: d(new cDateTime::Private())
{
    d->year = year;
    d->month = month;
    d->day = day;
    d->hour = hour;
    d->minute = minute;
    d->second = second;
    d->timezone = timezone;
}

cDateTime::cDateTime(int year, int month, int day)
: d(new cDateTime::Private())
{
    d->year = year;
    d->month = month;
    d->day = day;
}

cDateTime::cDateTime(const Kolab::cDateTime &other)
: d(new cDateTime::Private())
{
    *d = *other.d;
}

cDateTime::~cDateTime()
{

}


void cDateTime::operator=(const Kolab::cDateTime &other)
{
   *d = *other.d;
}

bool cDateTime::operator==(const Kolab::cDateTime &other) const
{
    if ( d->year == other.year() &&
        d->month == other.month() &&
        d->day == other.day() &&
        d->hour == other.hour() &&
        d->minute == other.minute() &&
        d->second== other.second() &&
        d->isUtc== other.isUTC() &&
        d->timezone== other.timezone()) {
        return true;
    }
    return false;
}



int cDateTime::year() const
{
    return d->year;
}

int cDateTime::month() const
{
    return d->month;
}

int cDateTime::day() const
{
    return d->day;
}

int cDateTime::hour() const
{
    return d->hour;
}

int cDateTime::minute() const
{
    return d->minute;
}

int cDateTime::second() const
{
    return d->second;
}

bool cDateTime::isDateOnly() const
{
    if ((d->hour < 0) && (d->minute < 0) && (d->second < 0)) {
        return true;
    }
    return false;
}

void cDateTime::setDate(int year, int month, int day)
{
    d->year = year;
    d->month = month;
    d->day = day;
}
void cDateTime::setTime(int hour, int minute, int second)
{
    d->hour = hour;
    d->minute = minute;
    d->second = second;
}
void cDateTime::setTimezone(const std::string &tz)
{
    d->timezone = tz;
}
void cDateTime::setUTC(bool utc)
{
    d->isUtc = utc;
}

bool cDateTime::isUTC() const
{
    return d->isUtc;
}

std::string cDateTime::timezone() const
{
    return d->timezone;
}

bool cDateTime::isValid() const
{
    return (d->year >= 0 && d->month >= 0 && d->day >= 0);
}

struct RecurrenceRule::Private
{
    Private()
    : freq(FreqNone),
    weekstart(Monday),
    count(-1),
    interval(1){};
    
    Frequency freq;
    Weekday weekstart;
    cDateTime end;
    int count;
    int interval;
    std::vector<int> bysecond;
    std::vector<int> byminute;
    std::vector<int> byhour;
    std::vector<DayPos> byday;
    std::vector<int> bymonthday;
    std::vector<int> byyearday;
    std::vector<int> byweekno;
    std::vector<int> bymonth;
};

RecurrenceRule::RecurrenceRule()
:   d(new RecurrenceRule::Private)
{
    
}

RecurrenceRule::RecurrenceRule(const Kolab::RecurrenceRule &other)
:   d(new RecurrenceRule::Private)
{
    *d = *other.d;
}

void RecurrenceRule::operator=(const Kolab::RecurrenceRule &other)
{
    *d = *other.d;
}

bool RecurrenceRule::operator==(const Kolab::RecurrenceRule &other) const
{
    if ( d->freq == other.frequency() &&
        d->weekstart == other.weekStart() &&
        d->end == other.end() &&
        d->count == other.count() &&
        d->interval == other.interval() &&
        d->bysecond == other.bysecond() &&
        d->byminute == other.byminute() &&
        d->byhour == other.byhour() &&
        d->byday == other.byday() &&
        d->bymonthday == other.bymonthday() &&
        d->byyearday == other.byyearday() &&
        d->byweekno == other.byweekno() &&
        d->bymonth == other.bymonth()) {
            return true;
        }
        return false;
}

RecurrenceRule::~RecurrenceRule()
{
    
}

void RecurrenceRule::setFrequency(RecurrenceRule::Frequency freq)
{
    d->freq = freq;
}

RecurrenceRule::Frequency RecurrenceRule::frequency() const
{
    return d->freq;
}

void RecurrenceRule::setWeekStart(Kolab::Weekday weekstart)
{
    d->weekstart = weekstart;
}

Kolab::Weekday RecurrenceRule::weekStart() const
{
    return d->weekstart;
}

void RecurrenceRule::setEnd(const Kolab::cDateTime &end)
{
    d->end = end;
}

cDateTime RecurrenceRule::end() const
{
    return d->end;
}

void RecurrenceRule::setCount(int count)
{
    d->count = count;
}

int RecurrenceRule::count() const
{
    return d->count;
}

void RecurrenceRule::setInterval(int interval)
{
    d->interval = interval;
}

int RecurrenceRule::interval() const
{
    return d->interval;
}

void RecurrenceRule::setBysecond(const std::vector< int >&by)
{
    d->bysecond = by;
}


std::vector< int > RecurrenceRule::bysecond() const
{
    return d->bysecond;
}

void RecurrenceRule::setByminute(const std::vector< int > &by)
{
    d->byminute = by;
}

std::vector< int > RecurrenceRule::byminute() const
{
    return d->byminute;
}

void RecurrenceRule::setByhour(const std::vector< int > &by)
{
    d->byhour = by;
}

std::vector< int > RecurrenceRule::byhour() const
{
    return d->byhour;
}

void RecurrenceRule::setByday(const std::vector< DayPos > &by)
{
    d->byday = by;
}

std::vector< DayPos > RecurrenceRule::byday() const
{
    return d->byday;
}

void RecurrenceRule::setBymonthday(const std::vector< int > &by)
{
    d->bymonthday = by;
}

std::vector< int > RecurrenceRule::bymonthday() const
{
    return d->bymonthday;
}

void RecurrenceRule::setByyearday(const std::vector< int > &by)
{
    d->byyearday = by;
}

std::vector< int > RecurrenceRule::byyearday() const
{
    return d->byyearday;
}

void RecurrenceRule::setByweekno(const std::vector< int > &by)
{
    d->byweekno = by;
}

std::vector< int > RecurrenceRule::byweekno() const
{
    return d->byweekno;
}

void RecurrenceRule::setBymonth(const std::vector< int > &by)
{
    d->bymonth = by;
}

std::vector< int > RecurrenceRule::bymonth() const
{
    return d->bymonth;
}

bool RecurrenceRule::isValid() const
{
    if (d->freq == FreqNone) {
        return false;
    }
    return true;
}



struct Attendee::Private
{
    Private()
    : partStat(PartNeedsAction),
    role(Required),
    rsvp(false),
    cutype(CutypeIndividual)
    {};
    
    ContactReference contact;
    PartStatus partStat;
    Role role;
    bool rsvp;
    std::vector<ContactReference> delegatedTo;
    std::vector<ContactReference> delegatedFrom;
    Cutype cutype;
};

Attendee::Attendee()
:   d(new Attendee::Private)
{

}

Attendee::Attendee(const ContactReference& contact)
:   d(new Attendee::Private)
{
    d->contact = contact; 
}

Attendee::Attendee(const Kolab::Attendee &other)
:   d(new Attendee::Private)
{
    *d = *other.d;
}

void Attendee::operator=(const Kolab::Attendee &other)
{
    *d = *other.d;
}

Attendee::~Attendee()
{

}

bool Attendee::operator==(const Kolab::Attendee &other) const
{
    if ( d->contact == other.contact() &&
        d->partStat == other.partStat() &&
        d->role == other.role() &&
        d->rsvp == other.rsvp() &&
        d->delegatedTo == other.delegatedTo() &&
        d->delegatedFrom == other.delegatedFrom() &&
        d->cutype == other.cutype()
    ) {
        return true;
    }
    return false;
}

bool Attendee::isValid() const 
{ 
    return d->contact.isValid();
};

void Attendee::setContact(const ContactReference &c)
{
    d->contact = c;
}

ContactReference Attendee::contact() const
{
    return d->contact;
}


void Attendee::setPartStat(PartStatus partStat)
{
    d->partStat = partStat;
}

PartStatus Attendee::partStat() const
{
    return d->partStat;
}

void Attendee::setRole(Role role)
{
    d->role = role;
}

Role Attendee::role() const
{
    return d->role;
}

void Attendee::setRSVP(bool rsvp)
{
    d->rsvp = rsvp;
}

bool Attendee::rsvp() const
{
    return d->rsvp;
}

void Attendee::setDelegatedTo(const std::vector< ContactReference > &del)
{
    d->delegatedTo = del;
}

std::vector< ContactReference > Attendee::delegatedTo() const
{
    return d->delegatedTo;
}

void Attendee::setDelegatedFrom(const std::vector< ContactReference > &del)
{
    d->delegatedFrom = del;
}

std::vector< ContactReference > Attendee::delegatedFrom() const
{
    return d->delegatedFrom;
}

void Attendee::setCutype(Cutype type)
{
    d->cutype = type;
}

Cutype Attendee::cutype() const
{
    return d->cutype;
}


struct Attachment::Private
{
    std::string uri;
    std::string data;
    std::string mimetype;
    std::string label;
    bool isValid;
};

Attachment::Attachment()
:   d(new Attachment::Private)
{
    d->isValid = false;
}

Attachment::Attachment(const Kolab::Attachment &other)
:   d(new Attachment::Private)
{
    *d = *other.d;
}

void Attachment::operator=(const Kolab::Attachment &other)
{
    *d = *other.d;
}

Attachment::~Attachment()
{
}

bool Attachment::operator==(const Kolab::Attachment &other) const
{
    return ( d->uri == other.uri() &&
        d->data == other.data() &&
        d->label == other.label() &&
        d->mimetype == other.mimetype() );
}

void Attachment::setUri(const std::string &uri, const std::string& mimetype)
{
    d->isValid = true;
    d->uri = uri;
    d->mimetype = mimetype;
}

std::string Attachment::uri() const
{
    return d->uri;
}

std::string Attachment::mimetype() const
{
    return d->mimetype;
}

void Attachment::setLabel(const std::string &label)
{
    d->label = label;
}

std::string Attachment::label() const
{
    return d->label;
}

void Attachment::setData(const std::string &data, const std::string& mimetype)
{
    d->isValid = true;
    d->data = data;
    d->mimetype = mimetype;
}

std::string Attachment::data() const
{
    return d->data;
}

bool Attachment::isValid() const
{
    return d->isValid;
}



struct Alarm::Private
{
    Private(): relativeTo(Start),
    numrepeat(0),
    type(Alarm::InvalidAlarm) {};
    std::string text;
    Attachment audioFile;
    std::string summary;
    std::vector<ContactReference> attendees;
    cDateTime start;
    Duration relativeDuration;
    Relative relativeTo;
    Duration duration;
    int numrepeat;
    Type type;
    
};

Alarm::Alarm()
:   d(new Alarm::Private)
{
}

Alarm::Alarm(const std::string &text)
:   d(new Alarm::Private)
{
    d->text = text;
    d->type = DisplayAlarm;
}


Alarm::Alarm(const Kolab::Attachment& audio)
:   d(new Alarm::Private)
{
    d->audioFile = audio;
    d->type = AudioAlarm;
}

Alarm::Alarm(const std::string& summary, const std::string& description, const std::vector<ContactReference> attendees)
:   d(new Alarm::Private)
{
    d->summary = summary;
    d->text = description;
    d->attendees = attendees;
    d->type = EMailAlarm;
    
}

Alarm::Alarm(const Kolab::Alarm &other)
:   d(new Alarm::Private)
{
    *d = *other.d;
}

void Alarm::operator=(const Kolab::Alarm &other)
{
    *d = *other.d;
}

Alarm::~Alarm()
{
}

bool Alarm::operator==(const Kolab::Alarm &other) const
{
    return ( d->text == other.description() &&
        d->text == other.description() &&
        d->audioFile == other.audioFile() &&
        d->summary == other.summary() &&
        d->attendees == other.attendees() &&
        d->start == other.start() &&
        d->relativeDuration == other.relativeStart() &&
        d->relativeTo == other.relativeTo() &&
        d->duration == other.duration() &&
        d->numrepeat == other.numrepeat() );
}

std::string Alarm::text() const
{
    return d->text;
}

Attachment Alarm::audioFile() const
{
    return d->audioFile;
}

std::string Alarm::summary() const
{
    return d->summary;
}

std::string Alarm::description() const
{
    return d->text;
}

std::vector<ContactReference> Alarm::attendees() const
{
    return d->attendees;
}

void Alarm::setStart(const Kolab::cDateTime &start)
{
    d->start = start;
}

cDateTime Alarm::start() const
{
    return d->start;
}

void Alarm::setRelativeStart(const Kolab::Duration &duration, Relative relativeTo)
{
    d->relativeDuration = duration;
    d->relativeTo = relativeTo;
}

Duration Alarm::relativeStart() const
{
    return d->relativeDuration;
}

Relative Alarm::relativeTo() const
{
    return d->relativeTo;
}

void Alarm::setDuration(const Kolab::Duration &duration, int numrepeat)
{
    d->numrepeat = numrepeat;
    d->duration = duration;
}

Duration Alarm::duration() const
{
    return d->duration;
}

int Alarm::numrepeat() const
{
    return d->numrepeat;
}

Alarm::Type Alarm::type() const
{
    return d->type;
}






}//Namespace
