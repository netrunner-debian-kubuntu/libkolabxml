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

#ifndef INCIDENCE_P
#define INCIDENCE_P

#include "kolabcontainers.h"

namespace Kolab {
    
    struct PrivateIncidence
    {
        PrivateIncidence()
        : sequence(0),
        classification(ClassPublic),
        thisAndFuture(false),
        priority(0),
        status(StatusUndefined){}
        
        std::string uid;
        cDateTime created;
        cDateTime lastModified;
        int sequence;
        Classification classification;
        std::vector< std::string > categories;
        std::vector< std::string > relatedTo;
        cDateTime start;
        
        cDateTime recurrenceID;
        bool thisAndFuture;
        std::string summary;
        std::string description;
        std::string location;
        int priority;
        Status status;
        RecurrenceRule rrule;
        std::vector< cDateTime > recurrenceDates;
        std::vector< cDateTime > exceptionDates;
        ContactReference organizer;
        Duration duration;
        
        std::vector<Attendee> attendees;
        std::vector<Attachment> attachments;
        std::vector<CustomProperty> customProperties;
        
        std::vector<Alarm> alarms;
    };
    
}

#endif
