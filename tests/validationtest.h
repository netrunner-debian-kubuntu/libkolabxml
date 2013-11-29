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
#ifndef VALIDATIONTEST_H
#define VALIDATIONTEST_H


#include <QObject>

class ValidationTest: public QObject {
    Q_OBJECT
private slots:
    void testErrorOnEmptyEvent();
    void testNoErrorOnValidEvent();
    void testNoErrorOnValidTodo();
    void testOlsonTimezone();
    void testDifferentTimezones();
    void testUTCwithTimezone();
    void testTimezoneZ();
    void testWindowsTimezone();
};

#endif
