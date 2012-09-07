/*
    Copyright (C) 2012 Christian Mollekopf <mollekopf@kolabsys.com>

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

#include "parsingtest.h"

#include <QTest>
#include <iostream>

#include "src/kolabformat.h"

void ParsingTest::vcardParsingTest()
{
    const Kolab::Contact &contact = Kolab::readContact(TEST_DATA_PATH "/testfiles/vcard/contact.xml", true);
    QCOMPARE(Kolab::error(), Kolab::NoError);
    std::cout << Kolab::writeContact(contact);
}


QTEST_MAIN( ParsingTest )

#include "parsingtest.moc"
