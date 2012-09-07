/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  Christian Mollekopf <chrigi_1@fastmail.fm>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#include "kolabfreebusy.h"
#include "kolabfreebusy_p.h"

namespace Kolab {



FreebusyPeriod::FreebusyPeriod()
: d(new FreebusyPeriod::Private())
{

}

FreebusyPeriod::FreebusyPeriod(const FreebusyPeriod &other)
: d(new FreebusyPeriod::Private())
{
    *d = *other.d;
}

void FreebusyPeriod::operator=(const FreebusyPeriod &other)
{
    *d = *other.d;
}

FreebusyPeriod::~FreebusyPeriod()
{

}

bool FreebusyPeriod::operator==(const FreebusyPeriod &other) const
{
    return d->type == other.type() &&
           d->eventUid == other.eventUid() &&
           d->eventSummary == other.eventSummary() &&
           d->eventLocation == other.eventLocation() &&
           d->periods == other.periods();
}

bool FreebusyPeriod::isValid() const
{
    return d->type == Invalid;
}

void FreebusyPeriod::setType(FBType t)
{
    d->type = t;
}

FreebusyPeriod::FBType FreebusyPeriod::type() const
{
    return d->type;
}

void FreebusyPeriod::setPeriods(const std::vector< Period > &periods)
{
    d->periods = periods;
}

std::vector< Period > FreebusyPeriod::periods() const
{
    return d->periods;
}

void FreebusyPeriod::setEvent(const std::string& uid, const std::string& summary, const std::string& location)
{
    d->eventUid = uid;
    d->eventSummary = summary;
    d->eventLocation = location;
}

std::string FreebusyPeriod::eventUid() const
{
    return d->eventUid;
}

std::string FreebusyPeriod::eventSummary() const
{
    return d->eventSummary;
}

std::string FreebusyPeriod::eventLocation() const
{
    return d->eventLocation;
}




Freebusy::Freebusy()
: d(new Freebusy::Private())
{
}

Freebusy::Freebusy(const Freebusy &other)
: d(new Freebusy::Private())
{
    *d = *other.d;
}

Freebusy::~Freebusy()
{
}

void Freebusy::operator=(const Freebusy &other)
{
    *d = *other.d;
}

bool Freebusy::isValid() const
{
    return !d->uid.empty();
}

void Freebusy::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string Freebusy::uid() const
{
    return d->uid;
}

void Freebusy::setTimestamp(const cDateTime &t)
{
    d->timestamp = t;
}

cDateTime Freebusy::timestamp() const
{
    return d->timestamp;
}

void Freebusy::setStart(const cDateTime &s)
{
    d->start = s;
}

cDateTime Freebusy::start() const
{
    return d->start;
}

void Freebusy::setEnd(const cDateTime &e)
{
    d->end = e;
}

cDateTime Freebusy::end() const
{
    return d->end;
}

void Freebusy::setOrganizer(const ContactReference &c)
{
    d->organizer = c;
}

ContactReference Freebusy::organizer() const
{
    return d->organizer;
}

void Freebusy::setPeriods(const std::vector< FreebusyPeriod > &periods)
{
    d->periods = periods;
}

std::vector< FreebusyPeriod > Freebusy::periods() const
{
    return d->periods;
}


}