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
 * @CreateTime: Oct 27, 2017 9:49 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintSum.hh"

using namespace std;

bool ConstraintSumEQ::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    recomputeBounds();

    if (limit < min || max < limit)
        return true;

    int positionOfLastChange = -1;
    do {
        for (int i = 0; (size_t)i < listSize; i++) {
            if (i == positionOfLastChange) {
                positionOfLastChange = -1;
                break;
            }

            Variable* var = scope[i];

            int sizeBefore = var->domainCurSize;
            if (sizeBefore == 1)
                continue;

            min -= var->getLowerBoundVal();
            max -= var->getUpperBoundVal();

            for (int j = var->domainCurSize - 1; j >= 0; j--) {
                int v = var->LocalDomIndToVal(j);
                if (v < limit - max || v > limit - min)
                    if (deleteValue(var, j, level))
                        return true;
            }

            if (sizeBefore != var->domainCurSize) {
                touched.push_back(var);
                positionOfLastChange = i;
            }

            min += var->getLowerBoundVal();
            max += var->getUpperBoundVal();
        }
    } while (positionOfLastChange != -1);

    return false;
}

bool ConstraintSumGE::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    recomputeBounds();

    if (min >= limit)
        return false;
    if (max < limit)
        return true;

    for (size_t i = 0; i < listSize; i++) {
        Variable* var = scope[i];

        int sizeBefore = var->domainCurSize;
        if (sizeBefore == 1)
            continue;

        min -= var->getLowerBoundVal();

        int curLim = limit - (max - var->getUpperBoundVal());
        for (int j = var->domainCurSize - 1; j >= 0; j--)
            if (var->LocalDomIndToVal(j) < curLim)
                deleteValue(var, j, level);

        if (sizeBefore != var->domainCurSize)
            touched.push_back(var);

        assert(var->domainCurSize);

        min += var->getLowerBoundVal();
        if (min >= limit)
            return false;
    }

    return false;
}

bool ConstraintSumLE::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    recomputeBounds();

    if (max <= limit)
        return false;
    if (min > limit)
        return true;

    for (size_t i = 0; i < listSize; i++) {
        Variable* var = scope[i];

        int sizeBefore = var->domainCurSize;
        if (sizeBefore == 1)
            continue;

        max -= var->getUpperBoundVal();

        int curLim = limit - (min - var->getLowerBoundVal());
        for (int j = var->domainCurSize - 1; j >= 0; j--)
            if (var->LocalDomIndToVal(j) > curLim)
                deleteValue(var, j, level);

        if (sizeBefore != var->domainCurSize)
            touched.push_back(var);

        assert(var->domainCurSize);

        max += var->getUpperBoundVal();
        if (max <= limit)
            return false;
    }

    return false;
}

bool ConstraintSumNE::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    if (sentinel1 == -1 || scope[sentinel1]->isAssigned()) {
        sentinel1 = findAnotherSentinel();
        if (sentinel1 == -1) { // No new non singleton variable
            if (sentinel2 == -1 || scope[sentinel2]->isAssigned())
                return currentSum() == limit;

            int before = scope[sentinel2]->domainCurSize;
            if (scope[sentinel2]->removeValue((limit - currentSumExcept(sentinel2)), level, this))
                return true;

            if (before != scope[sentinel2]->domainCurSize)
                touched.push_back(scope[sentinel2]);
            return false;
        }
    }

    if (sentinel2 == -1 || scope[sentinel2]->isAssigned()) {
        sentinel2 = findAnotherSentinel();

        if (sentinel2 == -1) {
            int before = scope[sentinel1]->domainCurSize;
            if (scope[sentinel1]->removeValue((limit - currentSumExcept(sentinel1)), level, this))
                return true;

            if (before != scope[sentinel1]->domainCurSize)
                touched.push_back(scope[sentinel1]);
            return false;
        }
    }

    return false;
}