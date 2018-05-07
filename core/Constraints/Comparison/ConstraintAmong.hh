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
 * @CreateTime: Mar 19, 2018 2:10 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTAMONG_H_
#define CONSTRAINTAMONG_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"

class ConstraintAmong : public Constraint {
private:
    std::vector<int> values;
    int k;

    unsigned* sparse;
    unsigned* dense;
    int limit;

    inline bool isPresentInValues(int value) { return std::find(values.begin(), values.end(), value) != values.end(); }

public:
    ConstraintAmong(std::string n, std::vector<Variable*> vars, std::vector<int> vals, int lim)
        : Constraint(n, vars)
        , values(vals)
        , k(lim)
        , limit(-1)
    {
        dense = new unsigned[vars.size()];
        sparse = new unsigned[vars.size()];
        for (size_t i = 0; i < vars.size(); ++i) {
            dense[i] = i;
            sparse[i] = i;
        }
    };

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

#endif // CONSTRAINTAMONG_H_