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

#include <cassert>
#include <set>

#include "ConstraintExtNConflict.hh"
#include "Options.hh"

using namespace std;

ConstraintExtNConflict::ConstraintExtNConflict(string n, vector<Variable*> vars, vector<vector<ind>>& t)
    : Constraint(n, vars)
    , tuples(t)
{
    currentLimit = t.size();
    for (size_t i = 0; i < currentLimit; ++i)
        position.push_back(i);
    assert(vars.size());
    assert(vars.size() == t[0].size());
}

bool ConstraintExtNConflict::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    for (size_t i = 0; i < currentLimit;) {
        if (isValidTuple(tuples[position[i]]))
            ++i;
        else
            removeTuple(i, level);
    }

    int nValidTuples = 1;
    for (size_t i = 0; i < listSize && nValidTuples < 10000; ++i)
        nValidTuples *= scope[i]->domainCurSize;

    if (nValidTuples >= 10000)
        return false;

    for (size_t i = 0; i < listSize; ++i) {
        int saveSize = scope[i]->domainCurSize;
        size_t nbValidTuplesOfValues = nValidTuples / saveSize;
        for (int j = saveSize - 1; j >= 0; j--) {
            assert(nbValidTuplesOfValues >= timesSeen[i][scope[i]->indDomLocalToIndVPLocal(j)]);
            if (nbValidTuplesOfValues == timesSeen[i][scope[i]->indDomLocalToIndVPLocal(j)] && deleteValue(scope[i], j, level))
                return true;
        }

        if (saveSize != scope[i]->domainCurSize)
            touched.push_back(scope[i]);
    }

    return false;
}

bool ConstraintExtNConflict::init()
{
    for (size_t i = 0; i < listSize; ++i)
        timesSeen.push_back(vector<size_t>(scope[i]->domainInitSize, 0));

    for (size_t i = 0, stop = tuples.size(); i < stop; ++i)
        for (size_t j = 0; j < listSize; ++j) {
            timesSeen[j][tuples[position[i]][j] - scope[j]->domainStart]++;
            assert(timesSeen[j][tuples[position[i]][j] - scope[j]->domainStart] <= stop);
        }

    return false;
}

bool ConstraintExtNConflict::isValidTuple(vector<ind> tuple)
{
    for (size_t i = 0; i < listSize; i++)
        if (!scope[i]->isValidVpInd(tuple[i]))
            return false;
    return true;
}

void ConstraintExtNConflict::removeTuple(size_t tuplePos, int level)
{
    assert(tuplePos < currentLimit);

    if (saveSize.empty() || saveSize.top().level != level)
        saveSize.push({ level, currentLimit });

    saveSize.top().sizeDom--;
    currentLimit--;

    for (size_t j = 0; j < listSize; ++j) {
        timesSeen[j][tuples[position[tuplePos]][j] - scope[j]->domainStart]--;
        assert(timesSeen[j][tuples[position[tuplePos]][j] - scope[j]->domainStart] >= 0);
    }

    std::swap(position[tuplePos], position[currentLimit]);
}

void ConstraintExtNConflict::restoreTuples(int level)
{
    if (saveSize.empty() || saveSize.top().level < level)
        return;

    while (saveSize.size() && level < saveSize.top().level)
        saveSize.pop();

    size_t oldLimit = currentLimit;

    if (saveSize.empty())
        currentLimit = tuples.size();
    else
        currentLimit = saveSize.top().sizeDom;

    for (size_t i = oldLimit; i < currentLimit; ++i)
        for (size_t j = 0; j < listSize; ++j)
            timesSeen[j][tuples[position[i]][j] - scope[j]->domainStart]++;
}