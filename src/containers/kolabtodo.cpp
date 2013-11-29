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

#include "kolabtodo.h"
#include "incidence_p.h"

namespace Kolab {
    
    
struct Todo::Private: public PrivateIncidence
{
    Private()
    : PrivateIncidence(),
    percentComplete(0){}
    
    cDateTime due;
    int percentComplete;
    std::vector< Todo > exceptions;
};

Todo::Todo()
: d(new Todo::Private())
{
    
}

Todo::Todo(const Todo &other)
: d(new Todo::Private())
{
    *d = *other.d;
}

Todo::~Todo()
{
    
}

void Todo::operator=(const Kolab::Todo &other)
{
    *d = *other.d;
}

bool Todo::isValid() const
{
    return !d->uid.empty();
}

void Todo::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string Todo::uid() const
{
    return d->uid;
}

void Todo::setCreated(const Kolab::cDateTime &created)
{
    d->created = created;
}

cDateTime Todo::created() const
{
    return d->created;
}

void Todo::setLastModified(const Kolab::cDateTime &lastMod)
{
    d->lastModified = lastMod;
}

cDateTime Todo::lastModified() const
{
    return d->lastModified;
}

void Todo::setSequence(int sequence)
{
    d->sequence = sequence;
}

int Todo::sequence() const
{
    return d->sequence;
}

void Todo::setClassification(Classification class_)
{
    d->classification = class_;
}

Classification Todo::classification() const
{
    return d->classification;
}

void Todo::setCategories(const std::vector< std::string > &categories)
{
    d->categories = categories;
}

void Todo::addCategory(const std::string &cat)
{
    d->categories.push_back(cat);
}

std::vector< std::string > Todo::categories() const
{
    return d->categories;
}

void Todo::setRelatedTo(const std::vector< std::string > &related)
{
    d->relatedTo = related;
}

void Todo::addRelatedTo(const std::string &related)
{
    d->relatedTo.push_back(related);
}

std::vector< std::string > Todo::relatedTo() const
{
    return d->relatedTo;
}

void Todo::setStart(const Kolab::cDateTime &start)
{
    d->start = start;
}

cDateTime Todo::start() const
{
    return d->start;
}

void Todo::setDue(const Kolab::cDateTime &due)
{
    d->due = due;
}

cDateTime Todo::due() const
{
    return d->due;
}

void Todo::setRecurrenceID(const Kolab::cDateTime &rID, bool thisandfuture)
{
    d->recurrenceID = rID;
    d->thisAndFuture = thisandfuture;
}

cDateTime Todo::recurrenceID() const
{
    return d->recurrenceID;
}

bool Todo::thisAndFuture() const
{
    return d->thisAndFuture;
}

void Todo::setSummary(const std::string &summary)
{
    d->summary = summary;
}

std::string Todo::summary() const
{
    return d->summary;
}

void Todo::setDescription(const std::string &description)
{
    d->description = description;
}

std::string Todo::description() const
{
    return d->description;
}

void Todo::setPriority(int priority)
{
    d->priority = priority;
}

int Todo::priority() const
{
    return d->priority;
}

void Todo::setStatus(Status status)
{
    d->status = status;
}

Status Todo::status() const
{
    return d->status;
}

void Todo::setPercentComplete(int complete)
{
    d->percentComplete = complete;
}
    
int Todo::percentComplete() const
{
    return d->percentComplete;
}

void Todo::setLocation(const std::string &location)
{
    d->location = location;
}

std::string Todo::location() const
{
    return d->location;
}

void Todo::setRecurrenceRule(const Kolab::RecurrenceRule &rrule)
{
    d->rrule = rrule;
}

RecurrenceRule Todo::recurrenceRule() const
{
    return d->rrule;
}

void Todo::setRecurrenceDates(const std::vector< cDateTime > &dates)
{
    d->recurrenceDates = dates;
}

void Todo::addRecurrenceDate(const Kolab::cDateTime &dt)
{
    d->recurrenceDates.push_back(dt);
}

std::vector< cDateTime > Todo::recurrenceDates() const
{
    return d->recurrenceDates;
}

void Todo::setExceptionDates(const std::vector< cDateTime > &dates)
{
    d->exceptionDates = dates;
}

void Todo::addExceptionDate(const Kolab::cDateTime &dt)
{
    d->exceptionDates.push_back(dt);
}

std::vector< cDateTime > Todo::exceptionDates() const
{
    return d->exceptionDates;
}

void Todo::setOrganizer(const ContactReference &organizer)
{
    d->organizer = organizer;
}

ContactReference Todo::organizer() const
{
    return d->organizer;
}

void Todo::setAttendees(const std::vector< Attendee > &attendees)
{
    d->attendees = attendees;
}

std::vector< Attendee > Todo::attendees() const
{
    return d->attendees;
}

void Todo::setAttachments(const std::vector< Attachment > &attach)
{
    d->attachments = attach;
}

std::vector< Attachment > Todo::attachments() const
{
    return d->attachments;
}

void Todo::setUrl(const std::string &url)
{
    d->url = url;
}

std::string Todo::url() const
{
    return d->url;
}

void Todo::setCustomProperties(const std::vector< CustomProperty > &prop)
{
    d->customProperties = prop;
}

std::vector< CustomProperty > Todo::customProperties() const
{
    return d->customProperties;
}

void Todo::setExceptions(const std::vector< Todo > &exceptions)
{
    d->exceptions = exceptions;
}

std::vector< Todo > Todo::exceptions() const
{
    return d->exceptions;
}

void Todo::setAlarms(const std::vector< Alarm > &alarms)
{
    d->alarms = alarms;
}

std::vector< Alarm > Todo::alarms() const
{
    return d->alarms;
}

}
