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

#include "kolabformat.h"

#include <iostream>
#include "xcalconversions.h"

#include "xcardconversions.h"
#include "utils.h"
#include "kolabconversions.h"
#include "objectvalidation.h"

namespace Kolab {
    
ErrorSeverity error()
{
    return Utils::getError();
}

bool errorOccurred()
{
    return Utils::getError() > Warning;
}

std::string errorMessage()
{
    return Utils::getErrorMessage();
}

std::string productId()
{
    return Utils::productId();
}

std::string xCalVersion()
{
    return Utils::xCalVersion();
}

std::string xKolabVersion()
{
    return Utils::kolabVersion();
}

std::string getSerializedUID()
{
    return Utils::createdUid();
}

std::string generateUID()
{
    return Utils::getUID();
}

void overrideTimestamp(const cDateTime& dt)
{
    Utils::setOverrideTimestamp(dt);
}

Kolab::Event readEvent(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    Kolab::XCAL::IncidenceTrait <Kolab::Event >::IncidencePtr ptr = XCAL::deserializeIncidence< XCAL::IncidenceTrait<Kolab::Event> >(s, isUrl);
    if (!ptr.get()) {
        return Kolab::Event();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeEvent(const Kolab::Event &event, const std::string& productId)
{
    Utils::clearErrors();
    validate(event);
    const std::string result = XCAL::serializeIncidence< XCAL::IncidenceTrait<Kolab::Event> >(event, productId);
    //Validate
    XCAL::deserializeIncidence< XCAL::IncidenceTrait<Kolab::Event> >(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

Kolab::Todo readTodo(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    XCAL::IncidenceTrait<Kolab::Todo>::IncidencePtr ptr = XCAL::deserializeIncidence< XCAL::IncidenceTrait<Kolab::Todo> >(s, isUrl);
    if (!ptr.get()) {
        return Kolab::Todo();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeTodo(const Kolab::Todo &event, const std::string& productId)
{
    Utils::clearErrors();
    validate(event);
    const std::string result = XCAL::serializeIncidence< XCAL::IncidenceTrait<Kolab::Todo> >(event, productId);
    //Validate
    XCAL::deserializeIncidence< XCAL::IncidenceTrait<Kolab::Todo> >(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

Journal readJournal(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    XCAL::IncidenceTrait<Kolab::Journal>::IncidencePtr ptr = XCAL::deserializeIncidence<XCAL::IncidenceTrait<Kolab::Journal> >(s, isUrl);
    if (!ptr.get()) {
        return Kolab::Journal();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeJournal(const Kolab::Journal &j, const std::string& productId)
{
    Utils::clearErrors();
    validate(j);
    const std::string result = XCAL::serializeIncidence< XCAL::IncidenceTrait<Kolab::Journal> >(j, productId);
    //Validate
    XCAL::deserializeIncidence< XCAL::IncidenceTrait<Kolab::Journal> >(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

Kolab::Freebusy readFreebusy(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    XCAL::IncidenceTrait<Kolab::Freebusy>::IncidencePtr ptr = XCAL::deserializeIncidence<XCAL::IncidenceTrait<Kolab::Freebusy> >(s, isUrl);
    if (!ptr.get()) {
        return Kolab::Freebusy();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeFreebusy(const Freebusy &f, const std::string& productId)
{
    Utils::clearErrors();
    validate(f);
    const std::string result = XCAL::serializeFreebusy<XCAL::IncidenceTrait<Kolab::Freebusy> >(f, productId);
    XCAL::deserializeIncidence<XCAL::IncidenceTrait<Kolab::Freebusy> >(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

Kolab::Contact readContact(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    boost::shared_ptr <Kolab::Contact > ptr = XCARD::deserializeCard<Kolab::Contact>(s, isUrl);
    if (!ptr.get()) {
        return Kolab::Contact();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeContact(const Contact &contact, const std::string& productId)
{
    Utils::clearErrors();
    validate(contact);
    const std::string result = XCARD::serializeCard(contact, productId);
    //Validate
    XCARD::deserializeCard<Kolab::Contact>(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

DistList readDistlist(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    boost::shared_ptr <Kolab::DistList> ptr = XCARD::deserializeCard<Kolab::DistList>(s, isUrl);
    if (!ptr.get()) {
        return Kolab::DistList();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeDistlist(const DistList &list, const std::string& productId)
{
    Utils::clearErrors();
    validate(list);
    const std::string result = XCARD::serializeCard(list, productId);
    //Validate
    XCARD::deserializeCard<Kolab::DistList>(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

Note readNote(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    boost::shared_ptr <Kolab::Note> ptr = Kolab::KolabObjects::deserializeObject<Kolab::Note>(s, isUrl);
    if (!ptr.get()) {
        return Kolab::Note();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeNote(const Note &note, const std::string& productId)
{
    Utils::clearErrors();
    validate(note);
    const std::string result = Kolab::KolabObjects::serializeObject<Kolab::Note>(note, productId);
    //Validate
    Kolab::KolabObjects::deserializeObject<Kolab::Note>(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

File readFile(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    boost::shared_ptr <Kolab::File> ptr = Kolab::KolabObjects::deserializeObject<Kolab::File>(s, isUrl);
    if (!ptr.get()) {
        return Kolab::File();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeFile(const File &file, const std::string& productId)
{
    Utils::clearErrors();
    validate(file);
    const std::string result = Kolab::KolabObjects::serializeObject<Kolab::File>(file, productId);
    //Validate
    Kolab::KolabObjects::deserializeObject<Kolab::File>(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}

Configuration readConfiguration(const std::string& s, bool isUrl)
{
    Utils::clearErrors();
    boost::shared_ptr <Kolab::Configuration> ptr = Kolab::KolabObjects::deserializeObject<Kolab::Configuration>(s, isUrl);
    if (!ptr.get()) {
        return Kolab::Configuration();
    }
    validate(*ptr);
    return *ptr;
}

std::string writeConfiguration(const Configuration &config, const std::string& productId)
{
    Utils::clearErrors();
    validate(config);
    const std::string result = Kolab::KolabObjects::serializeObject<Kolab::Configuration>(config, productId);
    //Validate
    Kolab::KolabObjects::deserializeObject<Kolab::Configuration>(result, false);
    if (errorOccurred()) {
        LOG("Error occurred while writing.")
    }
    return result;
}


}

