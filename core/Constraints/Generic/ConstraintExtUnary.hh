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
 * @CreateTime: Sep 28, 2017 8:37 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTEXTUNARY_H_
#define CONSTRAINTEXTUNARY_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"

class ConstraintExtUnary : public Constraint {
private:
    std::vector<int> tuples;
    Variable* onlyVar;
    bool isSupport;

public:
    ConstraintExtUnary(std::string n, Variable* var, std::vector<int>& t, bool support)
        : Constraint(n, std::vector<Variable*>({ var }))
        , tuples(t)
        , onlyVar(var)
        , isSupport(support){};

    ConstraintExtUnary(std::string n, Variable* var, int t, bool support)
        : Constraint(n, std::vector<Variable*>({ var }))
        , tuples(vector<int>({ t }))
        , onlyVar(var)
        , isSupport(support){};

    bool init()
    {
        if (isSupport)
            return onlyVar->keepOnlyValues(tuples, 0, this);
        else
            return onlyVar->removeValues(tuples, 0, this);
    }

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched) { return false; }
};

#endif // CONSTRAINTEXTUNARY_H_
