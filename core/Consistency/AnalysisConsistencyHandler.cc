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
 * @CreateTime: Oct 2, 2017 3:08 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 6:00 PM
 * @Description: Implementation of ConsistencyHandler.hh
 */

#include "AnalysisConsistencyHandler.hh"
#include "ConflictAnalysis.hh"
#include "ConsistencyHandler.hh"
#include "Constraint.hh"
#include "GeneralizedNoGoodsManager.hh"
#include "InfoSystem.hh"
#include "Options.hh"
#include "Variable.hh"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <functional>
#include <iostream>
#include <strings.h>
#include <vector>

using namespace std;

Constraint* AnalysisConsistencyHandler::propagate(bool incConflict)
{
    touched.clear();
    int dontStop = true;

    for (; dontStop;) {
        for (; !propagateQueue.empty();) {
            Variable* vTmp = pickVariable();

            for (auto c : vTmp->getConsts()) {
                if (c->getStamp() < vTmp->ctrStamp) {
                    Stats::nbPropCalls++;

                    if (c->propagate(decisionLevel, vTmp, touched)) {
                        if (incConflict)
                            increase(c);
                        return c;
                    }

                    for (auto v : touched)
                        addToQueue(v);
                    touched.clear();

                    c->updateStamp();
                }
            }
        }

        assert(genHandler != nullptr);

        dontStop = false;

        for (; !clQueue.empty();) {
            Variable* vTmp = clQueue.back();
            clQueue.pop_back();
            vTmp->clStamp = 0;

            int res = genHandler->propagate(decisionLevel, vTmp, touched);
            if (res != -1)
                return genHandler->refs[res];

            if (!touched.empty()) {
                dontStop = true;
                for (auto v : touched)
                    addToQueue(v);
                touched.clear();
            }
        }
    }

    return nullptr;
} // propagate
