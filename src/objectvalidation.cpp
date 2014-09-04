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

#include "objectvalidation.h"

#include "kolabevent.h"
#include "kolabtodo.h"
#include "kolabjournal.h"
#include "kolabcontact.h"
#include "kolabconfiguration.h"
#include "kolabfile.h"
#include "utils.h"
#include "tztable.h"
#include <boost/unordered_set.hpp>

namespace Kolab {

static boost::unordered_set<std::string> initializeTzSet() {
    boost::unordered_set<std::string> set;
    for (int i = 0; i < numOlsonTimezones; i++) {
        set.insert(olsonTimezones[i]);
    }
    return set;
}

const boost::unordered_set<std::string> tzSet = initializeTzSet();

bool isValid(const cDateTime &datetime)
{
    if (!datetime.isValid()) {
        return true;
    }
    const std::string tz = datetime.timezone();
    if (!tz.empty()) {
        if (datetime.isUTC() && !tz.empty()) {
            Utils::logMessage("A UTC datetime may not have a timezone", "", 0, Error);
            return false;
        }
        if (tzSet.find(tz) == tzSet.end()) {
            Utils::logMessage("Not a valid olson timezone: " + tz, "", 0, Error);
            return false;
        }
    }
    return true;
}

#define ASSERT(arg) \
    do {\
        if ( !(arg) ) { \
            Utils::logMessage(#arg " is false", __FILE__, __LINE__, Error); \
        } \
    } while(0)

#define ASSERTEQUAL(arg1, arg2) \
    do {\
        if ( (arg1) != (arg2) ) { \
            Utils::logMessage(#arg1 " != " #arg2, __FILE__, __LINE__, Error); \
        } \
    } while(0)

#define ASSERTEXISTING(arg) \
    do {\
        if ( !(arg).isValid() ) { \
            Utils::logMessage(#arg " is not set", __FILE__, __LINE__, Error); \
        } \
    } while(0)

#define ASSERTVALID(arg) \
    do {\
        if ( (arg).isValid() && !isValid((arg)) ) { \
            Utils::logMessage(#arg " is not valid", __FILE__, __LINE__, Error); \
        } \
    } while(0)

void validate(const Event &event)
{
    ASSERTEXISTING(event.start());
    ASSERTVALID(event.start());
    ASSERTVALID(event.end());
    if (event.start().isValid() && event.end().isValid()) {
        ASSERTEQUAL(event.start().isDateOnly(), event.end().isDateOnly());
    }
}

void validate(const Todo& todo)
{
    ASSERTVALID(todo.start());
    ASSERTVALID(todo.due());
    if (todo.start().isValid() && todo.due().isValid()) {
        ASSERTEQUAL(todo.start().isDateOnly(), todo.due().isDateOnly());
    }
}

void validate(const Journal& journal)
{
    ASSERTVALID(journal.start());
}

void validate(const Contact& contact)
{

}

void validate(const DistList& distlist)
{

}

void validate(const Freebusy& freebusy)
{

}

void validate(const Note& note)
{

}

void validate(const Configuration& configuration)
{

}

void validate(const File& file)
{

}

}

