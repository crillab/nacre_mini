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
 * @CreateTime: Jan 3, 2018 4:36 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#include "Clauses.hh"
#include "Constraint.hh"
#include "GeneralizedNoGoodsManager.hh"
#include "Options.hh"
#include "Variable.hh"

#include <numeric>

using namespace std;

int Clauses::checkWatch(unsigned lit, int level, vector<Variable*>& touched)
{
    assert(lit < (Variable::varProps.size() << 1));
    vector<unsigned>& idxClauses = watched[lit];

    unsigned* ptr_j = &idxClauses.data()[0];
    unsigned* ptr_i = &idxClauses.data()[0];
    unsigned* ptr_e = &idxClauses.data()[idxClauses.size()];

    for (; ptr_i != ptr_e; ++ptr_i) {
        vector<unsigned>& cl = clauses[*ptr_i];

        // Putting lit at pos 1
        if (lit == cl[0])
            swap(cl[0], cl[1]);

        //check the satisfiability of the clause (by looking at cl[0])
        if ((Variable::varProps[cl[0] >> 1].state ^ sign(cl[0])) == POS) {
            *ptr_j++ = *ptr_i;
            continue;
        }

        for (size_t another = 2, stop = cl.size(); another < stop; ++another) {
            if ((Variable::varProps[cl[another] >> 1].state ^ sign(cl[another])) != NEG) { // we have another watch
                swap(cl[1], cl[another]);
                watched[cl[1]].push_back(*ptr_i);
                goto nextTurn;
            }
        }

        // There is no new watch => its a unit clause
        // Checking also watch at pos 0 to know if the last watch is assigned too
        if (sign(cl[0])) { // negative sign so we must remove the value
            Variable& zV = *Variable::varProps[cl[0] >> 1].toVar;
            assert(zV.isValidVpInd(cl[0] >> 1)); // the value must be alive in the domain otherwise the clause was already satisfied
            if (zV.isAssigned()) { // if the var is already assigned to this value => conflict
                Stats::nbConflict++;
                int idx = *ptr_i;
                for (; ptr_i != ptr_e;)
                    *ptr_j++ = *ptr_i++;
                idxClauses.resize(ptr_j - (unsigned*)&idxClauses.data()[0]);
                return idx;
            }

            Stats::nbSupp++;
            zV.removeAt(Variable::varProps[cl[0] >> 1].posInVar, level, refs[*ptr_i]);
            touched.push_back(&zV);
        } else { //positive sign, we must assign the value
            Variable& zV = *Variable::varProps[cl[0] >> 1].toVar;
            if ((Variable::varProps[cl[0] >> 1].state ^ sign(cl[0])) == NEG) { //if the value was removed => conflict
                Stats::nbConflict++;
                int idx = *ptr_i;
                for (; ptr_i != ptr_e;)
                    *ptr_j++ = *ptr_i++;
                idxClauses.resize(ptr_j - (unsigned*)&idxClauses.data()[0]);
                return idx;
            }

            assert(!zV.isAssigned()); // the value is not alone in the domain otherwise the clause was already satisfied
            Stats::nbSupp += zV.domainCurSize - 1;

            zV.assignAt(Variable::varProps[cl[0] >> 1].posInVar, level, refs[*ptr_i]);
            touched.push_back(&zV);
        }

        *ptr_j++ = *ptr_i; //lit continue to be a watched value for backtracking
    nextTurn:;
    }

    idxClauses.resize(ptr_j - (unsigned*)&idxClauses.data()[0]);
    return -1;
}

/**
   Progation process.

   @param[in] level,  
   @param[in] cur, 
   @param[in] touched, 
 */
int Clauses::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    int ret;

    if (cur->isAssigned()) {
        // If the variable is assigned we need to find new watch for the clauses where not(Lit) appears
        ret = checkWatch(cur->indDomLocalToIndVP(0) << 1 | 1, level, touched);
        if (ret != -1)
            return ret;
    }

    int saveDomainCurSize = cur->domainCurSize;
    for (int vi = cur->domainCurSize, stop = cur->getPuSizeLast(); vi < stop; vi++) {
        // For all of the values removed since the last call we need to find new watch for the clauses where Lit appears
        ret = checkWatch(cur->indDomLocalToIndVP(vi) << 1, level, touched);
        if (ret != -1)
            return ret;
    }

    cur->setPuSizeLast(saveDomainCurSize, level);
    return -1;
} // propagate

/**
   Add the clause in the constraint and watch the two first litterals.

   @param[in] cl, a set of 'literals'
*/
Constraint* Clauses::addClause(vector<unsigned>& cl, int uniqVar)
{
    assert(cl.size() > 1);

    // If the DB is full, call reduce
    if (Options::reduceDBOpt && nClausesInDB >= clauseDbSize)
        reduceDB();

    Stats::nbNoGoods++;
    nClausesInDB++;
    if (freeSpaces.empty()) { // In case of no free spaces, add the new clause at the end and init structs
        watched[cl[0]].push_back(clauses.size());
        watched[cl[1]].push_back(clauses.size());
        refs.push_back(new RefClause(clauses.size()));
        clauses.push_back(cl);
        uniqVars.push_back(uniqVar);

        if (uniqVar == 2 && Options::keepBinary) {
            Stats::binary++;
            clState.push_back(3);
        } else
            clState.push_back(2);

        refs.back()->claBumpActivity();
        return refs.back();
    } else { // otherwise reinit the structs already existing
        assert(Options::reduceDBOpt);
        int id = freeSpaces.back();
        freeSpaces.pop_back();

        watched[cl[0]].push_back(id);
        watched[cl[1]].push_back(id);
        clauses[id] = cl;
        uniqVars[id] = uniqVar;

        if (uniqVar == 2 && Options::keepBinary) {
            Stats::binary++;
            clState[id] = 3;
        } else
            clState[id] = 2;

        refs[id]->activity = .0;
        refs[id]->claBumpActivity();
        return refs[id];
    }
} // addClause

bool Clauses::delClause(unsigned id)
{
    if (refs[id]->isReason) // Do not delete a clause that is reason !
        return false;

    nClausesInDB--;

    // Unsubscribe watched literals
    auto& w0 = watched[clauses[id][0]];
    for (auto it = w0.begin(); it != w0.end(); ++it)
        if (*it == id) {
            w0.erase(it);
            break;
        }

    auto& w1 = watched[clauses[id][1]];
    for (auto it = w1.begin(); it != w1.end(); ++it)
        if (*it == id) {
            w1.erase(it);
            break;
        }

    freeSpaces.push_back(id);
    return true;
}

void Clauses::reduceDB()
{
    assert(Options::reduceDBOpt);
    assert(freeSpaces.empty());

    // create the vector v of integer and put a sequence of integer begining at 0 to represent clause IDs
    std::vector<int> v(clauses.size());
    std::iota(v.begin(), v.end(), 0);

    // Sort it by increasing number of unique variables, break ties by bigger activity
    std::sort(v.begin(), v.end(), [this](int x, int y) {
        if (uniqVars[x] != uniqVars[y])
            return uniqVars[x] < uniqVars[y];
        else
            return refs[x]->activity > refs[y]->activity;
    });

    double extra_lim = RefClause::cla_inc / nClausesInDB;
    // Remove the last half of clauses (if they aren't reasons -> handled in delClause)
    for (size_t i = v.size() >> 1, stop = v.size(); i < stop; ++i)
        if (clState[v[i]] != 3 && !refs[v[i]]->isReason && refs[v[i]]->activity < extra_lim)
            delClause(v[i]);

    clauseDbSize += 500; // grow the clauses DB
}
