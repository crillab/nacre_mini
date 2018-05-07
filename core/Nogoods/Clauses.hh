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
 * @CreateTime: Dec 4, 2017 3:48 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#ifndef CLAUSES_H_
#define CLAUSES_H_

#include "Stats.hh"
#include "Variable.hh"

#include <vector>

class RefClause;

class Clauses {
protected:
    std::vector<std::vector<unsigned>> watched;
    std::vector<unsigned> freeSpaces;

    std::vector<unsigned> clState;
    std::vector<unsigned> uniqVars;

    std::vector<bool> variablesUtil;
    std::vector<Variable*> varPtrUtil;

    int clauseDbSize = 4000;
    int nClausesInDB = 0;

    virtual int checkWatch(unsigned lit, int level, std::vector<Variable*>& touched);

    bool delClause(unsigned id);
    void reduceDB();

    inline bool sign(unsigned x) { return x & 1; }

public:
    std::vector<std::vector<unsigned>> clauses;
    std::vector<RefClause*> refs;

    /** Constructor */
    Clauses()
    {
        watched.resize(Variable::varProps.size() << 1);
        variablesUtil.resize(Variable::varProps.back().toVar->getId());
    }

    /** Propagator for nogoods */
    int propagate(int level, Variable* cur, std::vector<Variable*>& touched);

    /** Add the clause @cl to the base and init the watched literals */
    Constraint* addClause(std::vector<unsigned>& cl, int uniqVar = 0);
};
#endif //CLAUSES_H_
