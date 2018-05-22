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
 * @CreateTime: Sep 28, 2017 8:37 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintExtN.hh"
#include "Options.hh"
#include "XCSP3Constants.h"

#include <cassert>
#include <set>

using namespace std;

ConstraintExtN::ConstraintExtN(string n, vector<Variable*> vars, vector<vector<ind>>& t)
    : Constraint(n, vars)
    , tuples(t)
{
    currentLimit = t.size();
    for (size_t i = 0; i < currentLimit; ++i)
        position.push_back(i);
    assert(vars.size());
    assert(vars.size() == t[0].size());
    lastSize.resize(vars.size(), -1);
}

bool ConstraintExtN::init()
{
    for (size_t j = 0; j < listSize; ++j)
        if (scope[j]->keepOnlyVarProp(tuples, j, 0, this))
            return true;

    return false;
}

bool ConstraintExtN::isValidTuple(vector<ind> tuple)
{
    for (auto ind : sVal)
        if (tuple[ind] != STAR && !scope[ind]->isValidVpInd(tuple[ind]))
            return false;
    return true;
}

void ConstraintExtN::removeTuple(size_t tuplePos, int level)
{
    assert(tuplePos < currentLimit);

    if (saveSize.empty() || saveSize.top().level != level)
        saveSize.push({ level, currentLimit, lastSize });

    saveSize.top().sizeDom--;
    currentLimit--;

    std::swap(position[tuplePos], position[currentLimit]);
}

void ConstraintExtN::restoreTuples(int level)
{
    if (saveSize.empty() || saveSize.top().level < level)
        return;

    while (saveSize.size() && level < saveSize.top().level)
        saveSize.pop();

    if (saveSize.empty()) {
        currentLimit = tuples.size();
        for (size_t j = 0, stop = lastSize.size(); j < stop; ++j)
            lastSize[j] = -1;
    } else {
        currentLimit = saveSize.top().sizeDom;
        lastSize = saveSize.top().lastSize;
    }
}

bool ConstraintExtN::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    sSup.clear();
    sVal.clear();

    for (size_t i = 0; i < listSize; ++i) {
        Variable* v = scope[i];
        if (v->wasPushed && !v->lastPushed)
            continue;

        sSup.push_back(i);
        v->cleanDBU();

        if (v->domainCurSize != lastSize[i] || v->lastPushed) {
            sVal.push_back(i);
            lastSize[i] = v->domainCurSize;
        }
    }

    for (size_t i = 0; i < currentLimit;) {
        if (isValidTuple(tuples[position[i]])) {
            for (int j = sSup.size() - 1; j >= 0; --j) {
                int s = sSup[j];
                if (tuples[position[i]][s] != STAR) {
                    Variable* v = scope[s];
                    v->insertInDBU(v->indVpToIndVpLocal(tuples[position[i]][s]));
                    if (v->DBUSize != (size_t)scope[s]->domainCurSize)
                        continue;
                }

                swap(sSup[j], sSup[sSup.size() - 1]);
                sSup.pop_back();
            }
            ++i;
        } else
            removeTuple(i, level);
    }

    if (!currentLimit)
        return true;

    for (auto i : sSup) {
        int before = scope[i]->domainCurSize;
        if (!scope[i]->DBUSize)
            return true;
        if (scope[i]->delInDBU(level, this))
            return true;

        if (before != scope[i]->domainCurSize)
            touched.push_back(scope[i]);

        lastSize[i] = scope[i]->domainCurSize;
    }

    return false;
}
