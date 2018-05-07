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
 * @CreateTime: Oct 2, 2017 1:45 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Modify Here, Please 
 */

#include "Options.hh"

#include <cstring>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>

namespace Options {
int baseRst = 10;
rstTypes optRst = rstTypes::low;
valueSelectionTypes valueSelection = valueSelectionTypes::min;
bool assign = true;
bool saving = false;
countTypes countSols = countTypes::no;
variableHeuristic varHeuristic = variableHeuristic::domwdeg;
verbose Verbose = verbose::low;
bool SaveExpl = false;
bool reduceDBOpt = true;
bool keepBinary = true;
}

bool Options::load_options(int& argc, char** argv, int& method)
{
    std::string strMethod = argv[1];
    if (!strMethod.compare("-moreHelp")) {
        method = 0;
    } else if (!strMethod.compare("-complete")) {
        method = 1;
    } else if (!strMethod.compare("-incng"))
        method = 2;
    else if (!strMethod.compare("-ngen"))
        method = 3;
    else if (!strMethod.compare("-nld"))
        method = 4;
    else if (!strMethod.compare("-ca")) {
        Options::SaveExpl = true;
        method = 5;
    } else if (!strMethod.compare("-infosCSP"))
        method = 10;
    else
        return true;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        // Count solutions
        if (!arg.compare("-sols")) {
            Options::countSols = countTypes::all;
            if (method == 1) {
                Options::optRst = rstTypes::noRst;
                baseRst = std::numeric_limits<int>::max();
            }
        }

        // Restarts
        else if (!arg.compare("-noRst")) {
            Options::optRst = rstTypes::noRst;
            baseRst = std::numeric_limits<int>::max();
        } else if (!arg.compare("-double")) {
            Options::optRst = rstTypes::hundred_perc;
        } else if (!arg.compare("-50perc")) {
            Options::optRst = rstTypes::fifty_perc;
        } else if (!arg.compare("-10perc")) {
            Options::optRst = rstTypes::ten_perc;
        } else if (!arg.compare("-luby10")) {
            Options::optRst = rstTypes::luby10;
            baseRst = 10;
        } else if (!arg.compare("-luby100") || !arg.compare("-l100")) {
            Options::optRst = rstTypes::luby100;
            baseRst = 100;
        } else if (!arg.compare("-luby50")) {
            Options::optRst = rstTypes::luby50;
            baseRst = 50;
        }

        // Heuristics -- Values
        else if (!arg.compare("-valMin")) {
            Options::valueSelection = valueSelectionTypes::min;
        } else if (!arg.compare("-valMax")) {
            Options::valueSelection = valueSelectionTypes::max;
        } else if (!arg.compare("-valRand")) {
            Options::valueSelection = valueSelectionTypes::vrand;
        } else if (!arg.compare("-valFirst")) {
            Options::valueSelection = valueSelectionTypes::zero;
        } else if (!arg.compare("-valLast")) {
            Options::valueSelection = valueSelectionTypes::last;
        }

        // Heuristics -- Variables
        else if (!arg.compare("-dom")) {
            Options::varHeuristic = variableHeuristic::dom;
        } else if (!arg.compare("-domdeg")) {
            Options::varHeuristic = variableHeuristic::domdeg;
        } else if (!arg.compare("-domwdeg") || !arg.compare("-dwd")) {
            Options::varHeuristic = variableHeuristic::domwdeg;
        }

        // Heuristics -- Miscellaneous
        else if (!arg.compare("-refutation")) {
            Options::assign = false;
        } else if (!arg.compare("-saving")) {
            Options::saving = true;
        }

        // Conflict analysis options
        else if (!arg.compare("-noReduceDB")) {
            Options::reduceDBOpt = false;
        } else if (!arg.compare("-noBinaryCL")) {
            Options::keepBinary = false;
        }

        // Verbosity
        else if (!arg.compare("-verb=0") || !arg.compare("-cm")) {
            Options::Verbose = verbose::no;
        } else if (!arg.compare("-verb=1")) {
            Options::Verbose = verbose::low;
        } else if (!arg.compare("-verb=2")) {
            Options::Verbose = verbose::medium;
        } else if (!arg.compare("-verb=3")) {
            Options::Verbose = verbose::high;
        }

        else {
            std::cerr << "Unknow option \"" << arg << "\"." << std::endl;
            return true;
        }
    }
    return false;
}
