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

#include "kolabevent.h"
#include "kolabevent_p.h"

namespace Kolab {

Event::Event()
: d(new Event::Private())
{
//     std::cout << "ctor" << std::endl;
}

Event::Event(const Event &other)
: d(new Event::Private())
{
    *d = *other.d;
//     std::cout << "cctor" << std::endl;
}

Event::~Event()
{
//     std::cout << "dtor" << std::endl;
}

void Event::operator=(const Kolab::Event &other)
{
    *d = *other.d;
}

bool Event::isValid() const
{
    return !d->uid.empty();
}

void Event::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string Event::uid() const
{
    return d->uid;
}

void Event::setCreated(const Kolab::cDateTime &created)
{
    d->created = created;
}

cDateTime Event::created() const
{
    return d->created;
}

void Event::setLastModified(const Kolab::cDateTime &lastMod)
{
    d->lastModified = lastMod;
}

cDateTime Event::lastModified() const
{
    return d->lastModified;
}

void Event::setSequence(int sequence)
{
    d->sequence = sequence;
}

int Event::sequence() const
{
    return d->sequence;
}

void Event::setClassification(Classification class_)
{
    d->classification = class_;
}

Classification Event::classification() const
{
    return d->classification;
}

void Event::setCategories(const std::vector< std::string > &categories)
{
    d->categories = categories;
}

void Event::addCategory(const std::string &cat)
{
    d->categories.push_back(cat);
}

std::vector< std::string > Event::categories() const
{
    return d->categories;
}

void Event::setStart(const Kolab::cDateTime &start)
{
    d->start = start;
}

cDateTime Event::start() const
{
    return d->start;
}

void Event::setEnd(const Kolab::cDateTime &end)
{
    d->end = end;
}

cDateTime Event::end() const
{
    return d->end;
}

void Event::setDuration(const Duration &duration)
{
    d->duration = duration;
}

Duration Event::duration() const
{
    return d->duration;
}


void Event::setRecurrenceID(const Kolab::cDateTime &rID, bool thisandfuture)
{
    d->recurrenceID = rID;
    d->thisAndFuture = thisandfuture;
}

cDateTime Event::recurrenceID() const
{
    return d->recurrenceID;
}

bool Event::thisAndFuture() const
{
    return d->thisAndFuture;
}

void Event::setSummary(const std::string &summary)
{
    d->summary = summary;
}

std::string Event::summary() const
{
    return d->summary;
}

void Event::setDescription(const std::string &description)
{
    d->description = description;
}

std::string Event::description() const
{
    return d->description;
}

void Event::setPriority(int priority)
{
    d->priority = priority;
}

int Event::priority() const
{
    return d->priority;
}

void Event::setStatus(Status status)
{
    d->status = status;
}

Status Event::status() const
{
    return d->status;
}

void Event::setLocation(const std::string &location)
{
    d->location = location;
}

std::string Event::location() const
{
    return d->location;
}

void Event::setRecurrenceRule(const Kolab::RecurrenceRule &rrule)
{
    d->rrule = rrule;
}

RecurrenceRule Event::recurrenceRule() const
{
    return d->rrule;
}

void Event::setRecurrenceDates(const std::vector< cDateTime > &dates)
{
    d->recurrenceDates = dates;
}

void Event::addRecurrenceDate(const Kolab::cDateTime &dt)
{
    d->recurrenceDates.push_back(dt);
}

std::vector< cDateTime > Event::recurrenceDates() const
{
    return d->recurrenceDates;
}

void Event::setExceptionDates(const std::vector< cDateTime > &dates)
{
    d->exceptionDates = dates;
}

void Event::addExceptionDate(const Kolab::cDateTime &dt)
{
    d->exceptionDates.push_back(dt);
}

std::vector< cDateTime > Event::exceptionDates() const
{
    return d->exceptionDates;
}

void Event::setTransparency(bool isTransparent)
{
    d->isTransparent = isTransparent;
}

bool Event::transparency() const
{
    return d->isTransparent;
}

void Event::setOrganizer(const ContactReference &organizer)
{
    d->organizer = organizer;
}

ContactReference Event::organizer() const
{
    return d->organizer;
}

void Event::setAttendees(const std::vector< Attendee > &attendees)
{
    d->attendees = attendees;
}

std::vector< Attendee > Event::attendees() const
{
    return d->attendees;
}

void Event::setAttachments(const std::vector< Attachment > &attach)
{
    d->attachments = attach;
}

std::vector< Attachment > Event::attachments() const
{
    return d->attachments;
}

void Event::setUrl(const std::string &url)
{
    d->url = url;
}

std::string Event::url() const
{
    return d->url;
}

void Event::setCustomProperties(const std::vector< CustomProperty > &prop)
{
    d->customProperties = prop;
}

std::vector< CustomProperty > Event::customProperties() const
{
    return d->customProperties;
}

void Event::setExceptions(const std::vector< Event > &exceptions)
{
    d->exceptions = exceptions;
}

std::vector< Event > Event::exceptions() const
{
    return d->exceptions;
}

void Event::setAlarms(const std::vector< Alarm > &alarms)
{
    d->alarms = alarms;
}

std::vector< Alarm > Event::alarms() const
{
    return d->alarms;
}




}
