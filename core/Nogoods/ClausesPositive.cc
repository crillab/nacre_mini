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
 * @Last Modified Time: May 7, 2018 6:04 PM
 * @Description: Modify Here, Please 
 */

#include "ClausesPositive.hh"
#include "LecoutreNoGoodsManager.hh"
#include "Variable.hh"

using namespace std;

size_t ClausesPositive::searchAnotherWatch(vector<ind>& cl)
{
    for (size_t another = 2; another < cl.size(); ++another)
        if (!Variable::varProps[cl[another]].toVar->isAssignedToIndVp(cl[another]))
            return another;
    return cl.size();
}

int ClausesPositive::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    assert(cur->isAssigned());

    indVp lit = cur->getLowerBoundInd();
    vector<unsigned>& idxClauses = watched[lit];
    vector<unsigned> newWatch;

    //Check all clauses where lit is a watch
    for (size_t i = 0, stop = idxClauses.size(); i < stop; ++i) {
        vector<ind>& cl = clauses[idxClauses[i]];

        // Putting lit at pos 1
        if (lit == cl[0])
            swap(cl[0], cl[1]);

        // Searching for another watch
        size_t another = searchAnotherWatch(cl);

        // If a new watch is found
        if (another < cl.size()) {
            swap(cl[1], cl[another]);
            watched[cl[1]].push_back(idxClauses[i]);
            continue;
        }

        // Their is no new watch => its a unit clause
        // Checking also watch at pos 0 to know if the last watch is assigned too
        Variable* zV = Variable::varProps[cl[0]].toVar;

        if (zV->isAssignedToIndVp(cl[0])) { // UNSAT because assigned to the watched value
            assert(zV->getLowerBoundVal() == Variable::varProps[cl[0]].val);
            Stats::nbConflict++;
            newWatch.insert(newWatch.end(), watched[lit].begin() + i + 1, watched[lit].end());
            watched[lit] = newWatch;
            return idxClauses[i];
        }

        // if the value is still alive and not alone in the domain => remove it
        int before = zV->domainCurSize;
        zV->removeAt(Variable::varProps[cl[0]].posInVar, level);
        Stats::nbSupp++;
        if (before != zV->domainCurSize)
            touched.push_back(zV);

        newWatch.push_back(idxClauses[i]); //lit continue to be a watched value for backtracking
    }

    watched[lit] = newWatch;
    return -1;
} // propagate

/**
    Add the clause in the constraint and watch the two first litterals.

    @param[in] cl, a set of 'literals'
*/
void ClausesPositive::addClause(vector<indVp>& cl)
{
    assert(cl.size() > 1);
    watched[cl[0]].push_back(clauses.size());
    watched[cl[1]].push_back(clauses.size());

    clauses.push_back(cl);
    Stats::nbNoGoods++;
} // addClause
