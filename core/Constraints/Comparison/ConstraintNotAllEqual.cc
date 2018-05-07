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
 * @CreateTime: Nov 24, 2017 3:05 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintNotAllEqual.hh"

#include <limits>

using namespace std;

bool ConstraintNotAllEqual::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    int uniqueUnfixedVar = -1;
    int uniqueFixedVal = numeric_limits<int>::max();

    for (size_t i = 0; i < listSize; ++i) {
        if (!scope[i]->isAssigned()) {
            if (uniqueUnfixedVar != -1)
                return false; // AC because at least two unfixed variables
            uniqueUnfixedVar = i;
        } else {
            if (uniqueFixedVal == numeric_limits<int>::max())
                uniqueFixedVal = scope[i]->getLowerBoundVal();
            else if (uniqueFixedVal != scope[i]->getLowerBoundVal())
                return false; // AC because two fixed variables with different values
        }
    }

    if (uniqueUnfixedVar == -1)
        return true; // because all variables are assigned to the same value

    assert(uniqueFixedVal != numeric_limits<int>::max());

    int before = scope[uniqueUnfixedVar]->domainCurSize;
    if (scope[uniqueUnfixedVar]->removeValue(uniqueFixedVal, level, this))
        return true;

    if (before != scope[uniqueUnfixedVar]->domainCurSize)
        touched.push_back(scope[uniqueUnfixedVar]);
    return false;
}