/*
    Copyright (C) 2013 Christian Mollekopf <mollekopf@kolabsys.com>

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

#include "validationtest.h"

#include <QTest>

#include "src/kolabformat.h"

using namespace Kolab;

void ValidationTest::testErrorOnEmptyEvent()
{
    Event event;
    writeEvent(event);
    QCOMPARE(Kolab::error(), Kolab::Error);
}

void ValidationTest::testNoErrorOnValidEvent()
{
    Event event;
    event.setStart(cDateTime(2013,1,1,1,1,1));
    writeEvent(event);
    QCOMPARE(Kolab::error(), Kolab::NoError);
}

void ValidationTest::testNoErrorOnValidTodo()
{
    {
        Todo todo;
        todo.setStart(cDateTime("Europe/Zurich", 2013,1,1,1,1,1));
        writeTodo(todo);
        QCOMPARE(Kolab::error(), Kolab::NoError);
    }
    {
        Todo todo;
        todo.setDue(cDateTime("Europe/Zurich", 2013,1,1,1,1,1));
        writeTodo(todo);
        QCOMPARE(Kolab::error(), Kolab::NoError);
    }
    {
        Todo todo;
        todo.setStart(cDateTime("Europe/Zurich", 2013,1,1,1,1,1));
        todo.setDue(cDateTime("Europe/Zurich", 2013,1,1,1,1,1));
        writeTodo(todo);
        QCOMPARE(Kolab::error(), Kolab::NoError);
    }
}

void ValidationTest::testOlsonTimezone()
{
    Event event;
    event.setStart(cDateTime("Europe/Zurich",2013,1,1,1,1,1));
    writeEvent(event);
    QCOMPARE(Kolab::error(), Kolab::NoError);
}

void ValidationTest::testDifferentTimezones()
{
    Event event;
    event.setStart(cDateTime("Europe/Zurich",2013,1,1,1,1,1));
    event.setEnd(cDateTime("Europe/London",2013,1,1,1,1,1));
    writeEvent(event);
    QCOMPARE(Kolab::error(), Kolab::NoError);

    Todo todo;
    todo.setStart(cDateTime("Europe/Zurich",2013,1,1,1,1,1));
    todo.setDue(cDateTime("Europe/London",2013,1,1,1,1,1));
    writeTodo(todo);
    QCOMPARE(Kolab::error(), Kolab::NoError);
}

void ValidationTest::testUTCwithTimezone()
{
    Event event;
    cDateTime dt("Europe/Zurich",2013,1,1,1,1,1);
    dt.setUTC(true);
    event.setStart(dt);
    writeEvent(event);
    QCOMPARE(Kolab::error(), Kolab::Error);
}

void ValidationTest::testTimezoneZ()
{
    Event event;
    event.setStart(cDateTime("Z",2013,1,1,1,1,1));
    writeEvent(event);
    QCOMPARE(Kolab::error(), Kolab::Error);
}

void ValidationTest::testWindowsTimezone()
{
    Event event;
    event.setStart(cDateTime("Central European Standard Time",2013,1,1,1,1,1));
    writeEvent(event);
    QCOMPARE(Kolab::error(), Kolab::Error);
}


QTEST_MAIN( ValidationTest )

#include "validationtest.moc"
