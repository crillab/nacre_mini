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
 * @CreateTime: Apr 24, 2018 3:38 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Modify Here, Please 
 */

#include "Stats.hh"

#include <iostream>

namespace Stats {
// CONSTRAINTS
unsigned long long nbConstraints = 0;
unsigned long long saveNbConstraints = 0;
unsigned long long minArity = -1;
unsigned long long maxArity = 0;

// NOGOODS - CLAUSES
unsigned long long nbSupp = 0;
unsigned long long nbConflict = 0;
unsigned long long nbNoGoods = 0;
unsigned long long unit = 0;
unsigned long long binary = 0;
unsigned long long unitVar = 0;

// AC
unsigned long long assignCall = 0;
unsigned long long rmCall = 0;
unsigned long long nbPropCalls = 0;
unsigned long long nbDecisions = 0;

// SOLVER
unsigned long long run = 1;
unsigned long long conflict = 0;
unsigned long long nbSols = 0;

// VARS
unsigned long long nbVarProps = 0;
unsigned long long saveNbVarProps = 0;
unsigned long long nbVars = 0;
unsigned long long uselessVars = 0;
}

void Stats::print_all_stats()
{
    std::cout << std::endl;
}

void Stats::print_after_init()
{
    std::cout << "c" << std::endl;
    std::cout << "c # Vars " << Stats::nbVars << "(" << Stats::nbVarProps << ")" << std::endl;
    std::cout << "c # Assigned Vars " << Stats::assignCall << std::endl;
    std::cout << "c # Useless Vars " << Stats::uselessVars << std::endl;
    std::cout << "c # Removed Values " << Stats::rmCall << std::endl;
    std::cout << "c # Constraints " << Stats::nbConstraints << std::endl;
    std::cout << "c" << std::endl;
}
