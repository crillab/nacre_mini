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
 * @CreateTime: Jan 3, 2018 1:56 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#include "ConflictAnalysis.hh"
#include "Constraint.hh"
#include "Options.hh"
#include "Variable.hh"

using namespace std;

unsigned long long ConflictAnalysis::gblCaStamp = 1;

void ConflictAnalysis::printHeaderAnalyse(Constraint* ctr)
{
    if ((unsigned)ctr->getId() >= Stats::saveNbConstraints) {
        cerr << "fail from " << ctr->getId() << "(" << ctr->getName() << ") ";
        vector<unsigned>& curCl = RefClause::dbClauses->clauses[static_cast<RefClause*>(ctr)->ref];
        for (size_t i = 0; i < curCl.size(); ++i)
            cerr << ((curCl[i] & 1) ? "¬" : "") << (curCl[i] >> 1) << " ";
    } else {
        cerr << "fail from ctr " << ctr->getId() << "(" << ctr->getName() << ") " << ctr->getUsableScopeSize() << "/" << ctr->getScope().size() << ". scp : ";
        for (auto v : ctr->getScope()) {
            cerr << v->getName() << "(" << v->domainStart << "-" << v->domainStart + v->domainInitSize - 1 << ") ";
        }
    }
    cerr << endl;
}

void ConflictAnalysis::buildStack(vector<unsigned>& conflictCl, int level)
{
    for (auto lit : conflictCl) {
        indVp ivp = lit >> 1;
        Expl& cur = Variable::vpExpl[ivp];

        assert(Variable::varProps[ivp].state != UNDEF);

        if (!cur.level) // level 0 decision, does not belong to explanation
            continue; // conflictCl isn't ordered by level, so we continue instead of breaking
        else if (cur.level == level) {
            if (Options::Verbose >= verbose::high)
                cerr << "is+" << ((lit & 1) ? "¬" : "") << ivp << " ";
            stack.insert(make_pair(lit, &cur)); // we put the same sign as the conflictCl because it's handled by getConflict method
        } else {
            assert(!Variable::varProps[ivp].marked);
            Variable::varProps[ivp].marked = true;
            if (Options::Verbose >= verbose::high)
                cerr << "ic+" << ((lit & 1) ? "¬" : "") << ivp << " ";
            cl.push_back(lit); // we put the same sign as the conflictCl because it's handled by getConflict method
        }
    }
}

vector<unsigned> ConflictAnalysis::analyse(Constraint* ctr, int level, int& bjLevel, int& uniqVar)
{
    conflictCl.clear();
    cl.clear();
    stack.clear();
    bjLevel = 0;
    cl.push_back(0); // assertive literal position
    ConflictAnalysis::gblCaStamp++;

    if (Options::Verbose >= verbose::high)
        printHeaderAnalyse(ctr);

    // We gather the conflict clause
    ctr->getConflict(conflictCl);

    // We build the stack from conflictCl
    buildStack(conflictCl, level);

    if (stack.empty() && !level)
        return vector<unsigned>(); // UNS because the conflict come from level 0 decisions

    assert(!stack.empty());
    Variable* curVarDec = nullptr;
    // We handle every current level lit until only one remains. The last one is useful to determine the assertive lit
    while (stack.size() > 1) {
        std::pair<unsigned, Expl*> litExpl = pop();

        unsigned lit = get<0>(litExpl);
        Expl& cur = *get<1>(litExpl);
        Variable* var = Variable::varProps[lit >> 1].toVar;

        if (Options::Verbose >= verbose::high)
            cerr << "s-" << ((lit & 1) ? "¬" : "") << (lit >> 1) << " ";

        if (cur.ctr != nullptr)
            cur.ctr->getReason(lit, cur, level, stack, cl);
        else {
            if (Options::Verbose >= verbose::high)
                cerr << "dn ";
            curVarDec = var;
            break;
        }
    }

    // Handle assertive literal by checking if the last literal var was already found
    std::pair<unsigned, Expl*> litExpl = *(stack.rbegin());
    unsigned lit = get<0>(litExpl);
    Variable* var = Variable::varProps[lit >> 1].toVar;
    if (curVarDec != nullptr) {
        assert(curVarDec == var);
        if (Options::Verbose >= verbose::high)
            cerr << "s-" << ((lit & 1) ? "¬" : "") << (lit >> 1) << " ";
        lit = (static_cast<unsigned>(var->indDomLocalToIndVP(0)) << 1) | 1;
    }
    cl[0] = lit; // insert the assertive literal at the begining of the clause

    if (Options::Verbose >= verbose::high)
        cerr << ((lit & 1) ? "¬" : "") << (lit >> 1) << endl;

    for (auto c : cl)
        Variable::varProps[c >> 1].marked = false;

    set<int> uniqueVar;
    size_t save = 0;
    // Compute the backjump level and swap the literal to the second position of the clause
    // And compute the number of uniqVars in the clause for heuristics
    uniqueVar.insert(Variable::varProps[cl[0] >> 1].toVar->getId());
    for (size_t i = 1, stop = cl.size(); i < stop; i++) {
        uniqueVar.insert(Variable::varProps[cl[i] >> 1].toVar->getId());
        int curBjLvl = Variable::vpExpl[cl[i] >> 1].level;
        if (curBjLvl > bjLevel) {
            bjLevel = curBjLvl;
            save = i;
        }
    }

    if (save)
        swap(cl[1], cl[save]);
    uniqVar = uniqueVar.size();

    return cl;
}