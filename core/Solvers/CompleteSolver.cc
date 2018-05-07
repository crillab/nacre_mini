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
 * @CreateTime: Dec 15, 2017 2:38 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:56 PM
 * @Description: Modify Here, Please
 */

#include "CompleteSolver.hh"
#include "Constraint.hh"
#include "InfoSystem.hh"
#include "Options.hh"
#include "Solver.hh"
#include "Variable.hh"

#include <cassert>
#include <cstring>
#include <iostream>
#include <math.h>
#include <vector>

using namespace std;

int CompleteSolver::search(int zeroing)
{
    Variable* v = nullptr;
    vector<VP*> propagateStackVariable;
    int curConflict = 0;

    InfoSystem::elapsed_seconds();

    if (Options::Verbose != verbose::no)
        fprintf(stderr, "c | %6s | %7s | %8s | %7s | %10s |\n", "numtry", "numConf", "nbDec", "time", "Mem(Mb)");
    for (;;) {
        if (cspAC->propagate()) { // conflict
            cspAC->cleanQueue();
            curConflict++;
            Stats::conflict++;
            if (Options::optRst == rstTypes::noRst && Options::Verbose != verbose::no && !(Stats::conflict % 10000))
                fprintf(stderr, "c | %6llu | %7d | %8llu | %7.1lf | %10.3lf |\n",
                    Stats::run, curConflict, Stats::nbDecisions, InfoSystem::timeElapsed(), (double)InfoSystem::getMemUsed() / 1000);

            if (cspAC->getDecisionLevel() == zeroing)
                return 1;

            cspAC->cancelUntil(cspAC->getDecisionLevel() - 1);

            assert(propagateStackVariable.size());

            VP cur = *(propagateStackVariable.back());
            propagateStackVariable.pop_back();

            cspAC->pushNewVariable(!Options::assign, cur.posInVar, cur.toVar, false, false);
            if (Options::Verbose >= verbose::medium)
                cerr << "BT TO " << cspAC->getDecisionLevel() << (Options::assign ? " rem " : " fix ") << cur.toVar->getVarPropFromLocalDomInd(cur.posInVar).val
                     << (Options::assign ? " from " : " to ") << cur.toVar->getName() << endl;

        } else if (curConflict >= rst.GetNbConflictAllowed()) { // restart
            propagateStackVariable.clear();

            rst.Inc();

            if (Options::Verbose != verbose::no)
                fprintf(stderr, "c | %6llu | %7d | %8llu | %7.1lf | %10.3lf |\n",
                    Stats::run++, curConflict, Stats::nbDecisions, InfoSystem::elapsed_seconds(), (double)InfoSystem::getMemUsed() / 1000);
            curConflict = 0;

            cspAC->cancelUntil(zeroing);

        } else { // new decision level
            if (v && v->isAssigned())
                v->lastPushed = false;

            if ((v = PickVariable())) {
                indDomLocal iDom = PickValue(v);
                propagateStackVariable.push_back(&(v->getVarPropFromLocalDomInd(iDom)));
                if (Options::Verbose >= verbose::medium)
                    cerr << "(" << cspAC->getDecisionLevel() << (Options::assign ? ") Assigned " : ") Refuted ") << v->getName()
                         << (Options::assign ? " to " : " from ") << v->getVarPropFromLocalDomInd(iDom).val << endl;
                cspAC->pushNewVariable(Options::assign, iDom, v, true, true);
                continue;
            }

            if (Options::countSols == countTypes::no) {
                for (auto var : problem->getVariables()) {
                    if (!var->isAssigned())
                        var->assignAt(0, cspAC->getDecisionLevel());
                }
                return 0;
            }

            Stats::nbSols++;

            if (Options::Verbose >= verbose::high) {
                cout << "v <instantiation> <list> ";
                for (auto var : problem->getVariables())
                    cout << var->getName() << " ";
                cout << "</list> <values>";
                for (auto var : problem->getVariables())
                    cout << var->getVarPropFromLocalDomInd(0).val << " ";
                cout << "</values> </instantiation>" << endl
                     << endl;
            }

            cspAC->cleanQueue();
            if (cspAC->getDecisionLevel() == zeroing)
                return 1;

            cspAC->cancelUntil(cspAC->getDecisionLevel() - 1);

            assert(propagateStackVariable.size());

            VP cur = *(propagateStackVariable.back());
            propagateStackVariable.pop_back();
            if (Options::Verbose >= verbose::medium)
                cerr << "sol found removing val " << cur.toVar->getVarPropFromLocalDomInd(cur.posInVar).val
                     << " from " << cur.toVar->getName() << endl;
            cspAC->pushNewVariable(!Options::assign, cur.posInVar, cur.toVar, false, false);
        }
    }

    return 2;
} // search

indDomLocal CompleteSolver::PickValue(Variable* cur)
{
    if (Options::saving && cur->isValidLastChoice())
        return cur->lastChoiceToDomLocalInd();
    if (Options::valueSelection == valueSelectionTypes::min)
        return cur->getLowerBoundLocalDomInd();
    if (Options::valueSelection == valueSelectionTypes::zero)
        return 0;
    if (Options::valueSelection == valueSelectionTypes::max)
        return cur->getUpperBoundLocalDomInd();
    if (Options::valueSelection == valueSelectionTypes::last)
        return cur->domainCurSize - 1;
    if (Options::valueSelection == valueSelectionTypes::vrand) {
        return rand() % cur->domainCurSize;
    }

    throw runtime_error("Something went wrong (Options compatibility maybe!?)");
} // PickVariable

/**
   Méthode permettant de choisir la prochaine variable à affecter.
   @return une variable, ou nullptr si il n'y a plus de variable à affecter.
 */
Variable* CompleteSolver::PickVariable()
{
    if (Options::varHeuristic == variableHeuristic::domwdeg)
        return heuristicDomWDeg();
    if (Options::varHeuristic == variableHeuristic::dom)
        return heuristicDom();
    if (Options::varHeuristic == variableHeuristic::domdeg)
        return heuristicDomDeg();

    throw runtime_error("Something went wrong (Options compatibility maybe!?)");
} // PickVariable

Variable* CompleteSolver::heuristicDom()
{
    Variable* retV = nullptr;
    int best = numeric_limits<int>::max();
    int weight;

    for (auto vTmp : problem->getVariables()) {
        if (vTmp->isAssigned())
            continue;

        weight = vTmp->domainCurSize;
        if (weight < best) {
            best = weight;
            retV = vTmp;
        }
    }

    return retV;
}

Variable* CompleteSolver::heuristicDomDeg()
{
    Variable* retV = nullptr;
    double best = numeric_limits<double>::max();
    double weight;

    for (auto vTmp : problem->getVariables()) {
        if (vTmp->isAssigned())
            continue;

        if (!vTmp->getConsts().size())
            weight = numeric_limits<double>::max();
        else
            weight = (double)vTmp->domainCurSize / (double)vTmp->getConsts().size();

        if (weight <= best) {
            best = weight;
            retV = vTmp;
        }
    }

    return retV;
}

Variable* CompleteSolver::heuristicDomWDeg()
{
    Variable* retV = nullptr;
    double best = numeric_limits<double>::max();
    double weight;

    for (auto vTmp : problem->getVariables()) {
        if (vTmp->isAssigned())
            continue;

        if (!vTmp->getConsts().size())
            weight = numeric_limits<double>::max();
        else
            weight = (double)vTmp->domainCurSize / (double)(vTmp->score /* * vTmp->getConsts().size() */);

        if (weight <= best) {
            best = weight;
            retV = vTmp;
        }
    }

    return retV;
}

/**
   Allow to print the final message.
 */
int CompleteSolver::showFinal(int res)
{
    int ret = 0;

    cout << "c" << endl
         << "c # time = " << InfoSystem::timeElapsed() << endl
         << "c # Dec = " << Stats::nbDecisions << endl
         << "c # Propagate Calls = " << Stats::nbPropCalls << endl
         << "c # assignAt Calls = " << Stats::assignCall << endl
         << "c # removeAt Calls = " << Stats::rmCall << endl
         << "c # Run = " << Stats::run << endl
         << "c # Conflicts = " << Stats::conflict << endl;

    uint64_t mem_used = InfoSystem::getMemUsed();
    if (mem_used != 0)
        cout << "c # Memory used = " << (double)mem_used / 1000 << " Mb" << endl;

    if (Options::countSols == countTypes::all) {
        cout << "c # Sols = " << Stats::nbSols << endl;
        exit(40);
    }

    switch (res) {
    case 0:
        cout << "s SATISFIABLE" << endl
             << "c" << endl;
        cout << "v <instantiation>" << endl
             << "v     <list> ";
        for (auto var : problem->getVariables())
            cout << var->getName() << " ";
        cout << "</list>" << endl
             << "v     <values> ";
        for (auto var : problem->getVariables())
            cout << var->getVarPropFromLocalDomInd(0).val << " ";
        cout << "</values>" << endl
             << "v </instantiation>" << endl;
        ret = 10;
        break;
    case 1:
        cout << "s UNSATISFIABLE" << endl;
        ret = 20;
        break;
    case 2:
        cout << "s UNKNOWN" << endl;
        ret = 30;
        break;
    }

    return ret;
} // showFinal
