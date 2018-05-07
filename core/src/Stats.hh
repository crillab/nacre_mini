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
 * @CreateTime: Apr 24, 2018 3:37 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Modify Here, Please 
 */

#ifndef STATS_H_
#define STATS_H_

namespace Stats {
// CONSTRAINTS
extern unsigned long long nbConstraints;
extern unsigned long long saveNbConstraints;
extern unsigned long long minArity;
extern unsigned long long maxArity;

// NOGOODS - CLAUSES
extern unsigned long long nbSupp;
extern unsigned long long nbConflict;
extern unsigned long long nbNoGoods;
extern unsigned long long unit;
extern unsigned long long binary;
extern unsigned long long unitVar;

// AC
extern unsigned long long assignCall;
extern unsigned long long rmCall;
extern unsigned long long nbPropCalls;
extern unsigned long long nbDecisions;

// SOLVER
extern unsigned long long run;
extern unsigned long long conflict;
extern unsigned long long nbSols;
extern unsigned long long nbRst;

// VARS
extern unsigned long long nbVarProps;
extern unsigned long long saveNbVarProps;
extern unsigned long long nbVars;
extern unsigned long long uselessVars;

void print_after_init();

void print_all_stats();
}
#endif //STATS_H_
