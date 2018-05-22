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
 * @CreateTime: Oct 2, 2017 11:17 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Class used to represent the current problem, contains variables, constraints and backtrackable constraints 
 */

#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Constraint.hh"
#include "Options.hh"
#include "Stats.hh"
#include "Variable.hh"

#include <vector>

/**
 * Problem
 */

class Problem {
private:
    std::vector<Variable*> variables;
    std::vector<Constraint*> constraints;
    std::vector<Constraint*> backtrackableConstraints;

public:
    /// Getter for the problem variables
    inline std::vector<Variable*>& getVariables() { return variables; }

    /// Add a variable to the problem
    inline void addVariable(Variable* var) { variables.push_back(var); }

    /// Getter for the problem constaints
    inline std::vector<Constraint*>& getConstraints() { return constraints; }

    /// Setter for the backtrackable constraints of the problem
    inline void setBTConstraints(std::vector<Constraint*> vecCont) { backtrackableConstraints = vecCont; }

    /// Setter for the constraints of the problem and register them in their scope
    inline void setConstraints(std::vector<Constraint*> vecCont)
    {
        constraints = vecCont;
        for (auto c : constraints)
            for (auto v : c->getScope())
                v->addConsts(c);
        Stats::saveNbConstraints = Stats::nbConstraints;
    }

    inline void init(std::vector<Constraint*>& vecCont, std::vector<Constraint*>& vecBTCont)
    {
        for (auto v : variables)
            v->init();
        Stats::nbVarProps = Stats::saveNbVarProps = Variable::varProps.size();

        setConstraints(vecCont);
        setBTConstraints(vecBTCont);
    }

    /// Perform backtrack to @level on all variables and backtrackable contraints
    void backtrack(int level)
    {
        for (size_t i = 0, stop = Variable::vpExpl.size(); i < stop; ++i)
            if (Variable::vpExpl[i].level > level)
                Variable::vpExpl[i].unlock();

        for (auto v : variables)
            v->backtrackToLevel(level);
        for (auto c : backtrackableConstraints)
            c->backtrack(level);
    }
};

#endif // PROBLEM_H_
