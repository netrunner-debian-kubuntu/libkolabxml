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

#include "kolabfile.h"

namespace Kolab {

struct File::Private
{
    Private()
    : classification(ClassPublic){}
    
    std::string uid;
    cDateTime created;
    cDateTime lastModified;
    std::vector< std::string > categories;
    Classification classification;
    
    std::string note;
    
    Attachment attachment;
    std::vector<CustomProperty> customProperties;
};

File::File()
: d(new File::Private())
{
}

File::File(const File &other)
: d(new File::Private())
{
    *d = *other.d;
}

File::~File()
{
}

void File::operator=(const Kolab::File &other)
{
    *d = *other.d;
}

bool File::operator==(const Kolab::File& other) const
{
    return ( d->uid == other.uid() &&
    d->created == other.created() &&
    d->lastModified == other.lastModified() &&
    d->categories == other.categories() &&
    d->classification == other.classification() &&
    d->note == other.note() &&
    d->attachment == other.file() &&
    d->customProperties == other.customProperties());
}

bool File::isValid() const
{
    return !d->uid.empty();
}

void File::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string File::uid() const
{
    return d->uid;
}

void File::setCreated(const Kolab::cDateTime &created)
{
    d->created = created;
}

cDateTime File::created() const
{
    return d->created;
}

void File::setLastModified(const Kolab::cDateTime &lastMod)
{
    d->lastModified = lastMod;
}

cDateTime File::lastModified() const
{
    return d->lastModified;
}

void File::setClassification(Classification class_)
{
    d->classification = class_;
}

Classification File::classification() const
{
    return d->classification;
}

void File::setCategories(const std::vector< std::string > &categories)
{
    d->categories = categories;
}

void File::addCategory(const std::string &cat)
{
    d->categories.push_back(cat);
}

std::vector< std::string > File::categories() const
{
    return d->categories;
}

void File::setNote(const std::string &note)
{
    d->note = note;
}

std::string File::note() const
{
    return d->note;
}

void File::setFile(const Attachment &attach)
{
    d->attachment = attach;
}

Attachment File::file() const
{
    return d->attachment;
}

void File::setCustomProperties(const std::vector< CustomProperty > &prop)
{
    d->customProperties = prop;
}

std::vector< CustomProperty > File::customProperties() const
{
    return d->customProperties;
}

} //File
