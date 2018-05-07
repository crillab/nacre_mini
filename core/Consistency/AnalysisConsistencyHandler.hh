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
 * @CreateTime: Oct 2, 2017 3:05 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 6:00 PM
 * @Description: Handle arc-consistency
 */

#ifndef ANALYSISCONSISTENCYHANDLER_H_
#define ANALYSISCONSISTENCYHANDLER_H_

#include "AnalysisConsistencyHandler.hh"
#include "Clauses.hh"
#include "ConflictAnalysis.hh"
#include "ConsistencyHandler.hh"
#include "Options.hh"
#include "Problem.hh"
#include "Variable.hh"

#include <stack>

class AnalysisConsistencyHandler : public ConsistencyHandler {
private:
    std::vector<Variable*> clQueue;

    Variable* pickVariable() override
    {
        assert(!propagateQueue.empty());

        Variable* vTmp = nullptr;
        int domSize = std::numeric_limits<int>::max();

        auto toRem = propagateQueue.end();
        for (auto it = propagateQueue.begin(); it != propagateQueue.end(); ++it) {
            int curDom = (*it)->domainCurSize;
            if (curDom < domSize) {
                domSize = curDom;
                vTmp = *it;
                toRem = it;
            }
        }

        assert(vTmp);
        assert(toRem != propagateQueue.end());

        if (vTmp->clStamp != clStamp) {
            vTmp->clStamp = clStamp;
            clQueue.push_back(vTmp);
        }

        propagateQueue.erase(toRem);
        vTmp->propagateStamp = 0;
        return vTmp;
    }

    unsigned long long clStamp = 1;

public:
    /// Constructor
    AnalysisConsistencyHandler(Problem* p)
        : ConsistencyHandler(p){};

    ~AnalysisConsistencyHandler(){};

    inline void cleanQueue() override
    {
        ConsistencyHandler::cleanQueue();
        clQueue.clear();
        clStamp++;
    }

    /**
     * Perform AC on the propagation queue by variable minDom and with stamps.
     * 
     * @param[in] incConflict if set to false it doesn't increase the score of constraints and variables
     * 
     * @return true if an inconsistency is detected.
     */
    Constraint* propagate(bool incConflict = true);
};
#endif // ANALYSISCONSISTENCYHANDLER_H_
