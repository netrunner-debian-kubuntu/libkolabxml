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

#ifndef KOLABFORMAT_H
#define KOLABFORMAT_H

#include <string>
#include "kolabcontainers.h"
#include "kolabtodo.h"
#include "kolabevent.h"
#include "kolabjournal.h"
#include "kolabcontact.h"
#include "kolabnote.h"
#include "kolabconfiguration.h"
#include "kolabfreebusy.h"
#include "kolabfile.h"
#include "global_definitions.h"

/**
 * Kolab Format v3 Implementation
 *
 * Note that this code is threadsafe, as it uses thread-local storage.
 * 
 * Example:
 *
 * Kolab::Event event;
 * event.setStart(Kolab::cDateTime("Europe/Zurich",2011,10,10,12,1,1));
 * event.setEnd(Kolab::cDateTime("Europe/Zurich",2012,5,5,3,4,4));
 * event.setLastModified(Kolab::cDateTime(2011,10,11,12,1,2,true));
 * event.setCreated(Kolab::cDateTime(2011,10,11,12,1,3,true));
 * std::string serialization = Kolab::writeEvent(ev, "Product ID");
 * std::string serializedUID = Kolab::getSerializedUID(); //returns the UID of the just serialized event
 * if (Kolab::error() != Kolab::NoError) {
 *     std::cout << "Error on write " << Kolab::errorMessage();
 * }
 * Kolab::Event e = Kolab::readEvent(serialization, false);
 * if (Kolab::error() != Kolab::NoError) {
 *     std::cout << "Error on read " << Kolab::errorMessage();
 * }
 *
 */
namespace Kolab {

/**
 * Check to see if serialization/deserialization was successful.
 *
 * Returns true if at least an Error occurred. Warning is not considered an error.
 */
bool errorOccurred();

/**
 * Returns the highest severity of the errors which occured.
 *
 * Note that a Warning is not considered an error, use errorOccurred to check for errors.
 */
Kolab::ErrorSeverity error();

/**
 * Returns the corresponding error message to the reported error.
 */
std::string errorMessage();

/**
 * Returns productId string of the last deserialized object.
 * Updated during deserialization of object.
 */
std::string productId();

/**
 * Returns KolabFormat version of the last deserialized object.
 * Updated during deserialization of object.
 */
std::string xKolabVersion();

/**
 * Returns xCal version of the last deserialized xCal object.
 * Updated during deserialization of object.
 */
std::string xCalVersion();

/**
 * Returns the UID of the last serialized  object.
 * Updated during serialization of the object.
 */
std::string getSerializedUID();

/**
 * Returns a generated uid.
 */
std::string generateUID();

/**
 * Use this function to override the timestamp which is normally generated upon serialization from the system time.
 * To override the timestamp call this function once. You will need to clear the timestamp manually by setting a default constructed cDateTime().
 * 
 * The timestamp is used as lastModifiedDate on every write, and as creation-date on the first write (when there is no creation-date yet).
 * 
 * The supplied timestamp must be in UTC format.
 * 
 * This function exists primarily for testing purpose.
 */
void overrideTimestamp(const Kolab::cDateTime &dt);

/**
 * Serializing functions for kolab objects.
 * 
 * Check error() to see if the operation was successful.
 * 
 * @param isUrl if true, the file with the url @param s is opened and read.
 * @param productId Sets the productid on serialization. Note that the versionstring of libkolabxml will be appended to the productId.
 */

Kolab::Event readEvent(const std::string& s, bool isUrl);
std::string writeEvent(const Kolab::Event &, const std::string& productId = std::string());

Kolab::Todo readTodo(const std::string& s, bool isUrl);
std::string writeTodo(const Kolab::Todo &, const std::string& productId = std::string());

Kolab::Journal readJournal(const std::string& s, bool isUrl);
std::string writeJournal(const Kolab::Journal &, const std::string& productId = std::string());

Kolab::Freebusy readFreebusy(const std::string& s, bool isUrl);
std::string writeFreebusy(const Kolab::Freebusy &, const std::string& productId = std::string());

Kolab::Contact readContact(const std::string& s, bool isUrl);
std::string writeContact(const Kolab::Contact &, const std::string& productId = std::string());

Kolab::DistList readDistlist(const std::string& s, bool isUrl);
std::string writeDistlist(const Kolab::DistList &, const std::string& productId = std::string());

Kolab::Note readNote(const std::string& s, bool isUrl);
std::string writeNote(const Kolab::Note &, const std::string& productId = std::string());

Kolab::Configuration readConfiguration(const std::string& s, bool isUrl);
std::string writeConfiguration(const Kolab::Configuration &, const std::string& productId = std::string());

Kolab::File readFile(const std::string& s, bool isUrl);
std::string writeFile(const Kolab::File &, const std::string& productId = std::string());

}

#endif // KOLABFORMAT_H

