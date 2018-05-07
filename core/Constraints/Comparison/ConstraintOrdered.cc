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
 * @CreateTime: Nov 24, 2017 4:35 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintOrdered.hh"

using namespace std;

bool ConstraintOrderedLT::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    for (int i = listSize - 2; i >= 0; --i) {
        int before = scope[i]->domainCurSize;
        if (scope[i]->removeValuesAbove(scope[i + 1]->getUpperBoundVal() - 1, level, this))
            return true;
        if (before != scope[i]->domainCurSize)
            touched.push_back(scope[i]);
    }

    for (size_t i = 0; i < listSize - 1; ++i) {
        int before = scope[i + 1]->domainCurSize;
        if (scope[i + 1]->removeValuesUnder(scope[i]->getLowerBoundVal() + 1, level, this))
            return true;
        if (before != scope[i + 1]->domainCurSize)
            touched.push_back(scope[i + 1]);
    }

    return false;
}

bool ConstraintOrderedLE::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    for (int i = listSize - 2; i >= 0; --i) {
        int before = scope[i]->domainCurSize;
        if (scope[i]->removeValuesAbove(scope[i + 1]->getUpperBoundVal(), level, this))
            return true;
        if (before != scope[i]->domainCurSize)
            touched.push_back(scope[i]);
    }

    for (size_t i = 0; i < listSize - 1; ++i) {
        int before = scope[i + 1]->domainCurSize;
        if (scope[i + 1]->removeValuesUnder(scope[i]->getLowerBoundVal(), level, this))
            return true;
        if (before != scope[i + 1]->domainCurSize)
            touched.push_back(scope[i + 1]);
    }

    return false;
}

bool ConstraintOrderedGE::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    for (size_t i = 0; i < listSize - 1; ++i) {
        int before = scope[i + 1]->domainCurSize;
        if (scope[i + 1]->removeValuesAbove(scope[i]->getUpperBoundVal(), level, this))
            return true;
        if (before != scope[i + 1]->domainCurSize)
            touched.push_back(scope[i + 1]);
    }

    for (int i = listSize - 2; i >= 0; --i) {
        int before = scope[i]->domainCurSize;
        if (scope[i]->removeValuesUnder(scope[i + 1]->getLowerBoundVal(), level, this))
            return true;
        if (before != scope[i]->domainCurSize)
            touched.push_back(scope[i]);
    }

    return false;
}

bool ConstraintOrderedGT::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    for (size_t i = 0; i < listSize - 1; ++i) {
        int before = scope[i + 1]->domainCurSize;
        if (scope[i + 1]->removeValuesAbove(scope[i]->getUpperBoundVal() - 1, level, this))
            return true;
        if (before != scope[i + 1]->domainCurSize)
            touched.push_back(scope[i + 1]);
    }

    for (int i = listSize - 2; i >= 0; --i) {
        int before = scope[i]->domainCurSize;
        if (scope[i]->removeValuesUnder(scope[i + 1]->getLowerBoundVal() + 1, level, this))
            return true;
        if (before != scope[i]->domainCurSize)
            touched.push_back(scope[i]);
    }

    return false;
}