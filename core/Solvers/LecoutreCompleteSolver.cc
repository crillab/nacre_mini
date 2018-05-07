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
 * @CreateTime: Dec 15, 2017 1:11 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:56 PM
 * @Description: Modify Here, Please 
 */

#include "LecoutreCompleteSolver.hh"
#include "InfoSystem.hh"
#include "LecoutreNoGoodsManager.hh"

using namespace std;

int LecoutreCompleteSolver::showFinal(int res)
{
    if (Options::Verbose != verbose::no && manager)
        cerr << *manager << endl;
    return CompleteSolver::showFinal(res);
}

int LecoutreCompleteSolver::search(int zeroing)
{
    Variable* curVar = nullptr;
    vector<indVp> propagateStackVariable;
    vector<vector<indVp>> nogoodStackVariable(problem->getVariables().size());
    int curConflict = 0;

    manager = new LecoutreNoGoodsManager(cspAC);
    cspAC->setNgHandler(manager->getNgHandler());

    set<vector<indVp>> solsToBlock;

    vector<Variable*> vecVar = problem->getVariables();

    if (Options::Verbose != verbose::no)
        fprintf(stderr, "c | %6s | %7s | %8s | %8s | %8s |\n", "numtry", "numConf", "nbDec", "time", "#NG");
    for (;;) {
        if (cspAC->propagate()) { // conflict
            cspAC->cleanQueue();
            curConflict++;
            Stats::conflict++;

            if (!cspAC->getDecisionLevel())
                return 1;

            assert(propagateStackVariable.size());
            VP& cur = Variable::varProps[propagateStackVariable.back()];
            propagateStackVariable.pop_back();

            cspAC->cancelUntil(cspAC->getDecisionLevel() - 1);

            nogoodStackVariable[cspAC->getDecisionLevel() + 1].clear();
            if (cspAC->getDecisionLevel() > 1)
                nogoodStackVariable[cspAC->getDecisionLevel()].push_back(cur.toVar->indDomLocalToIndVP(0));

            cspAC->pushNewVariable(!Options::assign, cur.posInVar, cur.toVar, false, false);
            if (Options::Verbose >= verbose::medium)
                cerr << "BT TO " << cspAC->getDecisionLevel() << " rem " << cur.toVar->getVarPropFromLocalDomInd(cur.posInVar).val
                     << " from " << cur.toVar->getName() << endl;
        } else if (curConflict >= rst.GetNbConflictAllowed()) { // restart
            if (Options::Verbose != verbose::no)
                fprintf(stderr, "c | %6llu | %7d | %8llu | %8.1lf |", Stats::run++, curConflict, Stats::nbDecisions, InfoSystem::elapsed_seconds());

            rst.Inc();
            curConflict = 0;
            cspAC->cancelUntil(0);

            if (Options::countSols == countTypes::no)
                manager->restart(nogoodStackVariable, propagateStackVariable);
            else {
                cerr << endl;
                for (auto sol : solsToBlock)
                    manager->blockSolution(sol);
                solsToBlock.clear();
            }

            for (size_t i = 0, stop = vecVar.size(); i < stop; ++i)
                nogoodStackVariable[i].clear();
            propagateStackVariable.clear();
        } else { // new decision level
            if (curVar && curVar->isAssigned())
                curVar->lastPushed = false;

            if ((curVar = PickVariable())) {
                cspAC->pushNewVariable(Options::assign, PickValue(curVar), curVar, true, true);
                propagateStackVariable.push_back(curVar->indDomLocalToIndVP(0));
                if (Options::Verbose >= verbose::medium)
                    cerr << "(" << cspAC->getDecisionLevel() << ") Assigned " << curVar->getName()
                         << " to " << curVar->getVarPropFromLocalDomInd(0).val << " ~ " << curVar->indDomLocalToIndVP(0) << endl;
                continue;
            }

            if (Options::countSols == countTypes::no) {
                for (auto var : vecVar) {
                    if (!var->isAssigned())
                        var->assignAt(0, cspAC->getDecisionLevel());
                }
                return 0;
            }

            //________________________________________________
            vector<indVp> sol;
            for (size_t k = 0, stop = vecVar.size(); k < stop; ++k)
                sol.push_back(vecVar[k]->indDomLocalToIndVP(0));

            if (solsToBlock.insert(sol).second)
                Stats::nbSols++;

            if (Options::Verbose >= verbose::high) {
                cout << "v <instantiation> <list> ";
                for (auto var : problem->getVariables())
                    cout << var->getName() << " ";
                cout << "</list> <values>";
                for (auto var : problem->getVariables()) {
                    /* if (var->useless)
                        cout << "* ";
                    else */
                        cout << var->getVarPropFromLocalDomInd(0).val << " ";
                }
                cout << "</values> </instantiation>" << endl
                     << endl;
            }
            //_________________________________________

            if (!cspAC->getDecisionLevel())
                return 1;

            cspAC->cancelUntil(cspAC->getDecisionLevel() - 1);

            VP& cur = Variable::varProps[propagateStackVariable.back()];
            propagateStackVariable.pop_back();
            nogoodStackVariable[cspAC->getDecisionLevel()].clear();

            if (Options::Verbose >= verbose::medium)
                cerr << "sol found removing val " << cur.toVar->getVarPropFromLocalDomInd(cur.posInVar).val
                     << " from " << cur.toVar->getName() << endl;

            curVar = Variable::varProps[propagateStackVariable.back()].toVar;
            cspAC->pushNewVariable(!Options::assign, cur.posInVar, cur.toVar, false, false);
        }
    }

    return 2;
}
