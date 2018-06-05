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

#include "ConflictAnalysisSolver.hh"
#include "InfoSystem.hh"
#include "Options.hh"

using namespace std;

int ConflictAnalysisSolver::search(int zeroing)
{
    VP* vp = nullptr;
    int curConflict = 0;
    vector<unsigned> learnt_clause;
    int bjLevel = -1;
    int uniqVar = -1;
    AnalysisConsistencyHandler* AChandler = static_cast<AnalysisConsistencyHandler*>(cspAC);

    if (Options::Verbose != verbose::no) {
        if (Options::optRst == rstTypes::noRst)
            fprintf(stderr, "c | %6s | %7s | %8s | %7s | %10s | %10s |\n", "numtry", "numConf", "nbDec", "time", "Mem(Mb)", "#uLit");
        else
            fprintf(stderr, "c | %6s | %7s | %8s | %8s | %8s | %10s |\n", "numtry", "numConf", "nbDec", "time", "#NG", "#uLit");
    }

    for (;;) {
        Constraint* c = AChandler->propagate();

        if (c != nullptr) { // conflict
            AChandler->cleanQueue();
            curConflict++;
            Stats::conflict++;
            if (Options::optRst == rstTypes::noRst && Options::Verbose != verbose::no && !(Stats::conflict % 10000))
                fprintf(stderr, "c | %6llu | %7d | %8llu | %7.1lf | %10.3lf | %10llu |\n",
                    Stats::run, curConflict, Stats::nbDecisions, InfoSystem::timeElapsed(), (double)InfoSystem::getMemUsed() / 1000, Stats::unit);

            if (!AChandler->getDecisionLevel()) // conflict at level 0 => UNSAT
                return 1;

            learnt_clause.clear();
            learnt_clause = confAnalysis->analyse(c, AChandler->getDecisionLevel(), bjLevel, uniqVar);

            if (learnt_clause.empty())
                return 1;

            AChandler->cancelUntil(bjLevel);

            Constraint* ref = nullptr;
            if (learnt_clause.size() != 1)
                ref = manager->getNgHandler()->addClause(learnt_clause, uniqVar);

            VP& vp = Variable::varProps[learnt_clause[0] >> 1];

            if (learnt_clause.size() == 1)
                Stats::unit++;

            if ((learnt_clause[0] & 1))
                vp.toVar->removeAt(vp.posInVar, AChandler->getDecisionLevel(), ref);
            else
                vp.toVar->assignAt(vp.posInVar, AChandler->getDecisionLevel(), ref);
            Stats::nbSupp++;

            AChandler->addToQueue(vp.toVar);

            RefClause::claDecayActivity();

        } else if (curConflict >= rst.GetNbConflictAllowed()) { // restart
            if (Options::Verbose != verbose::no)
                fprintf(stderr, "c | %6llu | %7d | %8llu | %8.1lf | %8llu | %10llu |\n",
                    Stats::run, curConflict, Stats::nbDecisions, InfoSystem::elapsed_seconds(), Stats::nbNoGoods, Stats::unit);

            rst.Inc();
            curConflict = 0;
            AChandler->cancelUntil(0);
        } else { // new decision level
            if (vp && vp->toVar->isAssigned())
                vp->toVar->lastPushed = false;

            Variable* v = PickVariable();

            if (v) {
                indDomLocal idl = PickValue(v);
                vp = &(v->getVarPropFromLocalDomInd(idl));

                vp->lastAssigned = Options::assign;
                AChandler->pushNewVariable(Options::assign, idl, v, true, true);
                if (Options::Verbose == verbose::medium)
                    cerr << "(" << AChandler->getDecisionLevel() << ") " << vp->toVar->getName()
                         << (vp->lastAssigned ? "=" : "!=") << vp->toVar->getVarPropFromLocalDomInd(vp->posInVar).val
                         << " ~ " << vp->toVar->indDomLocalToIndVP(vp->posInVar) << endl;
                continue;
            }

            for (auto var : problem->getVariables())
                if (!var->isAssigned())
                    var->assignAt(0, AChandler->getDecisionLevel());
            return 0;
        }
    }

    return 2;
} // search
