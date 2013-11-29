/*
 * Copyright (C) 2013  Christian Mollekopf <mollekopf@kolabsys.com>
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

#ifndef OBJECTVALIDATION_H
#define OBJECTVALIDATION_H

namespace Kolab {

class Event;
class Todo;
class Journal;
class Freebusy;
class Contact;
class DistList;
class Note;
class Configuration;
class File;

/**
 * The validations in here are supposed to validate kolab objects beyond what the xml schema covers.
 *
 * A typical usecase is to check if event start/end date have the same timezone, or if all used timezones are valid.
 */

void validate(const Kolab::Event &event);
void validate(const Kolab::Todo &todo);
void validate(const Kolab::Journal &journal);
void validate(const Kolab::Freebusy &freebusy);
void validate(const Kolab::Contact &contact);
void validate(const Kolab::DistList &distlist);
void validate(const Kolab::Note &note);
void validate(const Kolab::Configuration &configuration);
void validate(const Kolab::File &file);

}

#endif

