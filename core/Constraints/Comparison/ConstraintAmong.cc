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
 * @CreateTime: Mar 19, 2018 2:10 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintAmong.hh"

using namespace std;

bool ConstraintAmong::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    int nGuaranteedVars = 0;
    int nPossibleVars = 0;
    limit = -1;

    for (size_t i = 0; i < listSize; ++i) {
        Variable& v = *scope[i];
        bool atLeastOnePresentValue = false, atLeastOneAbsentValue = false;
        for (int j = 0; j < v.domainCurSize && (!atLeastOnePresentValue || !atLeastOneAbsentValue); ++j) {
            bool b = isPresentInValues(v.LocalDomIndToVal(i));
            atLeastOnePresentValue = atLeastOnePresentValue || b;
            atLeastOneAbsentValue = atLeastOneAbsentValue || !b;
        }
        if (atLeastOnePresentValue) {
            nPossibleVars++;
            if (!atLeastOneAbsentValue && ++nGuaranteedVars > k)
                return true; // inconsistency detected
            if (atLeastOneAbsentValue) {
                int e = sparse[i];
                if (e > limit) {
                    limit++;
                    int f = dense[limit];
                    dense[e] = f;
                    dense[limit] = i;
                    sparse[i] = limit;
                    sparse[f] = e;
                }
            }
        }
    }

    if (nGuaranteedVars == k) {
        for (int i = limit; i >= 0; i--) {
            Variable* v = scope[dense[i]];

            int saveSize = v->domainCurSize;
            for (int j = saveSize; j >= 0; --j)
                if (isPresentInValues(v->LocalDomIndToVal(j)))
                    deleteValue(v, j, level);

            if (saveSize != v->domainCurSize)
                touched.push_back(v);

            assert(v->domainCurSize); // no inconsistency possible
        }
        return false;
    }

    if (nPossibleVars < k)
        return true;

    if (nPossibleVars == k) {
        for (int i = limit; i >= 0; i--) {
            Variable* v = scope[dense[i]];

            int saveSize = v->domainCurSize;
            for (int j = saveSize; j >= 0; --j)
                if (!isPresentInValues(v->LocalDomIndToVal(j)))
                    deleteValue(v, j, level);

            if (saveSize != v->domainCurSize)
                touched.push_back(v);

            assert(v->domainCurSize); // no inconsistency possible
        }
    }

    return false;
}
