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
 * @CreateTime: Nov 22, 2017 2:28 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintElement.hh"

using namespace std;

bool ConstraintElementConstant::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    if (!index->isAssigned()) {
        int saveSize = index->domainCurSize;
        for (int i = saveSize - 1; i >= 0; --i)
            if (!getVariableFor(index->LocalDomIndToVal(i))->isValidValue(result))
                if (deleteValue(index, i, level))
                    return true;

        if (saveSize != index->domainCurSize)
            touched.push_back(index);
    }

    // domain can have changed
    if (index->isAssigned()) {
        Variable* var = getVariableFor(index->getOnlyValue()); // This variable must be equal to result
        if (!var->isAssignedTo(result)) {
            if (fixToValue(var, result, level)) {
                touched.push_back(var);
                return false;
            }
            return true;
        }
    }

    return false;
}

bool ConstraintElementVariable::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    // If index is not singleton, we try to prune values :
    // - in result's domain, we prune the values which aren't in any of list variables'domains
    // - in index's domain, we prune the values v for which there is no value j such that list[v] and value both have j in their
    // domains
    if (!index->isAssigned()) {
        // Update resultSentinels and dom(result)
        if (!reduceResultDomain(touched, level))
            return true;

        for (;;) {
            // Update listSentinels and dom(index)
            int sizeBefore = index->domainCurSize;
            if (!reduceIndexDomain(touched, level))
                return true;
            if (sizeBefore == index->domainCurSize)
                break;

            // Update resultSentinels and dom(result)
            sizeBefore = result->domainCurSize;
            if (!reduceResultDomain(touched, level))
                return true;
            if (sizeBefore == result->domainCurSize)
                break;
        }
    }

    // If index is singleton, we update dom(list[index]) and dom(result)
    // so that they are both equal to the intersection of the two domains
    if (index->isAssigned()) {
        Variable* var = getVariableFor(index->getOnlyValue());
        int before1 = var->domainCurSize;
        int before2 = result->domainCurSize;
        if (var->keepOnlyValues(result, 0, level) || result->keepOnlyValues(var, 0, level))
            return true;

        if (before1 != var->domainCurSize)
            touched.push_back(var);
        if (before2 != result->domainCurSize)
            touched.push_back(result);
    }

    return false;
}
