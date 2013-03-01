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

#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "src/kolabformat.h"

namespace po = boost::program_options;
using namespace std;

int main(int argc, char *argv[])
{
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("contact", "parse contact")
        ("distlist", "parse distlist")
        ("event", "parse event")
        ("todo", "parse todo")
        ("journal", "parse journal")
        ("freebusy", "parse freebusy")
        ("note", "parse note")
        ("configuration", "parse configuration")
        ("file", "parse file")
        ("input-file", po::value<std::vector<std::string> >(), "input files of given type")
    ;

    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
            options(desc).positional(p).run(), vm);
    po::notify(vm);

    if (vm.count("help")) {
        cout << desc << "\n";
        return 1;
    }

    vector<string> inputFiles;
    if (vm.count("input-file")) {
        inputFiles = vm["input-file"].as< vector<string> >();
    } else {
        cout << "Specify input-file";
        return -1;
    }

    for(vector<string>::const_iterator it = inputFiles.begin();
            it != inputFiles.end(); it++){

        if (vm.count("contact")) {
            Kolab::readContact(*it, true);
        } else if (vm.count("distlist")) {
            Kolab::readDistlist(*it, true);
        } else if (vm.count("event")) {
            Kolab::readEvent(*it, true);
        } else if (vm.count("todo")) {
            Kolab::readTodo(*it, true);
        } else if (vm.count("journal")) {
            Kolab::readJournal(*it, true);
        } else if (vm.count("freebusy")) {
            Kolab::readFreebusy(*it, true);
        } else if (vm.count("note")) {
            Kolab::readNote(*it, true);
        } else if (vm.count("configuration")) {
            Kolab::readConfiguration(*it, true);
        } else if (vm.count("file")) {
            Kolab::readFile(*it, true);
        } else {
            cout << "Specify type";
            return -1;
        }
        if (Kolab::error()) {
            cout << "Error: " << Kolab::errorMessage() << endl;
            return -1;
        } else {
            cout << "Parsed message without error." << endl;
        }
    }
    return 0;
}