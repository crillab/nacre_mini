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
 * @CreateTime: Dec 22, 2017 11:17 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#include "ConstraintExtBinary.hh"

using namespace std;

bool ConstraintExtBinary::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    if (x->isAssigned() && y->isAssigned())
        return (isSupport ^ matrix[x->indDomLocalToIndVPLocal(0)][y->indDomLocalToIndVPLocal(0)]);

    int saveSize = x->domainCurSize;
    if (saveSize > 1)
        for (int i = saveSize - 1; i >= 0; --i) {
            int idvx = x->indDomLocalToIndVPLocal(i);
            if (resx[idvx] != -1 && y->isValidLocalVpInd(resx[idvx]))
                continue;

            bool found = false;
            for (int j = y->domainCurSize - 1; j >= 0; --j) {
                int idvy = y->indDomLocalToIndVPLocal(j);
                if (!(isSupport ^ matrix[idvx][idvy])) {
                    resx[idvx] = idvy;
                    found = true;
                    break;
                }
            }

            if (!found && deleteValue(x, i, level))
                return true;
        }

    if (x->domainCurSize != saveSize)
        touched.push_back(x);

    saveSize = y->domainCurSize;
    if (saveSize > 1)
        for (int i = saveSize - 1; i >= 0; --i) {
            int idvy = y->indDomLocalToIndVPLocal(i);
            if (resy[idvy] != -1 && x->isValidLocalVpInd(resy[idvy]))
                continue;

            bool found = false;
            for (int j = x->domainCurSize - 1; j >= 0; --j) {
                int idvx = x->indDomLocalToIndVPLocal(j);
                if (!(isSupport ^ matrix[idvx][idvy])) {
                    resy[idvy] = idvx;
                    found = true;
                    break;
                }
            }

            if (!found && deleteValue(y, i, level))
                return true;
        }

    if (y->domainCurSize != saveSize)
        touched.push_back(y);

    if (x->isAssigned() && y->isAssigned())
        return (isSupport ^ matrix[x->indDomLocalToIndVPLocal(0)][y->indDomLocalToIndVPLocal(0)]);
    return false;
}
