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
 * @Description: Options handler
 */

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <iostream>

enum class valueSelectionTypes { zero,
    min,
    max,
    vrand,
    last };

enum class variableHeuristic { dom,
    domdeg,
    domwdeg };

enum class countTypes { no,
    all };

enum class verbose { no,
    low,
    medium,
    high };

enum class rstTypes { noRst,
    low,
    fifty_perc,
    luby10,
    luby50,
    luby100,
    hundred_perc,
    ten_perc };

namespace Options {
extern int baseRst;
extern rstTypes optRst;
extern countTypes countSols;
extern valueSelectionTypes valueSelection;
extern bool assign;
extern bool saving;
extern variableHeuristic varHeuristic;
extern verbose Verbose;
extern bool SaveExpl;
extern bool reduceDBOpt;
extern bool keepBinary;

/// Load Options from program arguments
bool load_options(int& argc, char** argv, int& method);
}
#endif //OPTIONS_H_
