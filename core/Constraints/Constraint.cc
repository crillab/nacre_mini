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
 * @CreateTime: Sep 27, 2017 5:13 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#include "Constraint.hh"
#include "ConflictAnalysis.hh"

#include <vector>

using namespace std;

Clauses* RefClause::dbClauses = nullptr;
Domain* Domain::domCtr = new Domain();

double RefClause::cla_inc = 1;
double RefClause::clause_decay = 0.999;

void Constraint::getConflict(vector<unsigned>& cl)
{
    for (size_t k = 0, stop = scope.size(); k < stop; ++k) {
        Variable* curVar = scope[k];

        if (curVar->caStamp == ConflictAnalysis::gblCaStamp)
            continue;
        curVar->caStamp = ConflictAnalysis::gblCaStamp;

        for (int i = curVar->domainCurSize; i < curVar->domainAfterInit; ++i) {
            indVp tmp = curVar->indDomLocalToIndVP(i);
            if (!Variable::vpExpl[tmp].level)
                break; // we break since the domain is ordered by decreasing levels
            cl.push_back(tmp << 1); // positive because deleted value, see clauses.hh
        }
    }
}

void Constraint::getReason(unsigned lit, Expl& litExpl, int level, set<std::pair<unsigned, Expl*>, varOrderCmp>& stack, vector<unsigned>& cl)
{
    for (size_t j = 0, stop = scope.size(); j < stop; ++j) {
        Variable* curVar = scope[j];

        if ((curVar->caStamp == ConflictAnalysis::gblCaStamp)
            || curVar->domainAfterInit == 1 || (!(lit & 1) && curVar == Variable::varProps[lit >> 1].toVar))
            continue;
        curVar->caStamp = ConflictAnalysis::gblCaStamp;

        for (int i = curVar->domainCurSize; i < curVar->domainAfterInit; ++i) {
            indVp ivp = curVar->indDomLocalToIndVP(i);
            Expl& curExpl = Variable::vpExpl[ivp];

            if (litExpl.order < curExpl.order)
                continue;
            if (!curExpl.level)
                break; // we break since the domain is ordered by decreasing levels
            else if (curExpl.level == level) {
                if (Options::Verbose >= verbose::high)
                    cerr << "cs+" << ivp << " ";
                stack.insert(make_pair(ivp << 1, &curExpl)); // positive because deleted value, see clauses.hh
            } else {
                if (Variable::varProps[ivp].marked)
                    continue;
                Variable::varProps[ivp].marked = true;
                if (Options::Verbose >= verbose::high)
                    cerr << "cc+" << ivp << " ";
                cl.push_back(ivp << 1); // positive because deleted value, see clauses.hh
            }
        }
    }
} // getReason

void RefClause::getConflict(vector<unsigned>& cl)
{
    cl = RefClause::dbClauses->clauses[ref];

    claBumpActivity();

    uniqueVar.clear();
    for (size_t i = 0, stop = cl.size(); i < stop; ++i) {
        Variable& v = *Variable::varProps[cl[i] >> 1].toVar;

        if (!uniqueVar.insert(v.getId()).second)
            continue;
        v.score++;
    }
}

void RefClause::claBumpActivity()
{
    if ((activity += cla_inc) > 1e20) {
        for (size_t i = 0, stop = dbClauses->clauses.size(); i < stop; i++)
            RefClause::dbClauses->refs[i]->activity *= 1e-20;
        cla_inc *= 1e-20;
    }
} // claBumpActivity

void RefClause::getReason(unsigned lit, Expl& litExpl, int level, set<std::pair<unsigned, Expl*>, varOrderCmp>& stack, vector<unsigned>& cl)
{
    vector<unsigned>& curCl = RefClause::dbClauses->clauses[ref];

    claBumpActivity();
    unsigned ivp = lit >> 1;
    Variable* litVar = Variable::varProps[ivp].toVar;

    for (size_t i = 0, stop = curCl.size(); i < stop; ++i) {
        unsigned curIvp = curCl[i] >> 1;
        if (curIvp == ivp)
            continue;

        Expl& curExpl = Variable::vpExpl[curIvp];
        if (!(lit & 1) && Variable::varProps[curIvp].toVar == litVar)
            continue;
        if (litExpl.order < curExpl.order || curExpl.order == 0 || !curExpl.level)
            continue;
        if (curExpl.level == level) {
            if (Options::Verbose >= verbose::high)
                cerr << "ns+" << ((curCl[i] & 1) ? "¬" : "") << (curIvp) << " ";
            stack.insert(make_pair(curCl[i], &curExpl));
        } else {
            if (Variable::varProps[curCl[i] >> 1].marked)
                continue;
            Variable::varProps[curCl[i] >> 1].marked = true;
            if (Options::Verbose >= verbose::high)
                cerr << "nc+" << ((curCl[i] & 1) ? "¬" : "") << (curIvp) << " ";
            cl.push_back(curCl[i]);
        }
    }
}

void Domain::getReason(unsigned lit, Expl& litExpl, int level, set<std::pair<unsigned, Expl*>, varOrderCmp>& stack, vector<unsigned>& cl)
{
    assert(lit & 1);
    Variable* curVar = Variable::varProps[lit >> 1].toVar;

    for (int i = 1; i < curVar->domainAfterInit; ++i) {
        indVp ivp = curVar->indDomLocalToIndVP(i);
        Expl& curExpl = Variable::vpExpl[ivp];

        if (!curExpl.level) {
            break; // we break since the domain is ordered by decreasing levels
        } else if (curExpl.level == level) {
            if (Options::Verbose >= verbose::high)
                cerr << "ds+" << ivp << " ";
            stack.insert(make_pair(ivp << 1, &curExpl)); // positive because deleted value, see clauses.hh
        } else {
            if (Variable::varProps[ivp].marked)
                continue;
            Variable::varProps[ivp].marked = true;
            if (Options::Verbose >= verbose::high)
                cerr << "dc+" << ivp << " ";
            cl.push_back(ivp << 1); // positive because deleted value, see clauses.hh
        }
    }
}
