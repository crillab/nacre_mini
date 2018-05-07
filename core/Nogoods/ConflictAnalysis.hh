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
 * @CreateTime: Jan 3, 2018 1:56 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONFLICTANALYSIS_H_
#define CONFLICTANALYSIS_H_

class Constraint;

#include "Clauses.hh"
#include "Variable.hh"

#include <set>
#include <vector>
#include <utility>

class ConflictAnalysis {
protected:
    std::vector<unsigned> cl;
    std::vector<unsigned> conflictCl;
    std::set<int> uniqueVar;

    std::set<std::pair<unsigned, Expl*>, varOrderCmp> stack;

    Clauses* clHandler;

    inline std::pair<unsigned, Expl*> pop()
    {
        std::pair<unsigned, Expl*> lit = *stack.begin();
        stack.erase(stack.begin());
        return lit;
    }

    void buildStack(std::vector<unsigned>& conflictCl, int level);

    void printHeaderAnalyse(Constraint* ctr);

public:
    void setHandler(Clauses* clh) { clHandler = clh; }

    static unsigned long long gblCaStamp;

    virtual std::vector<unsigned> analyse(Constraint* ctr, int level, int& bjLevel, int& uniqVar);
};

#endif //CONFLICTANALYSIS_H_