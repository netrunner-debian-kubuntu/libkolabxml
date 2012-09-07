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

#ifndef KOLABFREEBUSY_H
#define KOLABFREEBUSY_H

#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include "kolabcontainers.h"
namespace Kolab {

struct Period {
    Period(){};
    Period(const cDateTime& s, const cDateTime &e): start(s), end(e){};
    bool operator==(const Period &other) const { return start == other.start && end == other.end; };
    bool isValid() const { return start.isValid() && end.isValid(); };
    cDateTime start;
    cDateTime end;
};
    
class FreebusyPeriod {
public:
    FreebusyPeriod();
    ~FreebusyPeriod();
    FreebusyPeriod(const FreebusyPeriod &);
    void operator=(const FreebusyPeriod &);
    bool operator==(const FreebusyPeriod &) const;

    bool isValid() const;

    enum FBType {
        Invalid,
        Busy,
        Tentative,
        OutOfOffice
    };
    void setType(FBType t);
    FBType type() const;

    void setEvent(const std::string &uid, const std::string &summary, const std::string &location);
    std::string eventUid() const;
    std::string eventSummary() const;
    std::string eventLocation() const;
    
    void setPeriods(const std::vector<Period> &);
    std::vector<Period> periods() const;
private:
    struct Private;
    boost::scoped_ptr<Private> d;
};

class Freebusy {
public:
    Freebusy();
    ~Freebusy();
    Freebusy(const Freebusy &);
    void operator=(const Freebusy &);
//         bool operator==(const Freebusy &) const;

    bool isValid() const;

    void setUid(const std::string &);
    std::string uid() const;

    void setTimestamp(const cDateTime &);
    cDateTime timestamp() const;

    void setStart(const cDateTime &);
    cDateTime start() const;

    void setEnd(const cDateTime &);
    cDateTime end() const;

    void setOrganizer(const ContactReference &);
    ContactReference organizer() const;

    void setPeriods(const std::vector<FreebusyPeriod> &);
    std::vector<FreebusyPeriod> periods() const;

private:
    struct Private;
    boost::scoped_ptr<Private> d;
};

}

#endif

