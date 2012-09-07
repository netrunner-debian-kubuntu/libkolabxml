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

#ifndef FREEBUSY_P
#define FREEBUSY_P

#include "kolabcontainers.h"
#include "kolabfreebusy.h"

namespace Kolab {

struct FreebusyPeriod::Private
{
    Private()
    : type(Invalid)
    {}

    FBType type;
    std::string eventUid;
    std::string eventSummary;
    std::string eventLocation;
    std::vector< Period > periods;
};

class Freebusy;

struct Freebusy::Private
{
    Private() {}

    std::string uid;
    cDateTime timestamp;
    cDateTime start;
    cDateTime end;
    ContactReference organizer;
    std::vector< FreebusyPeriod > periods;
};

}

#endif
