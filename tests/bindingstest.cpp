#include "bindingstest.h"

#include <QObject>
#include <QtTest/QtTest>

#include "bindings/iCalendar-props.hxx"
#include "bindings/iCalendar-valtypes.hxx"
#include "bindings/kolabformat.hxx"

#include <iostream>

#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <src/kolabformat.h>
// #include <kolab/kolabkcalconversion.h>
#include <iostream>
#include <fstream>
#include "serializers.h"
#include <src/utils.h>
#include "src/containers/kolabjournal.h"
#include "libkolabxml-version.h"

void BindingsTest::categorycolorConfigurationCompletness()
{
    Kolab::CategoryColor color("name");
    color.setColor("color");
    std::vector<Kolab::CategoryColor> entries;
    entries.push_back(color);
    color.setSubcategories(entries);
    entries.push_back(color);
    color.setSubcategories(entries);
    entries.push_back(color);
    
    Kolab::Configuration configuration(entries);
    configuration.setUid("uid");
    configuration.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    configuration.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC

    const std::string &result = Kolab::writeConfiguration(configuration);
    QCOMPARE(Kolab::error(), Kolab::NoError);
//     std::cout << result << std::endl;
    const Kolab::Configuration &re = Kolab::readConfiguration(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    QCOMPARE(re.uid(), configuration.uid());
    QCOMPARE(re.created(), configuration.created());
    QCOMPARE(re.lastModified(), configuration.lastModified());
    QCOMPARE(re.type(), Kolab::Configuration::TypeCategoryColor);
    QCOMPARE(re.categoryColor(), entries);
}


void BindingsTest::dictionaryConfigurationCompletness()
{
    Kolab::Dictionary dict("en");
    std::vector<std::string> entries;
    entries.push_back("entry1");
    entries.push_back("entry2");
    entries.push_back("entry3");
    dict.setEntries(entries);

    Kolab::Configuration configuration(dict);
    configuration.setUid("uid");
    configuration.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    configuration.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC

    const std::string &result = Kolab::writeConfiguration(configuration);
    QCOMPARE(Kolab::error(), Kolab::NoError);
//     std::cout << result << std::endl;
    const Kolab::Configuration &re = Kolab::readConfiguration(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    QCOMPARE(re.uid(), configuration.uid());
    QCOMPARE(re.created(), configuration.created());
    QCOMPARE(re.lastModified(), configuration.lastModified());
    QCOMPARE(re.type(), Kolab::Configuration::TypeDictionary);
    QCOMPARE(re.dictionary(), dict);
}

void BindingsTest::snippetConfigurationCompletness()
{
    std::vector<Kolab::Snippet> list;
    Kolab::Snippet snippet1("name1", "text1");
    snippet1.setShortCut("shrtct1");
    list.push_back(snippet1);
    Kolab::Snippet snippet2("name1", "text1");
    snippet2.setTextType(Kolab::Snippet::HTML);
    list.push_back(snippet2);

    Kolab::SnippetsCollection snippets("collectionname");
    snippets.setSnippets(list);

    Kolab::Configuration configuration(snippets);
    configuration.setUid("uid");
    configuration.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    configuration.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC

    const std::string &result = Kolab::writeConfiguration(configuration);
    QCOMPARE(Kolab::error(), Kolab::NoError);
//     std::cout << result << std::endl;
    const Kolab::Configuration &re = Kolab::readConfiguration(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    QCOMPARE(re.uid(), configuration.uid());
    QCOMPARE(re.created(), configuration.created());
    QCOMPARE(re.lastModified(), configuration.lastModified());
    QCOMPARE(re.type(), Kolab::Configuration::TypeSnippet);
    QCOMPARE(re.snippets(), snippets);
}

void BindingsTest::relationConfigurationCompletness()
{
    Kolab::Relation relation("name1", "type1");
    relation.setColor("color");
    relation.setIconName("icon");
    relation.setParent("parent");
    relation.setPriority(3);
    std::vector<std::string> members;
    members.push_back("member1");
    members.push_back("member2");
    relation.setMembers(members);

    Kolab::Configuration configuration(relation);
    configuration.setUid("uid");
    configuration.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    configuration.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC

    const std::string &result = Kolab::writeConfiguration(configuration);
    QCOMPARE(Kolab::error(), Kolab::NoError);
//     std::cout << result << std::endl;
    const Kolab::Configuration &re = Kolab::readConfiguration(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    QCOMPARE(re.uid(), configuration.uid());
    QCOMPARE(re.created(), configuration.created());
    QCOMPARE(re.lastModified(), configuration.lastModified());
    QCOMPARE(re.type(), Kolab::Configuration::TypeRelation);
    QCOMPARE(re.relation(), relation);
}

void BindingsTest::fileDriverConfigurationCompletness()
{
    Kolab::FileDriver fileDriver("driver", "title");
    fileDriver.setEnabled(false);
    fileDriver.setHost("host");
    fileDriver.setPort(9);
    fileDriver.setUsername("username");
    fileDriver.setPassword("password");

    Kolab::Configuration configuration(fileDriver);
    configuration.setUid("uid");
    configuration.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    configuration.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC

    const std::string &result = Kolab::writeConfiguration(configuration);
    QCOMPARE(Kolab::error(), Kolab::NoError);
//     std::cout << result << std::endl;
    const Kolab::Configuration &re = Kolab::readConfiguration(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    QCOMPARE(re.uid(), configuration.uid());
    QCOMPARE(re.created(), configuration.created());
    QCOMPARE(re.lastModified(), configuration.lastModified());
    QCOMPARE(re.type(), Kolab::Configuration::TypeFileDriver);
    QCOMPARE(re.fileDriver(), fileDriver);
}

void BindingsTest::noteCompletness()
{
    Kolab::Note note;
    note.setUid("UID");
    note.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    note.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    note.setClassification(Kolab::ClassConfidential);
    note.addCategory("Category");
    note.setSummary("summary");
    note.setDescription("description");
    note.setColor("color");
    
    std::vector<Kolab::Attachment> attachments;
    Kolab::Attachment attachment;
    attachment.setData("data", "mimetype");
    attachment.setLabel("label");
    attachments.push_back(attachment);
    Kolab::Attachment attachment2;
    attachment2.setUri("data", "mimetype");
    attachment2.setLabel("label2");
    attachments.push_back(attachment2);
    note.setAttachments(attachments);
    
    const std::string &result = Kolab::writeNote(note);
    QCOMPARE(Kolab::error(), Kolab::NoError);
//     std::cout << result << std::endl;
    
    const Kolab::Note &re = Kolab::readNote(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    QCOMPARE(re.uid(), note.uid());
    QCOMPARE(re.created(), note.created());
    QCOMPARE(re.lastModified(), note.lastModified());
    QCOMPARE(re.classification(), note.classification());
    QCOMPARE(re.categories(), note.categories());
    QCOMPARE(re.summary(), note.summary());
    QCOMPARE(re.description(), note.description());
    QCOMPARE(re.color(), note.color());
    QCOMPARE(re.attachments(), note.attachments());
}

void BindingsTest::fileCompletness()
{
    Kolab::File file;
    file.setUid("UID");
    file.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    file.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    file.setClassification(Kolab::ClassConfidential);
    file.addCategory("Category");
    file.setNote("summary");
    Kolab::Attachment attachment;
    attachment.setData("data", "mimetype");
    attachment.setLabel("label");
    file.setFile(attachment);
    
    const std::string &result = Kolab::writeFile(file);
    QCOMPARE(Kolab::error(), Kolab::NoError);
//     std::cout << result << std::endl;
    
    const Kolab::File &re = Kolab::readFile(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    QCOMPARE(re.uid(), file.uid());
    QCOMPARE(re.created(), file.created());
    QCOMPARE(re.lastModified(), file.lastModified());
    QCOMPARE(re.classification(), file.classification());
    QCOMPARE(re.categories(), file.categories());
    QCOMPARE(re.note(), file.note());
    QCOMPARE(re.file(), file.file());
}


// void BindingsTest::eventCompletness_data()
template <typename T>
void setIncidence(T &ev)
{
    ev.setUid("UID");
    ev.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    ev.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    ev.setSequence(1);
    ev.setClassification(Kolab::ClassConfidential);
    ev.addCategory("Category");
    ev.setStart(Kolab::cDateTime("Europe/Zurich", 2006,1,6,12,0,0));
    
    Kolab::RecurrenceRule rule;
    rule.setFrequency(Kolab::RecurrenceRule::Daily);
    rule.setCount(5);
    std::vector<int> list;
    list.push_back(1);
    list.push_back(3);
    rule.setBysecond(list);
    rule.setByminute(list);
    rule.setByhour(list);
    std::vector<Kolab::DayPos> byday;
    byday.push_back(Kolab::DayPos(15, Kolab::Friday));
    byday.push_back(Kolab::DayPos(0, Kolab::Monday));
    byday.push_back(Kolab::DayPos(-3, Kolab::Monday));
    rule.setByday(byday);
    rule.setBymonthday(list);
    rule.setByyearday(list);
    rule.setByweekno(list);
    rule.setBymonth(list);
    
    ev.setRecurrenceRule(rule);
    ev.addRecurrenceDate(Kolab::cDateTime("Europe/Zurich", 2006,1,6,12,0,0));
    ev.addExceptionDate(Kolab::cDateTime("Europe/Zurich", 2006,1,6,12,0,0));
    ev.setRecurrenceID(Kolab::cDateTime("Europe/Zurich", 2006,1,6,12,0,0), true);
    ev.setSummary("summary");
    ev.setDescription("description");
    ev.setComment("comment");
    ev.setPriority(3);
    ev.setStatus(Kolab::StatusConfirmed);
    ev.setLocation("location");
    ev.setOrganizer(Kolab::ContactReference("mail", "name", "uid"));
    ev.setUrl("http://example.com");
    
    Kolab::Attendee attendee(Kolab::ContactReference("mail", "name", "uid"));
    attendee.setPartStat(Kolab::PartDelegated);
    attendee.setRole(Kolab::Chair);
    attendee.setRSVP(true);
    std::vector <Kolab::ContactReference > reflist;
    reflist.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail", "name"));
    reflist.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail2", "name2"));
    attendee.setDelegatedTo(reflist);
    attendee.setDelegatedFrom(reflist);
    attendee.setCutype(Kolab::CutypeResource);
    
    ev.setAttendees(std::vector<Kolab::Attendee>() << attendee << attendee);
    
    std::vector<Kolab::Attachment> attachments;
    
    Kolab::Attachment attach;
    attach.setData("data????*?*?*?*?*?", "mimetype");
    attach.setLabel("label");
    attachments.push_back(attach);
    
    Kolab::Attachment attach2;
    attach2.setUri(TEST_DATA_PATH "/testfiles/icalEvent.xml", "mimetype");
    attach2.setLabel("labe2l");
    attachments.push_back(attach2);
    
    Kolab::Attachment attach3;
    using namespace std;
    ifstream file (TEST_DATA_PATH "/testfiles/icalEvent.xml", ios::in|ios::binary|ios::ate);
    if (file.is_open()) {
        int size = file.tellg();
        char *memblock = new char [size];
        file.seekg (0, ios::beg);
        file.read (memblock, size);
        file.close();

        attach3.setData(string(memblock, size), "mimetype");

        delete[] memblock;
    } else {
        qWarning() << "Testfile not found";
    }
    attach3.setLabel("labe3l");
    attachments.push_back(attach3);
    
    ev.setAttachments(attachments);
    std::vector<Kolab::CustomProperty> properties;
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    ev.setCustomProperties(properties);
    
    std::vector<Kolab::Alarm> alarms;
//     Kolab::Alarm dispAlarm("ident");
//     dispAlarm.setRelativeStart(Kolab::Duration(3, true), Kolab::Start);
//     alarms.push_back(dispAlarm);
    std::vector<Kolab::ContactReference> att;
    att.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail", "name"));
    att.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail", "name"));
    Kolab::Alarm emailAlarm("ident", "value", att);
    emailAlarm.setStart(Kolab::cDateTime(2003,2,3,2,3,4, true));
    alarms.push_back(emailAlarm);
//     Kolab::Attachment audiofile;
//     audiofile.setUri("ksdjlksdflj", "sdkljdfl");
//     Kolab::Alarm audio(audiofile);
//     audio.setStart(Kolab::cDateTime(2003,2,3,2,3,4, true));
//     alarms.push_back(audio);
    ev.setAlarms(alarms);
}

template <typename T>
void checkIncidence(const T &ev, const T &re)
{
    //Common to all
    
    QCOMPARE(ev.uid(), re.uid());
    QCOMPARE(ev.created(), re.created());
    QCOMPARE(ev.lastModified(), re.lastModified());
    QCOMPARE(ev.sequence(), re.sequence());
    QCOMPARE(ev.classification(), re.classification());
    QCOMPARE(ev.categories(), re.categories());
    QCOMPARE(ev.start(), re.start());
    
    //Recurrence
    const Kolab::RecurrenceRule &r1 = ev.recurrenceRule();
    const Kolab::RecurrenceRule &r2 = re.recurrenceRule();
    QCOMPARE(r1.isValid(), r2.isValid());
    QCOMPARE(r1.frequency(), r2.frequency());
    QCOMPARE(r1.interval(), r2.interval());
    QCOMPARE(r1.weekStart(), r2.weekStart());
    QCOMPARE(r1.count(), r2.count());
    QCOMPARE(r1.end(), r2.end());
    QCOMPARE(r1.bysecond(), r2.bysecond());
    QCOMPARE(r1.byminute(), r2.byminute());
    QCOMPARE(r1.byhour(), r2.byhour());
    QCOMPARE(r1.byday(), r2.byday());
    QCOMPARE(r1.bymonthday(), r2.bymonthday());
    QCOMPARE(r1.byyearday(), r2.byyearday());
    QCOMPARE(r1.byweekno(), r2.byweekno());
    QCOMPARE(r1.bymonth(), r2.bymonth());
        
    //Rest
    QCOMPARE(ev.recurrenceDates(), re.recurrenceDates());
    QCOMPARE(ev.exceptionDates(), re.exceptionDates());
    QCOMPARE(ev.recurrenceID(), re.recurrenceID());
    QCOMPARE(ev.thisAndFuture(), re.thisAndFuture());
    QCOMPARE(ev.summary(), re.summary());
    QCOMPARE(ev.description(), re.description());
    QCOMPARE(ev.comment(), re.comment());
    QCOMPARE(ev.priority(), re.priority());
    QCOMPARE(ev.status(), re.status());
    QCOMPARE(ev.location(), re.location());
    QCOMPARE(ev.organizer(), re.organizer());
    QCOMPARE(ev.url(), re.url());
    QCOMPARE(ev.attendees(), re.attendees());
    QCOMPARE(ev.attachments(), re.attachments());
    QCOMPARE(ev.customProperties(), re.customProperties());
    QCOMPARE(ev.alarms(), re.alarms());
}



void BindingsTest::eventCompletness()
{
    Kolab::Event ev;
    setIncidence(ev);
    ev.setEnd(Kolab::cDateTime("Europe/Zurich", 2006,1,8,12,0,0));
    ev.setTransparency(true);

    std::string result = Kolab::writeEvent(ev);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::Event e = Kolab::readEvent(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    checkIncidence(ev, e);
    QCOMPARE(ev.end(), e.end());
    QCOMPARE(ev.transparency(), e.transparency());
    
}

void BindingsTest::eventDuration()
{
    Kolab::Event ev;
    ev.setStart(Kolab::cDateTime("Europe/Zurich", 2006,1,8,12,0,0));
    ev.setDuration(Kolab::Duration(11,22,33,44, true));

    const std::string result = Kolab::writeEvent(ev);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    const Kolab::Event e = Kolab::readEvent(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    QVERIFY(ev.duration().isValid());
    QCOMPARE(ev.duration(), e.duration());
}

void BindingsTest::eventExceptions()
{
    Kolab::Event ev;
    ev.setUid("uid1");
    ev.setStart(Kolab::cDateTime("Europe/Zurich", 2006,1,8,12,0,0));
    std::vector<Kolab::Event> exceptions;
    Kolab::Event ex1;
    ex1.setStart(Kolab::cDateTime("Europe/Zurich", 2006,1,8,12,0,0));
    ex1.setUid("uid1");
    ex1.setRecurrenceID(Kolab::cDateTime("Europe/Zurich", 2006,1,8,12,0,0), true);
    exceptions.push_back(ex1);
    Kolab::Event ex2;
    ex2.setStart(Kolab::cDateTime("Europe/Zurich", 2006,1,8,12,0,0));
    ex2.setUid("uid1");
    ex2.setRecurrenceID(Kolab::cDateTime("Europe/Zurich", 2007,1,8,12,0,0), false);
    exceptions.push_back(ex2);
    ev.setExceptions(exceptions);

    const std::string result = Kolab::writeEvent(ev);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    const Kolab::Event e = Kolab::readEvent(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    QCOMPARE(e.exceptions().size(), std::size_t(2));
    QCOMPARE(ev.exceptions().at(0).uid(), e.exceptions().at(0).uid());
    QCOMPARE(ev.exceptions().at(0).recurrenceID(), e.exceptions().at(0).recurrenceID());
    QCOMPARE(ev.exceptions().at(1).uid(), e.exceptions().at(1).uid());
    QCOMPARE(ev.exceptions().at(1).recurrenceID(), e.exceptions().at(1).recurrenceID());

}

void BindingsTest::todoCompletness()
{
    Kolab::Todo ev;
    setIncidence(ev);
    ev.setDue(Kolab::cDateTime("Europe/Zurich", 2006,1,8,12,0,0));
    ev.addRelatedTo("rel1");
    ev.addRelatedTo("rel2");
    ev.setPercentComplete(50);

    std::string result = Kolab::writeTodo(ev);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::Todo e = Kolab::readTodo(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    checkIncidence(ev, e);
    QCOMPARE(ev.due(), e.due());
    QCOMPARE(ev.relatedTo(), e.relatedTo());
    QCOMPARE(ev.percentComplete(), e.percentComplete());
}

void BindingsTest::dueDateDateOnly()
{
    Kolab::Todo ev;
    ev.setDue(Kolab::cDateTime(2006,1,8));
    std::string result = Kolab::writeTodo(ev);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::Todo e = Kolab::readTodo(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    QCOMPARE(ev.due(), e.due());
}

void BindingsTest::journalCompletness()
{
    Kolab::Journal ev;
    
    
    ev.setUid("UID");
    ev.setCreated(Kolab::cDateTime(2006,1,6,12,0,0,true)); //UTC
    ev.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0,true));
    ev.setSequence(1);
    ev.setClassification(Kolab::ClassConfidential);
    ev.addCategory("Category");
    ev.setStart(Kolab::cDateTime("Europe/Zurich", 2006,1,6,12,0,0));
    Kolab::Attendee attendee(Kolab::ContactReference("mail", "name", "uid"));
    attendee.setPartStat(Kolab::PartDelegated);
    attendee.setRole(Kolab::Chair);
    attendee.setRSVP(true);
    std::vector <Kolab::ContactReference > reflist;
    reflist.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail", "name"));
    reflist.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail2", "name2"));
    attendee.setDelegatedTo(reflist);
    attendee.setDelegatedFrom(reflist);
    attendee.setCutype(Kolab::CutypeResource);
    
    ev.setAttendees(std::vector<Kolab::Attendee>() << attendee << attendee);
    
    std::vector<Kolab::Attachment> attachments;
    
    Kolab::Attachment attach;
    attach.setData("data????*?*?*?*?*?", "mimetype");
    attach.setLabel("label");
    attachments.push_back(attach);
    
    Kolab::Attachment attach2;
    attach2.setUri(TEST_DATA_PATH "/testfiles/icalEvent.xml", "mimetype");
    attach2.setLabel("labe2l");
    attachments.push_back(attach2);
    
    Kolab::Attachment attach3;
    using namespace std;
    ifstream file (TEST_DATA_PATH "/testfiles/icalEvent.xml", ios::in|ios::binary|ios::ate);
    if (file.is_open()) {
        int size = file.tellg();
        char *memblock = new char [size];
        file.seekg (0, ios::beg);
        file.read (memblock, size);
        file.close();

        attach3.setData(string(memblock, size), "mimetype");

        delete[] memblock;
    } else {
        qWarning() << "Testfile not found";
    }
    attach3.setLabel("labe3l");
    attachments.push_back(attach3);
    
    ev.setAttachments(attachments);
    std::vector<Kolab::CustomProperty> properties;
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    ev.setCustomProperties(properties);
    
    
    
    std::string result = Kolab::writeJournal(ev);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::Journal re = Kolab::readJournal(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    
    
    QCOMPARE(ev.uid(), re.uid());
    QCOMPARE(ev.created(), re.created());
    QCOMPARE(ev.lastModified(), re.lastModified());
    QCOMPARE(ev.sequence(), re.sequence());
    QCOMPARE(ev.classification(), re.classification());
    QCOMPARE(ev.categories(), re.categories());
    QCOMPARE(ev.start(), re.start());
    QCOMPARE(ev.summary(), re.summary());
    QCOMPARE(ev.description(), re.description());
    QCOMPARE(ev.status(), re.status());
    QCOMPARE(ev.attendees(), re.attendees());
    QCOMPARE(ev.attachments(), re.attachments());
    QCOMPARE(ev.customProperties(), re.customProperties());
}

void BindingsTest::freebusyCompletness()
{
    Kolab::Freebusy ev;
    ev.setUid("UID");
    ev.setTimestamp(Kolab::cDateTime(2006,1,6,12,0,0, true));
    ev.setStart(Kolab::cDateTime(2006,1,6,12,0,0, true));
    ev.setEnd(Kolab::cDateTime(2006,1,6,12,0,0, true));
    ev.setOrganizer(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail", "name"));

    std::vector<Kolab::FreebusyPeriod> fbperiods;
    Kolab::FreebusyPeriod fbp;
    fbp.setType(Kolab::FreebusyPeriod::OutOfOffice);
    fbp.setEvent("summary", "uid", "location");
    std::vector<Kolab::Period> periods;
    periods.push_back(Kolab::Period(Kolab::cDateTime(2006,1,6,12,0,0, true),Kolab::cDateTime(2007,1,6,12,0,0, true)));
    periods.push_back(Kolab::Period(Kolab::cDateTime(2006,1,6,12,0,0, true),Kolab::cDateTime(2007,1,6,12,0,0, true)));
    fbp.setPeriods(periods);
    fbperiods.push_back(fbp);
    fbperiods.push_back(fbp);
    ev.setPeriods(fbperiods);


    Kolab::overrideTimestamp(ev.timestamp());
    std::string result = Kolab::writeFreebusy(ev);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::Freebusy re = Kolab::readFreebusy(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);


    QCOMPARE(ev.uid(), re.uid());
    QCOMPARE(ev.timestamp(), re.timestamp());
    QCOMPARE(ev.start(), re.start());
    QCOMPARE(ev.end(), re.end());
    QCOMPARE(ev.organizer(), re.organizer());
    QCOMPARE(ev.periods(), re.periods());
}

void BindingsTest::contactCompletness()
{
    std::vector<std::string> stringlist;
    stringlist.push_back("lbksdfbklsd");
    stringlist.push_back("sdf");

    Kolab::Contact c;
    c.setUid("1045b57d-ff7f-0000-d814-867b4d7f0000");
    c.setLastModified(Kolab::cDateTime(2006,1,6,12,0,0, true));
    c.setCategories(stringlist);
    c.setName("name");
    Kolab::NameComponents nc;
    nc.setSurnames(stringlist);
    nc.setGiven(stringlist);
    nc.setAdditional(stringlist);
    nc.setPrefixes(stringlist);
    nc.setSuffixes(stringlist);
    c.setNameComponents(nc);
    c.setNote("note");
    c.setFreeBusyUrl("freebusy");
    c.setUrls(std::vector<Kolab::Url>() << Kolab::Url("url1") << Kolab::Url("url1", Kolab::Url::Blog));
    c.setNickNames(stringlist);
    
    std::vector <Kolab::Related> relateds;
    Kolab::Related r1(Kolab::Related::Text, "text");
    r1.setRelationTypes(Kolab::Related::Child);
    relateds.push_back(r1);
    Kolab::Related r2(Kolab::Related::Uid, "urn");
    r2.setRelationTypes(Kolab::Related::Child|Kolab::Related::Spouse);
    relateds.push_back(r2);
    c.setRelateds(relateds);
    
    c.setBDay(Kolab::cDateTime(2001,12,10,12,12,12,false));
    c.setAnniversary(Kolab::cDateTime(2001,3,2,1,1,1,false));
    c.setPhoto("photo", "mimetype");
    c.setGender(Kolab::Contact::Male);
    c.setLanguages(stringlist);
    c.setIMaddresses(stringlist,1);
    std::vector <Kolab::Email> emails;
    emails.push_back(Kolab::Email("email1@example.org", Kolab::Email::NoType));
    emails.push_back(Kolab::Email("email2@example.org", Kolab::Email::Work));
    emails.push_back(Kolab::Email("email3@example.org", Kolab::Email::Work|Kolab::Email::Home));
    c.setEmailAddresses(emails,0);
    c.setTitles(stringlist);
    
    std::vector<Kolab::Affiliation> list;
    Kolab::Affiliation aff;
    aff.setOrganisation("org");
    aff.setOrganisationalUnits(stringlist);
    aff.setLogo("logo", "mime/miem");
    aff.setRoles(stringlist);
    std::vector<Kolab::Related> relateds2;
    relateds2.push_back(Kolab::Related(Kolab::Related::Text, "textRelation", Kolab::Related::Assistant));
    relateds2.push_back(Kolab::Related(Kolab::Related::Uid, "textRelation", Kolab::Related::Manager));
    aff.setRelateds(relateds2);
    std::vector<Kolab::Address> adrs;
    Kolab::Address adr1;
    adr1.setLabel("label");
    adrs.push_back(adr1);
    Kolab::Address adr2;
    adr2.setStreet("street");
    adr2.setTypes(Kolab::Address::Work);
    adrs.push_back(adr2);
    aff.setAddresses(adrs);
    list.push_back(aff);
    Kolab::Affiliation aff2;
    aff2.setOrganisation("org2");
    list.push_back(aff2);
    c.setAffiliations(list);
    Kolab::Address address;
    address.setCode("oiuoiu");
    address.setCountry("werwer");
    address.setLabel("lkjlkj");
    address.setLocality("alla");
    address.setRegion("skjdfkd");
    address.setStreet("sldkflsdfj");
    address.setTypes( Kolab::Address::Work | Kolab::Address::Home );
    std::vector<Kolab::Address> addresses;
    addresses.push_back(address);
    addresses.push_back(address);
    c.setAddresses(addresses);
    Kolab::Telephone phone;
    phone.setNumber("lkjsdflkjfds");
    phone.setTypes(Kolab::Telephone::Work|
                    Kolab::Telephone::Home|
                    Kolab::Telephone::Text|
                    Kolab::Telephone::Voice|
                    Kolab::Telephone::Fax|
                    Kolab::Telephone::Cell|
                    Kolab::Telephone::Video|
                    Kolab::Telephone::Pager|
                    Kolab::Telephone::Textphone|
                    Kolab::Telephone::Car
    );

    std::vector<Kolab::Telephone> telephones;
    telephones.push_back(phone);
    telephones.push_back(phone);
    c.setTelephones(telephones, 1);
    std::vector<Kolab::Geo> geo;
    geo << Kolab::Geo(1.3, -40.3);
    c.setGPSpos(geo);
    Kolab::Crypto crypto;
    crypto.setAllowed(Kolab::Crypto::PGPinline | Kolab::Crypto::SMIMEopaque);
    crypto.setSignPref(Kolab::Crypto::IfPossible);
    crypto.setEncryptPref(Kolab::Crypto::Never);
    c.setCrypto(crypto);
    std::vector<Kolab::Key> keys;
    keys.push_back(Kolab::Key("pgp", Kolab::Key::PGP));
    keys.push_back(Kolab::Key("smime", Kolab::Key::PKCS7_MIME));
    c.setKeys(keys);
    
    std::vector<Kolab::CustomProperty> properties;
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    c.setCustomProperties(properties);

    const std::string result = Kolab::writeContact(c);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::Contact e = Kolab::readContact(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    QCOMPARE(e.uid(), c.uid());
    QCOMPARE(e.lastModified(), c.lastModified());
    QCOMPARE(e.categories(), c.categories());
    QCOMPARE(e.name(), c.name());
    QCOMPARE(e.nameComponents(), c.nameComponents());
    QCOMPARE(e.note(), c.note());
    QCOMPARE(e.freeBusyUrl(), c.freeBusyUrl());
    QCOMPARE(e.titles(), c.titles());
    QCOMPARE(e.affiliations(), c.affiliations());
    QCOMPARE(e.urls(), c.urls());
    QCOMPARE(e.addresses(), c.addresses());
    QCOMPARE(e.addressPreferredIndex(), c.addressPreferredIndex());
    QCOMPARE(e.nickNames(), c.nickNames());
    QCOMPARE(e.relateds(), c.relateds());
    QCOMPARE(e.bDay(), c.bDay());
    QCOMPARE(e.anniversary(), c.anniversary());
    QCOMPARE(e.photo(), c.photo());
    QCOMPARE(e.photoMimetype(), c.photoMimetype());
    QCOMPARE(e.gender(), c.gender());
    QCOMPARE(e.languages(), c.languages());
    QCOMPARE(e.telephones(), c.telephones());
    QCOMPARE(e.telephonesPreferredIndex(), c.telephonesPreferredIndex());
    QCOMPARE(e.imAddresses(), c.imAddresses());
    QCOMPARE(e.imAddressPreferredIndex(), c.imAddressPreferredIndex());
    QCOMPARE(e.emailAddresses(), c.emailAddresses());
    QCOMPARE(e.emailAddressPreferredIndex(), c.emailAddressPreferredIndex());
    QCOMPARE(e.emailAddressPreferredIndex(), 0);
    QCOMPARE(e.gpsPos(), c.gpsPos());
    QCOMPARE(e.keys(), c.keys());
    QCOMPARE(e.crypto(), c.crypto());
    QCOMPARE(e.customProperties(), c.customProperties());
}

void BindingsTest::dateOnlyDates()
{
    Kolab::Contact c;
    c.setUid("1045b57d-ff7f-0000-d814-867b4d7f0000");
    c.setName("name");
    c.setBDay(Kolab::cDateTime(2001,12,10));
    c.setAnniversary(Kolab::cDateTime(2001,3,2));

    const std::string result = Kolab::writeContact(c);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::Contact e = Kolab::readContact(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    QCOMPARE(e.bDay(), c.bDay());
    QCOMPARE(e.anniversary(), c.anniversary());
}

void BindingsTest::distlistCompletness()
{
    std::vector<std::string> stringlist;
    stringlist.push_back("lbksdfbklsd");
    stringlist.push_back("sdf");

    Kolab::DistList c;
    c.setName("name");
    c.setUid("uid");
    std::vector<Kolab::ContactReference> members;
    members.push_back(Kolab::ContactReference(Kolab::ContactReference::EmailReference, "mail", "name"));
    members.push_back(Kolab::ContactReference(Kolab::ContactReference::UidReference, "urn"));
    c.setMembers(members);
    std::vector<Kolab::CustomProperty> properties;
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    properties.push_back(Kolab::CustomProperty("ident", "value"));
    c.setCustomProperties(properties);
    
    const std::string result = Kolab::writeDistlist(c);
    QVERIFY(Kolab::error() == Kolab::NoError);
//     std::cout << result << endl;
    Kolab::DistList e = Kolab::readDistlist(result, false);
    QVERIFY(Kolab::error() == Kolab::NoError);
    QCOMPARE(e.uid(), c.uid());
    QCOMPARE(e.name(), c.name());
    QCOMPARE(e.members(), c.members());
    QCOMPARE(e.customProperties(), c.customProperties());
}

void BindingsTest::generateTimestampIfEmpty()
{
    //xcal
    Kolab::Event ev;
    setIncidence(ev);
    ev.setLastModified(Kolab::cDateTime());

    std::string result = Kolab::writeEvent(ev);
    Kolab::Event e = Kolab::readEvent(result, false);
    QVERIFY(e.lastModified().isValid());
    QVERIFY(e.lastModified().isUTC());

    //xcard
    Kolab::Contact c;
    c.setUid("1045b57d-ff7f-0000-d814-867b4d7f0000");

    const std::string contactResult = Kolab::writeContact(c);
    Kolab::Contact retContact = Kolab::readContact(contactResult, false);
    QVERIFY(retContact.lastModified().isValid());
    QVERIFY(retContact.lastModified().isUTC());
}

void BindingsTest::versionTest()
{
    Kolab::Todo ev;
    setIncidence(ev);
    
    std::string result = Kolab::writeTodo(ev);
    Kolab::Todo e = Kolab::readTodo(result, false);
    QCOMPARE(Kolab::productId(), std::string(KOLAB_LIB_VERSION_STRING));
    QCOMPARE(Kolab::xKolabVersion(), std::string(KOLAB_FORMAT_VERSION));
    QCOMPARE(Kolab::xCalVersion(), std::string("2.0"));
}

void BindingsTest::errorTest()
{
    Kolab::Todo e = Kolab::readTodo("klbsdfbklsdbkl", false);
    QCOMPARE(Kolab::error(), Kolab::Critical);
    QVERIFY(!Kolab::errorMessage().empty());
}

//Don't break due to an error
void BindingsTest::errorRecoveryTest()
{
    Kolab::Todo e = Kolab::readTodo("klbsdfbklsdbkl", false);
    QCOMPARE(Kolab::error(), Kolab::Critical);

    Kolab::Todo ev;
    setIncidence(ev);
    
    const std::string result = Kolab::writeTodo(ev);
    Kolab::readTodo(result, false);
    QCOMPARE(Kolab::error(), Kolab::NoError);
}

void BindingsTest::BenchmarkRoundtripKolab()
{
    const Kolab::Event &event = Kolab::readEvent(TEST_DATA_PATH "/testfiles/icalEvent.xml", true);
    QVERIFY(!Kolab::errorOccurred());
    std::string result = Kolab::writeEvent(event);
    QBENCHMARK {
        Kolab::readEvent(result, false);
    }
}

void BindingsTest::BenchmarkRoundtrip()
{
    const Kolab::Event &event = Kolab::readEvent(TEST_DATA_PATH "/testfiles/icalEvent.xml", true);
    QVERIFY(!Kolab::errorOccurred());
    std::string result;
    QBENCHMARK {
        result = Kolab::writeEvent(event);
        Kolab::readEvent(result, false);
    }
}

void BindingsTest::preserveLatin1()
{
    Kolab::Event event;
    event.setSummary("äöü%@$£é¤¼²°");
    std::string result = Kolab::writeEvent(event);
//    std::cout << result << std::endl;
    Kolab::Event readEvent = Kolab::readEvent(result, false);
//     std::cout << readEvent.summary() << std::endl;
    QCOMPARE(readEvent.summary(), event.summary());
}

void BindingsTest::preserveUnicode()
{
    Kolab::Event event;
    event.setSummary("€Š�ـأبـ");
    std::string result = Kolab::writeEvent(event);
//    std::cout << result << std::endl;
    Kolab::Event readEvent = Kolab::readEvent(result, false);
//     std::cout << readEvent.summary() << std::endl;
    QCOMPARE(readEvent.summary(), event.summary());
}



QTEST_MAIN( BindingsTest )

#include "bindingstest.moc"
