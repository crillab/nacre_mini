/***************************************************************************************************
* Copyright (c) 2018 Artois University & CNRS
*
* This file is part of Nacre_mini
*
* Nacre_mini is free software: you can redistribute it and/or modify it 
* under the terms of the GNU General Public License as published by the Free Software Foundation, 
* either version 3 of the License, or (at your option) any later version.
*
* Nacre_mini is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with Nacre_mini. 
* If not, see http://www.gnu.org/licenses/.
*
* Contributors:
*       - Gaël Glorian (glorian@cril.fr)

***************************************************************************************************/

/*
 * @CreateTime: Sep 26, 2017 2:51 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Modify Here, Please 
 */

#include <csignal>

using namespace std;

#include "InfoSystem.hh"
#include "XCSP3CoreParser.h"
#include "main.hh"

using namespace XCSP3Core;

Solver* s;

void handle_interrupt(int sig)
{
    s->showFinal(2);
    exit(0);
} // handle_interrupt

int main(int argc, char** argv)
{

    XCSP3Callbacks cb; // my interface between the parser and the solver

    if (argc < 3) {
        if (argc == 1) {
            show_help();
            exit(0);
        }
        std::string arg = argv[1];
        if (!arg.compare("-moreHelp"))
            show_advanced_help();
        else
            show_help();
        exit(0);
    }

    try {
        XCSP3CoreParser parser(&cb);
        parser.parse(argv[1]);
        argv++;
        argc--;

        signal(SIGINT, handle_interrupt);
        signal(SIGTERM, handle_interrupt);
        signal(SIGALRM, handle_interrupt);

        s = create_solver(0, argc, argv);

        exit(s->showFinal(s->solve()));

    } catch (exception& e) {
        cout.flush();
        cerr << "\n\tUnexpected exception :\n";
        cerr << "\t" << e.what() << endl;
        exit(1);
    }

    return 0;
}
