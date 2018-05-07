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

#ifndef CONSISTENCYHANDLER_H_
#define CONSISTENCYHANDLER_H_

#include "Clauses.hh"
#include "ClausesPositive.hh"
#include "ConflictAnalysis.hh"
#include "Problem.hh"
#include "Stats.hh"
#include "Variable.hh"

#include <stack>

class IncNGNoGoodsManager;

class ConsistencyHandler {
protected:
    Problem* problem;

    std::vector<Variable*> propagateQueue;
    std::vector<Variable*> touched;

    int decisionLevel;
    unsigned long long gblStamp;

    Clauses* genHandler;
    ClausesPositive* ngHandler;
    IncNGNoGoodsManager* incNgHandler;

    virtual Variable* pickVariable();

    inline void increase(Constraint* c)
    {
        for (auto v : c->getScope())
            v->score++;
    }

public:
    /// Constructor
    ConsistencyHandler(Problem* p)
        : problem(p)
        , decisionLevel(0)
        , gblStamp(0)
        , genHandler(nullptr)
        , ngHandler(nullptr)
        , incNgHandler(nullptr){};

    virtual ~ConsistencyHandler(){};

    unsigned long long propagateStamp = 1;

    /// Initialize the problem (variables and constraints) and perform AC on it. Return true if an inconsistency is detected.
    bool init();

    /// Backtrack the problem to the level @lev
    void cancelUntil(int lev);

    inline void setGenHandler(Clauses* cl)
    {
        genHandler = cl;
        RefClause::dbClauses = cl;
    }
    inline void setNgHandler(ClausesPositive* cl) { ngHandler = cl; }
    inline void setIncNgHandler(IncNGNoGoodsManager* cl) { incNgHandler = cl; }

    /**
     * If @sign is true assign the variable @var to the @v th value of his sparse set
     * Else (@sign is false) remove the value form @var domain.
     * 
     * @param[in] sign if set to true assign, else remove
     * @param[in] v the local index of the domain of @var
     * @param[in] var the variable where the change is performed
     * @param[in] count if true increase @nbDecisions if @newDec is set to true
     * @param[in] newDec if true increase @decisionLevel
     */
    void pushNewVariable(bool sign, indDomLocal v, Variable* var, bool count, bool newDec);

    /**
     * Perform AC on the propagation queue by variable minDom and with stamps.
     * 
     * @param[in] incConflict if set to false it doesn't increase the score of constraints and variables
     * 
     * @return true if an inconsistency is detected.
     */
    bool propagate(bool incConflict = true);

    /// Empty the propagation queue
    virtual inline void cleanQueue()
    {
        propagateQueue.clear();
        propagateStamp++;
    }

    ///Getter for the current decision level
    inline int getDecisionLevel() { return decisionLevel; }

    /// Setter for the decision level
    inline void setDecisionLevel(int dl)
    {
        assert(dl >= decisionLevel);
        decisionLevel = dl;
    }

    inline void addToQueue(Variable* v)
    {
        v->ctrStamp = ++gblStamp;
        if (propagateStamp != v->propagateStamp) {
            v->propagateStamp = propagateStamp;
            propagateQueue.push_back(v);
        }
    }
};
#endif // CONSISTENCYHANDLER_H_
