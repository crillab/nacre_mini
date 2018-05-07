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
 * @CreateTime: Oct 12, 2017 6:09 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintPrimitive.hh"

#include <algorithm>
#include <cassert>
#include <vector>

using namespace std;

bool ConstraintPrimitiveEqual::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    int before = x->domainCurSize;
    if (x->keepOnlyValues(y, -k, level, this))
        return true;
    if (before != x->domainCurSize)
        touched.push_back(x);

    assert(x->domainCurSize <= y->domainCurSize);

    if (x->domainCurSize == y->domainCurSize)
        return false;

    before = y->domainCurSize;
    if (y->keepOnlyValues(x, k, level, this))
        return true;
    if (before != y->domainCurSize)
        touched.push_back(y);

    return false;
}

bool ConstraintPrimitiveLessEqual::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    int before = x->domainCurSize;
    if (x->removeValuesAbove(y->getUpperBoundVal() - k, level, this))
        return true;
    if (before != x->domainCurSize)
        touched.push_back(x);

    before = y->domainCurSize;
    if (y->removeValuesUnder(x->getLowerBoundVal() + k, level, this))
        return true;
    if (before != y->domainCurSize)
        touched.push_back(y);

    return false;
}

bool ConstraintPrimitiveLessThan::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    int before = x->domainCurSize;
    if (x->removeValuesAbove(y->getUpperBoundVal() - k - 1, level, this))
        return true;
    if (before != x->domainCurSize)
        touched.push_back(x);

    before = y->domainCurSize;
    if (y->removeValuesUnder(x->getLowerBoundVal() + k + 1, level, this))
        return true;
    if (before != y->domainCurSize)
        touched.push_back(y);

    return false;
}

bool ConstraintPrimitiveNotEqual::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    int before = y->domainCurSize;
    if (x->isAssigned() && y->removeValue(x->getOnlyValue() + k, level, this))
        return true;
    if (before != y->domainCurSize)
        touched.push_back(y);

    before = x->domainCurSize;
    if (y->isAssigned() && x->removeValue(y->getOnlyValue() - k, level, this))
        return true;
    if (before != x->domainCurSize)
        touched.push_back(x);

    return false;
}

bool ConstraintDisjunctive::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    int szX = x->domainCurSize;
    int szY = y->domainCurSize;

    for (;;) {
        int minx = x->getLowerBoundVal(), maxx = x->getUpperBoundVal();
        int miny = y->getLowerBoundVal(), maxy = y->getUpperBoundVal();

        if (!filterDomain(x, level, max(minx, min(maxx, maxy - k) + 1), min(maxx, max(minx, miny + ky) - 1)))
            return true;
        if (!filterDomain(y, level, max(miny, min(maxy, maxx - ky) + 1), min(maxy, max(miny, minx + k) - 1)))
            return true;

        if (minx == x->getLowerBoundVal() && maxx == x->getUpperBoundVal() && miny == y->getLowerBoundVal() && maxy == y->getUpperBoundVal())
            break;
    }

    if (szX != x->domainCurSize)
        touched.push_back(x);
    if (szY != y->domainCurSize)
        touched.push_back(y);

    return false;
}

bool ConstraintDisjunctive::filterDomain(Variable* cur, int level, int lbValue, int ubValue)
{
    // remove all values from cur between lbValue and ubValue (we assume that domains have the same type)
    if (lbValue > ubValue)
        return true; // nothing to filter

    if (cur->isAssigned())
        return !(lbValue <= x->getLowerBoundVal() && x->getLowerBoundVal() <= ubValue);

    for (int i = cur->domainCurSize - 1; i >= 0; --i) {
        int val = cur->LocalDomIndToVal(i);
        if (lbValue <= val && val >= ubValue && deleteValue(cur, i, level))
            return false;
    }

    return true;
}