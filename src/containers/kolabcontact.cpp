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

#include "kolabcontact.h"

namespace Kolab {
    
struct DistList::Private
{
    Private() {}
    
    std::string uid;
    cDateTime lastModified;
    std::vector< std::string > categories;
    
    std::string name;
    std::vector<ContactReference> members;
    std::vector<CustomProperty> customProperties;
};
    
DistList::DistList()
: d(new DistList::Private())
{
    
}

DistList::DistList(const DistList &other)
: d(new DistList::Private())
{
    *d = *other.d;
}

DistList::~DistList()
{
    
}

void DistList::operator=(const Kolab::DistList &other)
{
    *d = *other.d;
}

bool DistList::isValid() const
{
    return !d->uid.empty();
}

void DistList::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string DistList::uid() const
{
    return d->uid;
}

void DistList::setLastModified(const Kolab::cDateTime &dt)
{
    d->lastModified = dt;
}

cDateTime DistList::lastModified() const
{
    return d->lastModified;
}

void DistList::setName(const std::string &name)
{
    d->name = name;
}

std::string DistList::name() const
{
    return d->name;
}

void DistList::setMembers(const std::vector< ContactReference > &members)
{
    d->members = members;
}

std::vector< ContactReference > DistList::members() const
{
    return d->members;
}

void DistList::setCustomProperties(const std::vector< CustomProperty >& c)
{
    d->customProperties = c;
}

std::vector< CustomProperty > DistList::customProperties() const
{
    return d->customProperties;
}





    
struct Contact::Private
{
    Private()
    :  addressPreferredIndex(-1),
    gender(NotSet),
    telephonesPreferredIndex(-1),
    imAddressPreferredIndex(-1),
    emailAddressPreferredIndex(-1)
    {}
    
    std::string uid;
    cDateTime created;
    cDateTime lastModified;
    std::vector< std::string > categories;
    
    std::string name;
    NameComponents nameComponents;
    std::string note;
    std::string freeBusyUrl;
    std::vector< std::string > titles;
    std::vector<Affiliation> affiliations;
    std::vector<Url> urls;
    std::vector<Address> addresses;
    int addressPreferredIndex;
    std::vector<std::string> nickNames;
    std::vector<Related> relateds;
    cDateTime bDay;
    cDateTime anniversary;
    std::string photo;
    std::string photoMimetype;
    Gender gender;
    std::vector<std::string> languages;
    std::vector<Telephone> telephones;
    int telephonesPreferredIndex;
    std::vector<std::string> imAddresses;
    int imAddressPreferredIndex;
    std::vector<Email> emailAddresses;
    int emailAddressPreferredIndex;
    std::vector<Geo> gpsPos;
    std::vector<Key> keys;
    Crypto crypto;
    std::vector<CustomProperty> customProperties;
};

Contact::Contact()
: d(new Contact::Private())
{
    
}

Contact::Contact(const Contact &other)
: d(new Contact::Private())
{
    *d = *other.d;
}

Contact::~Contact()
{
    
}

void Contact::operator=(const Kolab::Contact &other)
{
    *d = *other.d;
}

bool Contact::isValid() const
{
    return !d->uid.empty();
}

void Contact::setUid(const std::string &uid)
{
    d->uid = uid;
}

std::string Contact::uid() const
{
    return d->uid;
}

void Contact::setLastModified(const Kolab::cDateTime &dt)
{
    d->lastModified = dt;
}

cDateTime Contact::lastModified() const
{
    return d->lastModified;
}

void Contact::setCategories(const std::vector< std::string > &cat)
{
 d->categories = cat;
}

void Contact::addCategory(const std::string &cat)
{
    d->categories.push_back(cat);
}

std::vector< std::string > Contact::categories() const
{
    return d->categories;
}

void Contact::setName(const std::string &name)
{
    d->name = name;
}

std::string Contact::name() const
{
    return d->name;
}

void Contact::setNameComponents(const Kolab::NameComponents &nc)
{
    d->nameComponents = nc;
}

NameComponents Contact::nameComponents() const
{
    return d->nameComponents;
}

void Contact::setNote(const std::string &note)
{
    d->note = note;
}

std::string Contact::note() const
{
    return d->note;
}

void Contact::setFreeBusyUrl(const std::string &url)
{
    d->freeBusyUrl = url;
}

std::string Contact::freeBusyUrl() const
{
    return d->freeBusyUrl;
}

void Contact::setTitles(const std::vector< std::string >& titles)
{
    d->titles = titles;
}

std::vector< std::string > Contact::titles() const
{
    return d->titles;
}


void Contact::setAffiliations(const std::vector< Affiliation > &a)
{
    d->affiliations = a;
}

std::vector< Affiliation > Contact::affiliations() const
{
    return d->affiliations;
}

void Contact::setUrls(const std::vector<Url> &urls)
{
    d->urls = urls;
}

std::vector< Url > Contact::urls() const
{
    return d->urls;
}

void Contact::setAddresses(const std::vector< Address > &ad, int preferred)
{
    d->addresses = ad;
    d->addressPreferredIndex = preferred;
}

std::vector< Address > Contact::addresses() const
{
    return d->addresses;
}

int Contact::addressPreferredIndex() const
{
    return d->addressPreferredIndex;
}


void Contact::setNickNames(const std::vector< std::string > &n)
{
    d->nickNames = n;
}

std::vector< std::string > Contact::nickNames() const
{
    return d->nickNames;
}

void Contact::setRelateds(const std::vector< Related > &relateds)
{
    d->relateds = relateds;
}

std::vector< Related > Contact::relateds() const
{
    return d->relateds;
}

void Contact::setBDay(const Kolab::cDateTime &bday)
{
    d->bDay = bday;
}

cDateTime Contact::bDay() const
{
    return d->bDay;
}

void Contact::setAnniversary(const Kolab::cDateTime& dt)
{
    d->anniversary = dt;
}

cDateTime Contact::anniversary() const
{
    return d->anniversary;
}

void Contact::setPhoto(const std::string& data, const std::string& mimetype)
{
    d->photo = data;
    d->photoMimetype = mimetype;
}

std::string Contact::photo() const
{
    return d->photo;
}

std::string Contact::photoMimetype() const
{
    return d->photoMimetype;
}

void Contact::setGender(Contact::Gender g)
{
    d->gender = g;
}

Contact::Gender Contact::gender() const
{
    return d->gender;
}

void Contact::setLanguages(const std::vector< std::string >& l)
{
    d->languages = l;
}

std::vector< std::string > Contact::languages() const
{
    return d->languages;
}

void Contact::setTelephones(const std::vector< Telephone >& tel, int preferredIndex)
{
    d->telephonesPreferredIndex = preferredIndex;
    d->telephones = tel;
}

std::vector< Telephone > Contact::telephones() const
{
    return d->telephones;
}

int Contact::telephonesPreferredIndex() const
{
    return d->telephonesPreferredIndex;
}

void Contact::setIMaddresses(const std::vector< std::string > &adr, int preferredIndex)
{
    d->imAddresses = adr;
    d->imAddressPreferredIndex = preferredIndex;
}

std::vector< std::string > Contact::imAddresses() const
{
    return d->imAddresses;
}

int Contact::imAddressPreferredIndex() const
{
    return d->imAddressPreferredIndex;
}

void Contact::setEmailAddresses(const std::vector< Email >& email, int preferredIndex)
{
    d->emailAddresses = email;
    d->emailAddressPreferredIndex = preferredIndex;
}

std::vector< Email > Contact::emailAddresses() const
{
    return d->emailAddresses;
}

int Contact::emailAddressPreferredIndex() const
{
    return d->emailAddressPreferredIndex;
}

void Contact::setGPSpos(const std::vector< Geo >& pos)
{
    d->gpsPos = pos;
}

std::vector< Geo > Contact::gpsPos() const
{
    return d->gpsPos;
}

void Contact::setKeys(const std::vector<Key> &keys)
{
    d->keys = keys;
}

std::vector<Key> Contact::keys() const
{
    return d->keys;
}

void Contact::setCrypto(const Kolab::Crypto& c)
{
    d->crypto = c;
}

Crypto Contact::crypto() const
{
    return d->crypto;
}

void Contact::setCustomProperties(const std::vector< CustomProperty >& c)
{
    d->customProperties = c;
}

std::vector< CustomProperty > Contact::customProperties() const
{
    return d->customProperties;
}































} //Namespace

