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
 * @CreateTime: Nov 22, 2017 2:27 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 6:00 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTALLEQUAL_H_
#define CONSTRAINTALLEQUAL_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"

class ConstraintAllEqual : public Constraint {
protected:
    indVpLocal* frontier;

public:
    ConstraintAllEqual(std::string n, std::vector<Variable*> vars)
        : Constraint(n, vars)
    {
        frontier = new int[listSize];
        for (size_t i = 0; i < listSize; ++i)
            frontier[i] = scope[i]->domainCurSize;
    }

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
    void backtrack(int level)
    {
        for (size_t i = 0; i < listSize; ++i)
            frontier[i] = scope[i]->domainCurSize;
    }
};

#endif // CONSTRAINTALLEQUAL_H_