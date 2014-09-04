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

#ifndef KOLAB_CONTAINERS_H
#define KOLAB_CONTAINERS_H
#include <string>
#include <vector>
#include <boost/scoped_ptr.hpp>

namespace Kolab {

/**
* - months: 1-12
* - days: 1-31
* - hour: 0-23
* - minute: 0-59
* - second: 0-59
*/
class cDateTime {
public:
    cDateTime();
    cDateTime(int year, int month, int day, int hour, int minute, int second, bool isUtc=false);
    cDateTime(const std::string &timezone, int year, int month, int day, int hour, int minute, int second);
    cDateTime(int year, int month, int day);
    ~cDateTime();
    cDateTime(const cDateTime &);
    void operator=(const cDateTime &);
    bool operator==(const cDateTime &) const;
   
    
    void setDate(int year, int month, int day);
    int year() const;
    int month() const;
    int day() const;
    
    bool isDateOnly() const;
    
    void setTime(int hour, int minute, int second);
    int hour() const;
    int minute() const;
    int second() const;
    
    void setUTC(bool);
    bool isUTC() const;
    void setTimezone(const std::string &);
    std::string timezone() const;
    
    bool isValid() const;
private:
    struct Private;
    boost::scoped_ptr<Private> d;
};

enum Classification {
    ClassPublic,
    ClassPrivate,
    ClassConfidential
};

enum Status {
    StatusUndefined,
    StatusNeedsAction,
    StatusCompleted,
    StatusInProcess,
    StatusCancelled,
    StatusTentative,
    StatusConfirmed,
    StatusDraft,
    StatusFinal
};

enum Weekday {
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday
};

struct DayPos {
    DayPos(): mIsValid(false){};
    DayPos(int occurrence, Weekday weekday): mOccurrence(occurrence), mWeekday(weekday), mIsValid(true){};
    bool operator==(const DayPos &other) const { return mOccurrence == other.mOccurrence && mWeekday == other.mWeekday; };
    int occurence() const { return mOccurrence; };
    Weekday weekday() const { return mWeekday; };
    bool isValid() { return mIsValid; };
private:
    int mOccurrence;
    Weekday mWeekday;
    bool mIsValid;
};

class Attachment {
public:
    Attachment();
    Attachment(const Kolab::Attachment &);
    ~Attachment();

    void operator=(const Attachment &);
    bool operator==(const Attachment &) const;

    void setUri(const std::string &uri, const std::string &mimetype);
    std::string uri() const;

     ///Un-encoded binary content, Implies embedded, will be encoded
     void setData(const std::string &, const std::string &mimetype);
     ///Decoded binary content.
     std::string data() const;
     //TODO add possibility to set already encoded data and uri to be embedded as performance/convenience improvement?
//     ///Base64 encoded binary content, Implies embedded
//      void setEncodedData(const std::string &, const std::string &mimetype);

    std::string mimetype() const;

    ///User visible label
    void setLabel(const std::string &);
    std::string label() const;
    
    bool isValid() const;
private:
    struct Private;
    boost::scoped_ptr<Private> d;
};

enum Relative {
    Start,
    End
};

struct Duration {
    Duration():mWeeks(0), mDays(0), mHours(0), mMinutes(0), mSeconds(0), mNegative(false), valid(false){};
    Duration(int weeks, bool negative = false): mWeeks(weeks), mDays(0), mHours(0), mMinutes(0), mSeconds(0), mNegative(negative), valid(true){};
    Duration(int days, int hours, int minutes, int seconds, bool negative = false): mWeeks(0), mDays(days), mHours(hours), mMinutes(minutes), mSeconds(seconds), mNegative(negative), valid(true){};
    bool operator==(const Duration &other) const{ return (/*mWeeks == other.mWeeks && 
                                                            mDays == other.mDays &&
                                                            mHours == other.mHours &&
                                                            mMinutes == other.mMinutes &&
                                                            mSeconds == other.mSeconds &&
                                                            mNegative == other.mNegative &&*/
                                                            ( ((((mWeeks * 7 + mDays) * 24 + mHours) * 60 + mMinutes) * 60 + mSeconds) ==
                                                              ((((other.mWeeks * 7 + other.mDays) * 24 + other.mHours) * 60 + other.mMinutes) * 60 + other.mSeconds) ) &&
                                                            valid == other.valid );};
    int weeks() const { return mWeeks; };
    int days() const { return mDays; };
    int hours() const { return mHours; };
    int minutes() const { return mMinutes; };
    int seconds() const { return mSeconds; };

    bool isNegative() const { return mNegative; };
    bool isValid() const { return valid; };
private:
    int mWeeks;
    int mDays;
    int mHours;
    int mMinutes;
    int mSeconds;
    bool mNegative;
    bool valid;
};

struct ContactReference {
    enum ReferenceType {
        Invalid,
        EmailReference,
        UidReference,
        EmailAndUidReference
    };
    ContactReference(): mType(Invalid) {};
    ///For xCal
    ContactReference(const std::string &email, const std::string &name = std::string(), const std::string &uid = std::string()): mType(EmailAndUidReference), mEmail(email), mUid(uid), mName(name) {};
    ///For xCard
    ContactReference(ReferenceType type, const std::string &emailOrUID, const std::string &name = std::string()): mType(type), mName(name) { 
        if (type == EmailReference) {
            mEmail = emailOrUID;
        } else {
            mUid = emailOrUID;
        }
    };
    bool operator==(const ContactReference &other) const { return mEmail == other.mEmail &&
                                                        mName == other.mName &&
                                                        mUid == other.mUid;
    };

    bool isValid() const { return mType != Invalid; };

    void setName(const std::string &name) { mName = name; };

    std::string email() const { return mEmail; };
    std::string uid() const { return mUid; };
    std::string name() const { return mName; };

    ReferenceType type() const { return mType; };

private:
    ReferenceType mType;
    std::string mEmail;
    std::string mUid;
    std::string mName;
};

class Alarm {
public:
    enum Type {
        InvalidAlarm,
        EMailAlarm,
        DisplayAlarm,
        AudioAlarm
    };
    
    Alarm();
    Alarm(const Alarm &);
    ~Alarm();

    void operator=(const Alarm &);
    bool operator==(const Alarm &other) const;

    ///EMail Alarm, @param attendees accepts only email + name and no uid
    Alarm(const std::string &summary, const std::string &description, const std::vector<ContactReference> attendees);
    std::string summary() const;
    std::string description() const;
    std::vector<ContactReference> attendees() const;

    ///Display Alarm
    Alarm(const std::string &text);
    std::string text() const;

    ///Audio Alarm
    Alarm(const Attachment &audio);
    Attachment audioFile() const;
    
    void setRelativeStart(const Duration &, Relative);
    Duration relativeStart() const;
    Relative relativeTo() const;
    
    void setStart(const cDateTime &);
    cDateTime start() const;

    void setDuration(const Duration &, int numrepeat);
    Duration duration() const;
    int numrepeat() const;
    
    Type type() const;

private:
    struct Private;
    boost::scoped_ptr<Private> d;
};


class RecurrenceRule {
public:
    
    RecurrenceRule();
    RecurrenceRule(const RecurrenceRule &);
    ~RecurrenceRule();

    void operator=(const RecurrenceRule &);
    bool operator==(const RecurrenceRule &other) const;
    
    enum Frequency {
        FreqNone,
        Yearly,
        Monthly,
        Weekly,
        Daily,
        Hourly,
        Minutely,
        Secondly
    };
    
    void setFrequency(Frequency);
    Frequency frequency() const;
    
    void setWeekStart(Weekday);
    Weekday weekStart() const;
    
    void setEnd(const cDateTime &);
    cDateTime end() const;
    
    void setCount(int count);
    int count() const;
    
    void setInterval(int);
    int interval() const;
    
    void setBysecond(const std::vector<int> &);
    std::vector<int> bysecond() const;
    
    void setByminute(const std::vector<int> &);
    std::vector<int> byminute() const;
    
    void setByhour(const std::vector<int> &);
    std::vector<int> byhour() const;
    
    void setByday(const std::vector<DayPos> &);
    std::vector<DayPos> byday() const;
    
    void setBymonthday(const std::vector<int> &);
    std::vector<int> bymonthday() const;
    
    void setByyearday(const std::vector<int> &);
    std::vector<int> byyearday() const;
    
    void setByweekno(const std::vector<int> &);
    std::vector<int> byweekno() const;
    
    void setBymonth(const std::vector<int> &);
    std::vector<int> bymonth() const;
    
    bool isValid() const;
    
private:
    struct Private;
    boost::scoped_ptr<Private> d;
};


enum PartStatus {
    PartNeedsAction,
    PartAccepted,
    PartDeclined,
    PartTentative,
    PartDelegated,
    PartInProcess,
    PartCompleted
};

enum Role {
    Required,
    Chair,
    Optional,
    NonParticipant
};

enum Cutype {
    CutypeUnknown,
    CutypeGroup,
    CutypeIndividual,
    CutypeResource,
    CutypeRoom
};

class Attendee {
public:
    Attendee();
    Attendee(const ContactReference &contact);
    Attendee(const Attendee &);
    ~Attendee();

    void operator=(const Attendee &);
    bool operator==(const Attendee &) const;

    bool isValid() const;

    void setContact(const ContactReference &);
    ContactReference contact() const;

    void setPartStat(PartStatus);
    PartStatus partStat() const;

    void setRole(Role);
    Role role() const;

    void setRSVP(bool);
    bool rsvp() const;

    void setDelegatedTo(const std::vector<ContactReference> &);
    std::vector<ContactReference> delegatedTo() const;

    void setDelegatedFrom(const std::vector<ContactReference> &);
    std::vector<ContactReference> delegatedFrom() const;

    void setCutype(Cutype);
    Cutype cutype() const;
private:
    struct Private;
    boost::scoped_ptr<Private> d;
};

struct CustomProperty {
    CustomProperty(){};
    CustomProperty(const std::string &i, const std::string &v)
    : identifier(i), value(v) {};

    bool operator==(const CustomProperty &other) const{ return (identifier == other.identifier && value == other.value);};
    std::string identifier;
    std::string value;
};

//WARNING this function copies the vector and does not modify it
template <typename T>
std::vector<T> operator<< ( std::vector<T> v, const T &s)
{
    v.push_back(s);
    return v;
}

    
}

#endif
