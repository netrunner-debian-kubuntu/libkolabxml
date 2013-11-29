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

#include "kolabconfiguration.h"

namespace Kolab {

struct Configuration::Private {
    Private():  type(Invalid){}

    std::vector<CategoryColor> categoryColor;
    Dictionary dictionary;
    SnippetsCollection snippets;
    ConfigurationType type;
    std::string uid;
    cDateTime created;
    cDateTime lastModified;
};

Configuration::Configuration()
:   d(new Configuration::Private)
{
}

Configuration::Configuration(const std::vector<CategoryColor> &c)
:   d(new Configuration::Private)
{
    d->categoryColor = c;
    d->type = TypeCategoryColor;
}

Configuration::Configuration(const Dictionary &dict)
:   d(new Configuration::Private)
{
    d->dictionary = dict;
    d->type = TypeDictionary;
}

Configuration::Configuration(const SnippetsCollection &snippets)
:   d(new Configuration::Private)
{
    d->snippets = snippets;
    d->type = TypeSnippet;
}

Configuration::Configuration(const Configuration &other)
:   d(new Configuration::Private)
{
    *d = *other.d;
}

Configuration::~Configuration()
{

}

void Configuration::operator=(const Configuration &other)
{
    *d = *other.d;
}

bool Configuration::isValid() const
{
    return d->type != Invalid;
}

void Configuration::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string Configuration::uid() const
{
    return d->uid;
}

void Configuration::setCreated(const cDateTime &created)
{
    d->created = created;
}

cDateTime Configuration::created() const
{
    return d->created;
}

void Configuration::setLastModified(const cDateTime &lastModified)
{
    d->lastModified = lastModified;
}

cDateTime Configuration::lastModified() const
{
    return d->lastModified;
}


Configuration::ConfigurationType Configuration::type() const
{
    return d->type;
}

std::vector<CategoryColor> Configuration::categoryColor() const
{
    return d->categoryColor;
}

Dictionary Configuration::dictionary() const
{
    return d->dictionary;
}

SnippetsCollection Configuration::snippets() const
{
    return d->snippets;
}

} //Namespace
