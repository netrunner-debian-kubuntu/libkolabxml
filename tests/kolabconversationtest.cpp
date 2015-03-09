/*
    Copyright (C) 2014 Sandro Knauß <knauss@kolabsys.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "kolabconversationtest.h"

#include <QTest>
#include <QDebug>
#include <iostream>

#include "src/kolabconversions.h"

void KolabConversationTest::configurationDeserialationTest()
{
    boost::shared_ptr <Kolab::Configuration> ptr = Kolab::KolabObjects::deserializeObject<Kolab::Configuration>(TEST_DATA_PATH "/testfiles/testConfiguration.xml", true);
    QVERIFY(ptr);
    QVERIFY(ptr->isValid());
    QCOMPARE(ptr->type(), Kolab::Configuration::TypeRelation);

}

void KolabConversationTest::noteDeserialationTest()
{
    boost::shared_ptr <Kolab::Note> ptr = Kolab::KolabObjects::deserializeObject<Kolab::Note>(TEST_DATA_PATH "/testfiles/testNote.xml", true);
    QVERIFY(ptr);
    QVERIFY(ptr->isValid());
    QCOMPARE(ptr->summary(), std::string("summarytext"));
}


QTEST_MAIN( KolabConversationTest )

#include "kolabconversationtest.moc"
