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
 * @CreateTime: Nov 27, 2017 9:38 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintSumScalarBoolean.hh"

using namespace std;

bool SumScalarBooleanLE::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    recomputeBounds();

    if (max <= limit)
        return false;
    if (min > limit)
        return true;

    // this is the only case where we can filter
    if (min == limit)
        filter(level, touched, 1);

    return false;
}

bool SumScalarBooleanEQ::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    recomputeBounds();

    if (min > limit || max < limit)
        return true;
    if (min == max || (min < limit && limit < max))
        return false; // because entailed in that case

    if (min == limit)
        filter(level, touched, 1);
    else if (max == limit) {
        filter(level, touched, 0);
        for (int i = limitSetEq; i >= 0; i--) {
            int j = set01vs01[i];
            assert(scope[j]->domainCurSize == 2 && scope[half + j]->domainCurSize == 2);

            scope[j]->removeValue(0, level, this);
            touched.push_back(scope[j]);

            scope[half + j]->removeValue(0, level, this);
            touched.push_back(scope[half + j]);
        }
    }

    return false;
}

bool SumScalarBooleanLEVar::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    recomputeBounds();

    int before = limit->domainCurSize;
    if (limit->removeValuesUnder(min, level, this))
        return true;
    if (before != limit->domainCurSize)
        touched.push_back(limit);

    if (max <= limit->getUpperBoundVal())
        return false;

    // this is the only case where we can filter
    if (min == limit->getUpperBoundVal())
        filter(level, touched, 1);

    return false;
}