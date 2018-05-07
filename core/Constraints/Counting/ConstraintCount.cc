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
 * @CreateTime: Nov 16, 2017 5:03 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintCount.hh"

using namespace std;

bool ConstraintExactlyK::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    int nGuaranteedOccurrences = 0; // number of singleton domains with the specified value
    int nPossibleOccurrences = 0; // number of variables still containing the specified value

    for (auto var : scope)
        if (var->isValidValue(value)) {
            nPossibleOccurrences++;
            if (var->isAssigned() && ++nGuaranteedOccurrences > k)
                return true;
        }

    if (nGuaranteedOccurrences == k) {
        // remove value from all non singleton domains
        for (int i = listSize - 1; i >= 0; i--) {
            Variable* var = scope[i];
            int before = var->domainCurSize;
            if (!var->isAssigned()) {
                if (var->removeValue(value, level, this))
                    return true;
                if (before != var->domainCurSize)
                    touched.push_back(var);
            }
        }
        return false;
    }

    if (nPossibleOccurrences < k)
        return true; // inconsistency detected

    if (nPossibleOccurrences == k) {
        // assign all non singleton variable containing the value to it
        for (int i = listSize - 1; i >= 0; i--) {
            Variable* var = scope[i];
            if (!var->isAssigned() && !var->isAssignedTo(value))
                if (fixToValue(var, value, level))
                    touched.push_back(var);
        }
    }

    return false;
}

bool ConstraintExactlyKVariable::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    int nGuaranteedOccurrences = 0;
    int nPossibleOccurrences = 0;

    // counting the nb of occurrences of value in the list
    for (size_t i = 0; i < listSize; ++i)
        if (scope[i]->isValidValue(value)) {
            nPossibleOccurrences++;
            if (scope[i]->isAssigned())
                nGuaranteedOccurrences++;
        }

    int before = var->domainCurSize;

    if (before == 1) {
        if (var->getLowerBoundVal() < nGuaranteedOccurrences || nPossibleOccurrences < var->getLowerBoundVal())
            return true;
    } else if (positionOfVarInList != -1 && var->isValidValue(value)) {
        // possible update of the domain of var when present in the vector, first by removing value (if present)
        // so as to update immediately nPossibleOccurrences
        bool deleted = false;
        for (int i = var->domainCurSize - 1; i >= 0; --i) {
            int v = var->getVarPropFromLocalDomInd(i).val;
            if (v == value) {
                if (value < nGuaranteedOccurrences + 1 || nPossibleOccurrences < value) { // +1 by assuming we assign the value
                    //cerr << v << "/" << nGuaranteedOccurrences << ":" << nPossibleOccurrences << endl;
                    if (var->removeValue(value, level, this))
                        return true;

                    deleted = true;
                }
            } else if (v < nGuaranteedOccurrences || nPossibleOccurrences - 1 < v) { // -1 by assuming we assign vx (and not value)
                if (deleteValue(var, i, level))
                    return true;
            }
        }

        if (deleted)
            nPossibleOccurrences--;
    } else if (var->removeValuesAbove(nPossibleOccurrences, level, this) || var->removeValuesUnder(nGuaranteedOccurrences, level, this))
        return true;

    if (before != var->domainCurSize)
        touched.push_back(var);

    // if var is singleton, updating the domain of the other variables
    if (var->isAssigned()) {
        int val = var->getLowerBoundVal();
        if (val == nGuaranteedOccurrences)
            for (size_t i = 0; i < listSize; ++i)
                if (!scope[i]->isAssigned()) {
                    int before = scope[i]->domainCurSize;
                    if (scope[i]->removeValue(value, level, this))
                        return true;
                    if (before != scope[i]->domainCurSize)
                        touched.push_back(scope[i]);
                }

        if (val == nPossibleOccurrences)
            for (size_t i = 0; i < listSize; ++i)
                if (!scope[i]->isAssigned() && fixToValue(scope[i], value, level))
                    touched.push_back(scope[i]);
    }
    return false;
}

bool ConstraintAtLeastK::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    auto tmp = varToScopePosition.find(cur);
    assert(tmp != varToScopePosition.end());
    unsigned p = tmp->second;

    if (sparse[p] <= (unsigned)limit || cur->isValidValue(value))
        return false;

    // we search for another sentinel
    for (unsigned i = limit + 1; i < listSize; ++i)
        if (scope[dense[i]]->isValidValue(value)) {
            int a = sparse[p];
            int b = sparse[dense[i]];
            dense[a] = dense[i];
            dense[b] = p;
            sparse[p] = b;
            sparse[dense[i]] = a;
            return false;
        }

    // no new sentinel found ; we have to assign all k remaining variables
    for (int i = limit; i >= 0; i--)
        if (dense[i] != p && !scope[dense[i]]->isAssignedTo(value)) {
            if (fixToValue(scope[dense[i]], value, level))
                touched.push_back(scope[dense[i]]);
            else
                return true;
        }

    return false;
}

bool ConstraintAtLeast1::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    if (cur == sentinel1) {
        if (!sentinel1->isValidValue(value)) {
            Variable* sentinel = findAnotherSentinel();
            if (sentinel)
                sentinel1 = sentinel;
            else if (!sentinel2->isAssignedTo(value)) {
                if (fixToValue(sentinel2, value, level))
                    touched.push_back(sentinel2);
                else
                    return true;
            }
        }
    } else if (cur == sentinel2) {
        if (!sentinel2->isValidValue(value)) {
            Variable* sentinel = findAnotherSentinel();
            if (sentinel)
                sentinel2 = sentinel;
            else if (!sentinel1->isAssignedTo(value)) {
                if (fixToValue(sentinel1, value, level))
                    touched.push_back(sentinel1);
                else
                    return true;
            }
        }
    }
    return false;
}

bool ConstraintAtMostK::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    if (!cur->isAssignedTo(value))
        return false;

    int cnt = 0;
    for (auto var : scope)
        if (var->isAssignedTo(value) && ++cnt > k)
            return true; // inconsistency detected

    if (cnt == k)
        for (auto var : scope)
            if (!var->isAssignedTo(value)) {
                int before = var->domainCurSize;
                var->removeValue(value, level, this);
                if (before != var->domainCurSize)
                    touched.push_back(var);
            }
    return false;
}

bool ConstraintAtMost1::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    if (!cur->isAssignedTo(value))
        return false;

    for (auto var : scope)
        if (var != cur) {
            int before = var->domainCurSize;
            if (var->removeValue(value, level, this))
                return true;
            if (before != var->domainCurSize)
                touched.push_back(var);
        }

    return false;
}
