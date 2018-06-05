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

#include "ConsistencyHandler.hh"
#include "ConflictAnalysis.hh"
#include "Constraint.hh"
#include "IncNGNoGoodsManager.hh"
#include "InfoSystem.hh"
#include "Stats.hh"
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

bool ConsistencyHandler::init()
{
    cout << "c NACRE 1.0.3" << endl;

    cleanQueue();

    if (Options::Verbose != verbose::no)
        cout << "c Init constraints...";
    for (auto c : problem->getConstraints())
        if (c->init())
            return true;
    if (Options::Verbose != verbose::no)
        cout << "Done" << endl
             << "c Arc consistency is performing...";

    for (auto v : problem->getVariables()) {
        if (v->getConsts().empty() || v->domainInitSize == 1) {
            v->useless = true;
            Stats::uselessVars++;
            v->assignAt(0, 0);
        } else
            addToQueue(v);
    }

    if (propagate())
        return true;

    if (Options::Verbose != verbose::no)
        cout << "Done" << endl
             << "c Initialization done in " << InfoSystem::timeElapsed() << " seconds" << endl;

    if (Options::Verbose != verbose::no)
        Stats::print_after_init();

    return false;
} // init

Variable* ConsistencyHandler::pickVariable()
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
    propagateQueue.erase(toRem);
    vTmp->propagateStamp = 0;
    return vTmp;
}

void ConsistencyHandler::pushNewVariable(bool sigTrue, indDomLocal posV, Variable* var, bool count, bool newDec)
{
    assert(propagateQueue.empty());

    if (newDec) {
        if (count)
            Stats::nbDecisions++;
        decisionLevel++;
    }

    if (sigTrue)
        var->assignAt(posV, decisionLevel);
    else
        var->removeAt(posV, decisionLevel);

    addToQueue(var);
} // pushNewVariable

void ConsistencyHandler::cancelUntil(int lev)
{
    if (decisionLevel == lev)
        return;
    assert(decisionLevel > lev);

    if (Options::Verbose >= verbose::medium)
        cerr << "Backjump to " << lev << " from " << decisionLevel << endl;

    decisionLevel = lev;
    problem->backtrack(lev);
} // cancelUntil

bool ConsistencyHandler::propagate(bool incConflict)
{
    touched.clear();
    for (; !propagateQueue.empty();) {
        Variable* vTmp = pickVariable();

        if (genHandler && genHandler->propagate(decisionLevel, vTmp, touched) != -1)
            return true;
        for (auto v : touched)
            addToQueue(v);
        touched.clear();

        if (vTmp->isAssigned()) {
            if (ngHandler && ngHandler->propagate(decisionLevel, vTmp, touched) != -1)
                return true;
            for (auto v : touched)
                addToQueue(v);
            touched.clear();

            if (incNgHandler && incNgHandler->revise(vTmp, touched))
                return true;
            for (auto v : touched)
                addToQueue(v);
            touched.clear();
        }

        for (auto c : vTmp->getConsts()) {
            if (c->getStamp() < vTmp->ctrStamp) {
                Stats::nbPropCalls++;

                if (c->propagate(decisionLevel, vTmp, touched)) {
                    if (incConflict)
                        increase(c);

                    return true;
                }

                for (auto v : touched)
                    addToQueue(v);
                touched.clear();

                c->updateStamp();
            }
        }
    }
    return false;
} // propagate
