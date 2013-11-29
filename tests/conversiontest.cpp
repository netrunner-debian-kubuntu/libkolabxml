#include "conversiontest.h"

#include <QtTest/QtTest>
#include <src/xcalconversions.h>
#include <src/xcardconversions.h>
#include <src/utils.h>

#include "serializers.h"
#include <boost/thread.hpp>
#include <curl/curlver.h>

Q_DECLARE_METATYPE(Kolab::Duration);
Q_DECLARE_METATYPE(Kolab::DayPos);
Q_DECLARE_METATYPE(Kolab::cDateTime);
Q_DECLARE_METATYPE(Kolab::ErrorSeverity);
Q_DECLARE_METATYPE(std::string);

using namespace Kolab::XCAL;

void ConversionTest::dateComparatorTest()
{
    QVERIFY( !(Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, false) == Kolab::cDateTime()));
    QVERIFY( Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, false) == Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, false));
}

 
void ConversionTest::durationParserTest_data()
{
    QTest::addColumn<Kolab::Duration>("expected");
    QTest::addColumn<QString>("string");

    QTest::newRow("Time") << Kolab::Duration(0,2,3,4, false) << "+PT2H3M4S";
    QTest::newRow("Day") << Kolab::Duration(1,2,3,4, false) << "+P1DT2H3M4S";
    QTest::newRow("Week") << Kolab::Duration(1, false) << "+P1W";
    QTest::newRow("Week Multidigit, negative") << Kolab::Duration(23, true) << "-P23W";
    Kolab::Utils::clearErrors();
}

void ConversionTest::durationParserTest()
{
    QFETCH(QString, string);
    QFETCH(Kolab::Duration, expected);
    const Kolab::Duration result = toDuration(string.toStdString());
    QCOMPARE(result, expected);
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}

void ConversionTest::durationSerializerTest_data()
{
    QTest::addColumn<QString>("expected");
    QTest::addColumn<Kolab::Duration>("duration");

    QTest::newRow("Time") << "PT2H3M4S" << Kolab::Duration(0,2,3,4, false);
    QTest::newRow("Day") << "P1DT2H3M4S" << Kolab::Duration(1,2,3,4, false);
    QTest::newRow("Week positive") << "P1W" << Kolab::Duration(1, false);
    QTest::newRow("Week negative") << "-P3W" << Kolab::Duration(3, true);
    QTest::newRow("Week Multidigit, negative") << "-P23W" << Kolab::Duration(23, true);
    Kolab::Utils::clearErrors();
}

void ConversionTest::durationSerializerTest()
{
    QFETCH(Kolab::Duration, duration);
    QFETCH(QString, expected);
    const std::string result = fromDuration(duration);
    QCOMPARE(QString::fromStdString(result), expected);
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}


void ConversionTest::dayPosParserTest_data()
{
    QTest::addColumn<Kolab::DayPos>("expected");
    QTest::addColumn<QString>("string");

    QTest::newRow("positive") << Kolab::DayPos(15, Kolab::Wednesday) << "15WE";
    QTest::newRow("positive with +") << Kolab::DayPos(15, Kolab::Wednesday) << "+15WE";
    QTest::newRow("negative") << Kolab::DayPos(-15, Kolab::Wednesday) << "-15WE";
    QTest::newRow("all occurrences") << Kolab::DayPos(0, Kolab::Wednesday) << "WE";
    Kolab::Utils::clearErrors();
}

void ConversionTest::dayPosParserTest()
{
    QFETCH(QString, string);
    QFETCH(Kolab::DayPos, expected);
    const Kolab::DayPos result = toDayPos(string.toStdString());
    QCOMPARE(result, expected);
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}

void ConversionTest::dayPosSerializerTest_data()
{
    QTest::addColumn<QString>("expected");
    QTest::addColumn<Kolab::DayPos>("daypos");

    QTest::newRow("pos") << "15WE" << Kolab::DayPos(15, Kolab::Wednesday);
    QTest::newRow("negative") << "-15WE" << Kolab::DayPos(-15, Kolab::Wednesday);
    QTest::newRow("all occurrences") << "WE" << Kolab::DayPos(0, Kolab::Wednesday);
    Kolab::Utils::clearErrors();

}

void ConversionTest::dayPosSerializerTest()
{
    QFETCH(Kolab::DayPos, daypos);
    QFETCH(QString, expected);
    const std::string result = fromDayPos(daypos);
    QCOMPARE(QString::fromStdString(result), expected);
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}

void ConversionTest::xcardDateParserTest_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<Kolab::cDateTime>("expected");
    QTest::addColumn<Kolab::ErrorSeverity>("errorState");

    QTest::newRow("datetime local") << "20010102T030405" << Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, false) << Kolab::NoError;
    QTest::newRow("datetime utc") << "20010102T030405Z" << Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, true) << Kolab::NoError;
    QTest::newRow("date only") << "20010102" << Kolab::cDateTime(2001, 01, 02) << Kolab::NoError;
    QTest::newRow("fail 1 short") << "20010102T03040" << Kolab::cDateTime() << Kolab::Error;
    QTest::newRow("fail non utc") << "20010102T030401s" << Kolab::cDateTime() << Kolab::Error;
    QTest::newRow("fail no time") << "20010102T" << Kolab::cDateTime() << Kolab::Error;
    Kolab::Utils::clearErrors();
}


void ConversionTest::xcardDateParserTest()
{
    QFETCH(QString, input);
    QFETCH(Kolab::cDateTime, expected);
    QFETCH(Kolab::ErrorSeverity, errorState);
    const Kolab::cDateTime &dt = Kolab::XCARD::toDateTime(input.toStdString());
    QCOMPARE(dt, expected);
    QCOMPARE(Kolab::Utils::getError(), errorState);
}

void ConversionTest::xCardSerializerTest_data()
{
    QTest::addColumn<QString>("expected");
    QTest::addColumn<Kolab::cDateTime>("dt");

    QTest::newRow("datetime local") << "20010102T030405" << Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, false);
    QTest::newRow("datetime utc") << "20010102T030405Z" << Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, true);
    QTest::newRow("dateonly") << "20010102" << Kolab::cDateTime(2001, 01, 02);
    Kolab::Utils::clearErrors();
}

void ConversionTest::xCardSerializerTest()
{
    QFETCH(QString, expected);
    QFETCH(Kolab::cDateTime, dt);
    const std::string &s = Kolab::XCARD::fromDateTime(dt);
    QCOMPARE(QString::fromStdString(s), expected);
    if (dt.isDateOnly()) {
        const std::string &s2 = Kolab::XCARD::fromDate(dt);
        QCOMPARE(QString::fromStdString(s2), expected);
    }
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}

void ConversionTest::uriInlineEncodingTest()
{
    const std::string &s = Kolab::Utils::uriInlineEncoding("data", "mimetype/mime");
    QCOMPARE(s, std::string("data:mimetype/mime;base64,ZGF0YQ=="));
    std::string mimetype;
    const std::string &d = Kolab::Utils::uriInlineDecoding(s, mimetype);
    QCOMPARE(mimetype, std::string("mimetype/mime"));
    QCOMPARE(d, std::string("data"));
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}

void ConversionTest::mailtoUriEncodingTest_data()
{
    QTest::addColumn<QString>("email");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("resultEncoded");
    //Older versions used to encode "." and "_". Not sure which version is actually required though (fixed in 7.26.0 but broken in 7.19.7, we're testing for 7.26.0)
    if (LIBCURL_VERSION_NUM >= 0x071a00) {
        QTest::newRow("1") << "email_1@email.com" << "John Doe" << "mailto:John%20Doe%3Cemail_1%40email.com%3E";
        QTest::newRow("Reserved characters") << "!*'();:@&=+$,/?#[]@email.com" << "John Doe" << "mailto:John%20Doe%3C%21%2A%27%28%29%3B%3A%40%26%3D%2B%24%2C%2F%3F%23%5B%5D%40email.com%3E";
        QTest::newRow("Unreserved characters") << "Aa0-_.~@email.com" << "John Doe" << "mailto:John%20Doe%3CAa0-_.~%40email.com%3E";
    } else {
        QTest::newRow("1") << "email_1@email.com" << "John Doe" << "mailto:John%20Doe%3Cemail%5F1%40email%2Ecom%3E";
        QTest::newRow("Reserved characters") << "!*'();:@&=+$,/?#[]@email.com" << "John Doe" << "mailto:John%20Doe%3C%21%2A%27%28%29%3B%3A%40%26%3D%2B%24%2C%2F%3F%23%5B%5D%40email%2Ecom%3E";
        QTest::newRow("Unreserved characters") << "Aa0-_.~@email.com" << "John Doe" << "mailto:John%20Doe%3CAa0%2D%5F%2E%7E%40email%2Ecom%3E";
    }
    
    Kolab::Utils::clearErrors();
}

void ConversionTest::mailtoUriEncodingTest()
{
    QFETCH(QString, email);
    QFETCH(QString, name);
    QFETCH(QString, resultEncoded);
    const std::string &mailto = Kolab::XCARD::toMailto(email.toStdString(), name.toStdString());
//     std::cout << mailto << std::endl;
    QCOMPARE(QString::fromStdString(mailto), resultEncoded);
    std::string n;
    const std::string &e = Kolab::XCARD::fromMailto(resultEncoded.toStdString(), n);
    QCOMPARE(QString::fromStdString(n), name);
    QCOMPARE(QString::fromStdString(e), email);
    
}

void ConversionTest::mailtoUriDecodingTest()
{
    QString name("John Doe");
    QString email("email_1@email.com");
    //While we shouldn't encode '_' and '.' according to RFC 3986, we should still understand it for backwards compatiblity
    const std::string resultEncoded = "mailto:John%20Doe%3Cemail%5F1%40email%2Ecom%3E";
    std::string n;
    const std::string &e = Kolab::XCARD::fromMailto(resultEncoded, n);
    QCOMPARE(QString::fromStdString(n), name);
    QCOMPARE(QString::fromStdString(e), email);
}

void ConversionTest::urnTest()
{
    QCOMPARE(Kolab::Shared::toURN("1045b57d-ff7f-0000-d814-867b4d7f0000"), std::string("urn:uuid:1045b57d-ff7f-0000-d814-867b4d7f0000"));
    QCOMPARE(Kolab::Shared::toURN("urn:uuid:1045b57d-ff7f-0000-d814-867b4d7f0000"), std::string("urn:uuid:1045b57d-ff7f-0000-d814-867b4d7f0000"));
    QCOMPARE(Kolab::Shared::fromURN("urn:uuid:1045b57d-ff7f-0000-d814-867b4d7f0000"), std::string("1045b57d-ff7f-0000-d814-867b4d7f0000"));
    QCOMPARE(Kolab::Shared::fromURN("1045b57d-ff7f-0000-d814-867b4d7f0000"), std::string("1045b57d-ff7f-0000-d814-867b4d7f0000"));
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}

void ConversionTest::geoUriTest()
{
    QCOMPARE(Kolab::XCARD::toGeoUri(-34.056, 179.3453), std::string("geo:-34.056,179.3453"));
    QCOMPARE(Kolab::XCARD::toGeoUri(-34.1, 179.5), std::string("geo:-34.1,179.5"));
    QCOMPARE(Kolab::XCARD::toGeoUri(-34.0, 179.0), std::string("geo:-34,179"));
    QCOMPARE(Kolab::XCARD::toGeoUri(-34, 179), std::string("geo:-34,179"));
    QCOMPARE(Kolab::XCARD::toGeoUri(-34.012342356, 179.3451234553), std::string("geo:-34.012342356,179.3451234553"));
    
    double lat, lon;
    QVERIFY(Kolab::XCARD::fromGeoUri(std::string("geo:-34.056,179.3453"), lat, lon));
    QCOMPARE(lat, -34.056);
    QCOMPARE(lon, 179.3453);
    QCOMPARE(Kolab::Utils::getError(), Kolab::NoError);
}


void ConversionTest::contactReferenceTest()
{
    Kolab::ContactReference email(Kolab::ContactReference::EmailReference, "mail", "name");
    QCOMPARE(Kolab::Shared::toMailto(email.email(), email.name()), std::string("mailto:name%3Cmail%3E"));
    Kolab::ContactReference urn(Kolab::ContactReference::UidReference, "urn");
    QCOMPARE(Kolab::Shared::toURN(urn.uid()), std::string("urn:uuid:urn"));
    
    QCOMPARE(Kolab::Shared::toContactReference("urn:uuid:urn"), urn);
    QCOMPARE(Kolab::Shared::toContactReference("mailto:name%3Cmail%3E"), email);
}


void threadF() 
{
    for (int i = 0; i < 5; ++i)
    {
        std::stringstream s;
//         s << boost::this_thread::get_id();
        std::string uid = s.str();
//         std::cout << uid << std::endl;
        Kolab::Utils::setCreatedUid(uid);
        boost::this_thread::sleep(boost::posix_time::millisec(50));
        QCOMPARE(Kolab::Utils::createdUid(), uid);
    }
}

void ConversionTest::threadLocalTest()
{
    //Ensure global variables are not mixed between threads and therefore threadsafe.
    boost::thread t(threadF);
    boost::thread t2(threadF);
    t.join();
    t2.join();
}

void ConversionTest::uuidGeneratorTest()
{
    const std::string &s = getUID();
    std::cout << s << std::endl;
    QVERIFY(!s.empty());
    QCOMPARE(s.size(), (std::size_t)36ul);
}

void ConversionTest::uuidGeneratorTest2()
{
    Kolab::Event event;
    event.setStart(Kolab::cDateTime(2001, 01, 02, 03, 04 ,05, false));
    QVERIFY(Kolab::getSerializedUID().empty());
    Kolab::writeEvent(event);
    std::string uid1 = Kolab::getSerializedUID();
    QVERIFY(!uid1.empty());
    Kolab::writeEvent(event);
    std::string uid2 = Kolab::getSerializedUID();
    std::cout << uid1 << uid2;
    QVERIFY(!uid2.empty());
    QVERIFY(uid1 != uid2);
}




QTEST_MAIN( ConversionTest )

#include "conversiontest.moc"


