#ifndef SERIALIZERS_H
#define SERIALIZERS_H

#include <QByteArray>
#include <QString>
#include <src/kolabformat.h>

namespace QTest {
    template<>
    char *toString(const Kolab::cDateTime &dt)
    {
        QByteArray ba = "Kolab::cDateTime(";
        ba += QByteArray::number(dt.year()) + ", " + QByteArray::number(dt.month())+ ", " + QByteArray::number(dt.day()) + ", ";
        ba += QByteArray::number(dt.hour()) + ", " + QByteArray::number(dt.minute()) + ", " + QByteArray::number(dt.second())+ ", ";
        ba += QString(dt.isUTC()?QString("UTC"):QString("TZ: "+QString::fromStdString(dt.timezone()))).toLatin1();
        ba += ")";
        return qstrdup(ba.data());
    }

    template<>
    char *toString(const std::vector<Kolab::cDateTime> &v)
    {
        QByteArray ba = "vector<Kolab::cDateTime>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QByteArray(toString(v.at(i)))+ "\n";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::Attendee &a)
    {
        QByteArray ba = "Kolab::Attendee(";
        ba += QString::fromStdString(a.contact().email()).toLatin1() + ", " + QString::fromStdString(a.contact().name()).toLatin1()+ ", " + 
        QByteArray::number(a.partStat()) + ", " + QByteArray::number(a.role())  + ", " + QByteArray::number(a.rsvp())  + ", " + 
        QString::fromStdString(a.contact().uid()).toLatin1();
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<Kolab::Attendee> &v)
    {
        QByteArray ba = "vector<Kolab::Attendee>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QByteArray(toString(v.at(i)))+ ", ";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
     
    template<>
    char *toString(const std::string &s)
    {
        QByteArray ba = "string(";
        ba += QString::fromStdString(s).toLatin1();
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<std::string> &v)
    {
        QByteArray ba = "vector<std::string>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QByteArray(toString(v.at(i)))+ ", ";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<int> &v)
    {
        QByteArray ba = "vector<int>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QString::number(v.at(i)).toLatin1()+ ", ";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::Duration &dt)
    {
        QByteArray ba = "Kolab::Duration(";
        ba += QByteArray::number(dt.weeks()) + ", " + QByteArray::number(dt.days())+ ", " + QByteArray::number(dt.hours()) + ", ";
        ba += QByteArray::number(dt.minutes()) + ", " + QByteArray::number(dt.seconds()) + ", " + QByteArray::number(dt.isNegative());
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::DayPos &dt)
    {
        QByteArray ba = "Kolab::DayPos(";
        ba += QByteArray::number(dt.occurence()) + ", " + QByteArray::number(dt.weekday());
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::Attachment &a)
    {
        QByteArray ba = "Kolab::Attachment(";
        ba += QString::fromStdString(a.uri()).toLatin1() + ", " + QString::fromStdString(a.mimetype()).toLatin1()+ ", " + 
        QString::fromStdString(a.label()).toLatin1();
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<Kolab::Attachment> &v)
    {
        QByteArray ba = "vector<Kolab::Attachment>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QByteArray(toString(v.at(i)))+ "\n";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::ContactReference &a)
    {
        QByteArray ba = "Kolab::ContactReference(";
        ba += QString::fromStdString(a.email()).toLatin1()+ ", ";
        ba += QString::fromStdString(a.name()).toLatin1()+ ", ";
        ba += QString::fromStdString(a.uid()).toLatin1();
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<Kolab::ContactReference> &v)
    {
        QByteArray ba = "vector<Kolab::ContactReference>(";
        for (std::size_t i = 0; i < v.size(); i++) {
                ba += QByteArray(toString(v.at(i)))+ "\n";
            }
            ba += ")";
            return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::Alarm &a)
    {
        QByteArray ba = "Kolab::Alarm(";
        ba += QByteArray::number(a.type()) + "\n " + QString::fromStdString(a.summary())+ "\n " + 
        QString::fromStdString(a.text())+"\n " +toString(a.duration()) + "\n " + QByteArray::number(a.numrepeat())+  "\n " + toString(a.start()) + "\n " +
        toString(a.relativeStart())  + "\n " +  QByteArray::number(a.relativeTo()) + "\n " + toString(a.audioFile()) +  "\n " + toString(a.attendees()) + "\n ";
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<Kolab::Alarm> &v)
    {
        QByteArray ba = "vector<Kolab::Alarm>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QByteArray(toString(v.at(i)))+ "\n";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::Related &a)
    {
        QByteArray ba = "Kolab::Related(";
        ba += QString::fromStdString(a.text()).toLatin1()+ "\n " + 
        QString::fromStdString(a.uri()).toLatin1()+"\n " +
        QByteArray::number(a.relationTypes()) + "\n " +
        QByteArray::number(a.type()) + "\n ";
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<Kolab::Related> &v)
    {
        QByteArray ba = "vector<Kolab::Related>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QByteArray(toString(v.at(i)))+ "\n";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const Kolab::Affiliation &a)
    {
        QByteArray ba = "Kolab::Affiliation(";
        ba += QString::fromStdString(a.organisation()).toLatin1()+ "\n " + 
        QString::fromStdString(a.logo())+"\n " + "\n " + toString(a.roles())+  "\n " + toString(a.relateds()) + "\n " +
        toString(a.addresses())  + "\n ";
        ba += ")";
        return qstrdup(ba.data());
    }
    
    template<>
    char *toString(const std::vector<Kolab::Affiliation> &v)
    {
        QByteArray ba = "vector<Kolab::Affiliation>(";
        for (std::size_t i = 0; i < v.size(); i++) {
            ba += QByteArray(toString(v.at(i)))+ "\n";
        }
        ba += ")";
        return qstrdup(ba.data());
    }
    


 }
 
#endif
 
