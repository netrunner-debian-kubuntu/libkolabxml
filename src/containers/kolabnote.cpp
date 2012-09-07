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

#include "kolabnote.h"

namespace Kolab {

struct Note::Private
{
    Private()
    : classification(ClassPublic){}
    
    std::string uid;
    cDateTime created;
    cDateTime lastModified;
    std::vector< std::string > categories;
    Classification classification;
    
    std::string summary;
    std::string description;
    std::string color;
    
    std::vector<Attachment> attachments;
    std::vector<CustomProperty> customProperties;
};

Note::Note()
: d(new Note::Private())
{
}

Note::Note(const Note &other)
: d(new Note::Private())
{
    *d = *other.d;
}

Note::~Note()
{
}

void Note::operator=(const Kolab::Note &other)
{
    *d = *other.d;
}

bool Note::operator==(const Kolab::Note& other) const
{
    return ( d->uid == other.uid() &&
    d->created == other.created() &&
    d->lastModified == other.lastModified() &&
    d->categories == other.categories() &&
    d->classification == other.classification() &&
    d->summary == other.summary() &&
    d->description == other.description() &&
    d->color == other.color() &&
    d->attachments == other.attachments() &&
    d->customProperties == other.customProperties());
}

bool Note::isValid() const
{
    return !d->uid.empty();
}

void Note::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string Note::uid() const
{
    return d->uid;
}

void Note::setCreated(const Kolab::cDateTime &created)
{
    d->created = created;
}

cDateTime Note::created() const
{
    return d->created;
}

void Note::setLastModified(const Kolab::cDateTime &lastMod)
{
    d->lastModified = lastMod;
}

cDateTime Note::lastModified() const
{
    return d->lastModified;
}

void Note::setClassification(Classification class_)
{
    d->classification = class_;
}

Classification Note::classification() const
{
    return d->classification;
}

void Note::setCategories(const std::vector< std::string > &categories)
{
    d->categories = categories;
}

void Note::addCategory(const std::string &cat)
{
    d->categories.push_back(cat);
}

std::vector< std::string > Note::categories() const
{
    return d->categories;
}

void Note::setSummary(const std::string &summary)
{
    d->summary = summary;
}

std::string Note::summary() const
{
    return d->summary;
}

void Note::setDescription(const std::string &description)
{
    d->description = description;
}

std::string Note::description() const
{
    return d->description;
}

void Note::setColor(const std::string &color)
{
    d->color = color;
}

std::string Note::color() const
{
    return d->color;
}

void Note::setAttachments(const std::vector< Attachment > &attach)
{
    d->attachments = attach;
}

std::vector< Attachment > Note::attachments() const
{
    return d->attachments;
}

void Note::setCustomProperties(const std::vector< CustomProperty > &prop)
{
    d->customProperties = prop;
}

std::vector< CustomProperty > Note::customProperties() const
{
    return d->customProperties;
}

} //Note
