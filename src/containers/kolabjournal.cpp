/*
 * Copyright (C) 2012  Christian Mollekopf <mollekopf@kolabsys.com>
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

#include "kolabjournal.h"
#include "incidence_p.h"

namespace Kolab {

struct Journal::Private: public PrivateIncidence
{
    Private()
    : PrivateIncidence() {}
};

Journal::Journal()
: d(new Journal::Private())
{
}

Journal::Journal(const Journal &other)
: d(new Journal::Private())
{
    *d = *other.d;
}

Journal::~Journal()
{
}

void Journal::operator=(const Kolab::Journal &other)
{
    *d = *other.d;
}

bool Journal::isValid() const
{
    return !d->uid.empty();
}

void Journal::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string Journal::uid() const
{
    return d->uid;
}

void Journal::setCreated(const Kolab::cDateTime &created)
{
    d->created = created;
}

cDateTime Journal::created() const
{
    return d->created;
}

void Journal::setLastModified(const Kolab::cDateTime &lastMod)
{
    d->lastModified = lastMod;
}

cDateTime Journal::lastModified() const
{
    return d->lastModified;
}

void Journal::setSequence(int sequence)
{
    d->sequence = sequence;
}

int Journal::sequence() const
{
    return d->sequence;
}

void Journal::setClassification(Classification class_)
{
    d->classification = class_;
}

Classification Journal::classification() const
{
    return d->classification;
}

void Journal::setCategories(const std::vector< std::string > &categories)
{
    d->categories = categories;
}

void Journal::addCategory(const std::string &cat)
{
    d->categories.push_back(cat);
}

std::vector< std::string > Journal::categories() const
{
    return d->categories;
}

void Journal::setStart(const Kolab::cDateTime &start)
{
    d->start = start;
}

cDateTime Journal::start() const
{
    return d->start;
}

void Journal::setSummary(const std::string &summary)
{
    d->summary = summary;
}

std::string Journal::summary() const
{
    return d->summary;
}

void Journal::setDescription(const std::string &description)
{
    d->description = description;
}

std::string Journal::description() const
{
    return d->description;
}

void Journal::setComment(const std::string &comment)
{
    d->comment = comment;
}

std::string Journal::comment() const
{
    return d->comment;
}

void Journal::setStatus(Status status)
{
    d->status = status;
}

Status Journal::status() const
{
    return d->status;
}

void Journal::setAttendees(const std::vector< Attendee > &attendees)
{
    d->attendees = attendees;
}

std::vector< Attendee > Journal::attendees() const
{
    return d->attendees;
}

void Journal::setAttachments(const std::vector< Attachment > &attach)
{
    d->attachments = attach;
}

std::vector< Attachment > Journal::attachments() const
{
    return d->attachments;
}

void Journal::setCustomProperties(const std::vector< CustomProperty > &prop)
{
    d->customProperties = prop;
}

std::vector< CustomProperty > Journal::customProperties() const
{
    return d->customProperties;
}


}//Namespace
