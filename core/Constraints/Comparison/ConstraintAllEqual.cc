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
 * @CreateTime: Nov 24, 2017 1:51 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 6:00 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintAllEqual.hh"

using namespace std;

bool ConstraintAllEqual::propagate(int level, Variable* cur, vector<Variable*>& touched)
{
    if (cur->isAssigned()) {
        // we remove the assigned value from the domain of the variables
        for (size_t i = 0; i < listSize; ++i) {
            if (scope[i]->isAssigned()) {
                if (cur->getOnlyValue() == scope[i]->getOnlyValue())
                    continue;
                else
                    return true;
            }

            if (fixToValue(scope[i], cur->getOnlyValue(), level))
                touched.push_back(scope[i]);
        }
    } else {
        set<indVpLocal> toDel;
        // we collect the set of dropped (indexes of) values (since the last call) over all variables
        for (size_t i = 0; i < listSize; ++i) {
            // cerr << scope[i]->domainCurSize << ":" << scope[i]->domainInitSize << "/" << frontier[i] << endl;
            for (int j = scope[i]->domainCurSize; j < frontier[i]; ++j) {
                assert(scope[i]->indDomLocalToIndVPLocal(j) < scope[0]->domainInitSize);
                toDel.insert(scope[i]->indDomLocalToIndVPLocal(j));
            }
        }

        // we remove these dropped (indexes of) values from the domain of all variables
        for (size_t i = 0; i < listSize; ++i) {
            Variable* var = scope[i];
            int before = var->domainCurSize;
            if (var->removeSetOfIndVpLocal(toDel, level, this))
                return true;
            if (before != var->domainCurSize)
                touched.push_back(var);

            // we record the frontier of dropped (indexes of) values for the next call
            frontier[i] = var->domainCurSize;
        }
    }
    return false;
}