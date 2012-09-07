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

#ifndef KOLAB_GLOBAL_DEFINITIONS_H
#define KOLAB_GLOBAL_DEFINITIONS_H

namespace Kolab {

enum ErrorSeverity {
    NoError,
    Warning, //Warning, error could be corrected, object can be used without dataloss
    Error, //Potentially corrupt object, writing the object back could result in dataloss. (Object could still be used to display the data readonly).
    Critical //Ciritcal error, produced object cannot be used and should be thrown away (writing back will result in dataloss).
};

}

#endif
