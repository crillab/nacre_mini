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
 * @CreateTime: Nov 8, 2017 9:39 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintExtremum.hh"

using namespace std;

bool ConstraintMaximum::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    int maxFirst = numeric_limits<int>::min();
    int maxLast = numeric_limits<int>::min();

    for (auto var : scope) {
        if (var->getLowerBoundVal() > maxFirst)
            maxFirst = var->getLowerBoundVal();
        if (var->getUpperBoundVal() > maxLast)
            maxLast = var->getUpperBoundVal();
    }

    // Filtering the domain of the extremum (max) variable
    int before = ext->domainCurSize;
    if (ext->removeValuesAbove(maxLast, level, this) || ext->removeValuesUnder(maxFirst, level, this))
        return true;
    if (before != ext->domainCurSize)
        touched.push_back(ext);

    for (int a = ext->getUpperBoundLocalInd(); a >= 0; --a) {
        VP& curVp = ext->getVarPropFromLocalInd(a);
        if (!ext->isValidPosInVar(curVp.posInVar))
            continue;
        if (scope[sentinels[a]]->getIndFromValue(curVp.val) == -1) {
            int s = findSentinelFor(curVp.posInVar);
            if (s != -1)
                sentinels[a] = s;
            else {
                before = ext->domainCurSize;
                if (deleteValue(ext, curVp.posInVar, level))
                    return true;
                if (touched.empty() && before != ext->domainCurSize)
                    touched.push_back(ext);
            }
        }
    }

    // Filtering the domains of variables in the vector
    int lastMax = ext->getUpperBoundVal();
    for (auto var : scope) {
        before = var->domainCurSize;
        if (var->removeValuesAbove(lastMax, level, this))
            return true;
        if (before != var->domainCurSize)
            touched.push_back(var);
    }

    // Possibly filtering the domain of the sentinel from the last value of the Max variable
    int sentinel = sentinels[ext->getUpperBoundLocalInd()];
    int valLimit = computeLimitForSentinel(sentinel);
    if (valLimit == lastMax)
        return false; // because another sentinel exists

    Variable* domSentinel = scope[sentinel];
    for (int a = domSentinel->getUpperBoundLocalInd(); a >= 0; --a) {
        VP& curVp = domSentinel->getVarPropFromLocalInd(a);
        if (!domSentinel->isValidPosInVar(curVp.posInVar) || curVp.val == lastMax)
            continue;
        if (curVp.val <= valLimit)
            break;
        if (ext->getIndFromValue(curVp.val) == -1) {
            before = domSentinel->domainCurSize;
            if (deleteValue(domSentinel, curVp.posInVar, level))
                return true;
            if (before != domSentinel->domainCurSize)
                touched.push_back(domSentinel);
        }
    }
    return false;
}

bool ConstraintMinimum::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    int minFirst = numeric_limits<int>::max();
    int minLast = numeric_limits<int>::max();

    for (auto var : scope) {
        if (var->getLowerBoundVal() < minFirst)
            minFirst = var->getLowerBoundVal();
        if (var->getUpperBoundVal() < minLast)
            minLast = var->getUpperBoundVal();
    }

    // Filtering the domain of the extremum (min) variable
    int before = ext->domainCurSize;
    if (ext->removeValuesAbove(minLast, level, this) || ext->removeValuesUnder(minFirst, level, this))
        return true;
    if (before != ext->domainCurSize)
        touched.push_back(ext);

    for (int a = ext->getLowerBoundInd() - ext->domainStart; a < ext->domainInitSize; ++a) {
        VP& curVp = ext->getVarPropFromLocalInd(a);
        if (!ext->isValidPosInVar(curVp.posInVar))
            continue;
        if (scope[sentinels[a]]->getIndFromValue(curVp.val) == -1) {
            int s = findSentinelFor(curVp.posInVar);
            if (s != -1)
                sentinels[a] = s;
            else {
                before = ext->domainCurSize;
                if (deleteValue(ext, curVp.posInVar, level))
                    return true;
                if (touched.empty() && before != ext->domainCurSize)
                    touched.push_back(ext);
            }
        }
    }

    // Filtering the domains of variables in the vector
    int firstMin = ext->getLowerBoundVal();
    for (auto var : scope) {
        before = var->domainCurSize;
        if (var->removeValuesUnder(firstMin, level, this))
            return true;
        if (before != var->domainCurSize)
            touched.push_back(var);
    }

    // Possibly filtering the domain of the sentinel from the last value of the Max variable
    int sentinel = sentinels[ext->getLowerBoundInd() - ext->domainStart];
    int valLimit = computeLimitForSentinel(sentinel);
    if (valLimit == firstMin)
        return false; // because another sentinel exists

    Variable* domSentinel = scope[sentinel];
    for (int a = ext->getLowerBoundInd() - ext->domainStart + 1; a < ext->domainInitSize; ++a) {
        VP& curVp = domSentinel->getVarPropFromLocalInd(a);
        if (!domSentinel->isValidPosInVar(curVp.posInVar))
            continue;
        if (curVp.val <= valLimit)
            break;
        if (ext->getIndFromValue(curVp.val) == -1) {
            before = domSentinel->domainCurSize;
            if (deleteValue(domSentinel, curVp.posInVar, level))
                return true;
            if (before != domSentinel->domainCurSize)
                touched.push_back(domSentinel);
        }
    }

    return false;
}
