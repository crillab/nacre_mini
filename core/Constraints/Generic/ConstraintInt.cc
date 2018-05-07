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
 * @CreateTime: Oct 17, 2017 4:40 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintInt.hh"

using namespace std;

bool ConstraintInt::computeNewTuple(int var, int value, int nValidTuplesMax)
{
    for (size_t k = 0; k < listSize; ++k)
        cptRet[k] = 0;
    cptRet[var] = value;

    // Compute new tuple
    for (int i = 0; i < nValidTuplesMax; ++i) {
        tuple.clear();
        for (size_t k = 0; k < listSize; ++k)
            tuple[scope[k]->getName()] = scope[k]->getVarPropFromLocalDomInd(cptRet[k]).val;

        if (t->evaluate(tuple)) {
            for (size_t k = var; k < listSize; ++k) {
                indVp cur = scope[k]->getIndFromValue(tuple[scope[k]->getName()]);
                assert(cur != -1);
                vpRes[k][cur - scope[k]->domainStart] = tuple;
            }

            return true;
        }

        //update cpt
        for (size_t k = 0; k < listSize; ++k) {
            if ((int)k == var)
                continue;
            cptRet[k]++;
            if (cptRet[k] < scope[k]->domainCurSize)
                break;
            cptRet[k] = 0;
        }
    }

    return false;
}

bool ConstraintInt::stillOK(map<string, int> tuple)
{
    for (size_t k = 0; k < listSize; ++k)
        if (!scope[k]->isValidValue(tuple[scope[k]->getName()]))
            return false;
    return true;
}

bool ConstraintInt::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    int nValidTuples = 1;
    for (size_t i = 0; i < listSize && nValidTuples < 10000; ++i)
        nValidTuples *= scope[i]->domainCurSize;

    if (nValidTuples >= 10000)
        return false;

    for (size_t i = 0; i < listSize; ++i) {
        Variable* vTmp = scope[i];
        int saveSize = vTmp->domainCurSize;
        int curTuplesByVal = nValidTuples / saveSize;
        // Check for each value if the tuple is still valid, compute a new one if needed
        for (int j = saveSize - 1; j >= 0; j--) {
            indVpLocal curVal = vTmp->indDomLocalToIndVPLocal(j);
            if ((vpRes[i][curVal].empty() || !stillOK(vpRes[i][curVal])) && !computeNewTuple(i, j, curTuplesByVal)) {
                if (deleteValue(vTmp, j, level))
                    return true;

                if (saveSize != vTmp->domainCurSize)
                    nValidTuples -= curTuplesByVal;
            }
        }

        if (saveSize != vTmp->domainCurSize)
            touched.push_back(vTmp);
    }

    return false;
}