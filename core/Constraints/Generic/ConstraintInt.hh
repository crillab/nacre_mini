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
 * @CreateTime: Oct 12, 2017 5:46 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTINT_H_
#define CONSTRAINTINT_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"
#include "XCSP3Tree.h"

class ConstraintInt : public Constraint {
private:
    XCSP3Core::Tree* t;
    std::vector<std::map<std::string, int>>* vpRes;

    std::map<std::string, int> tuple;
    std::vector<int> cptRet;

    bool computeNewTuple(int var, int value, int nValidTuplesMax);
    bool stillOK(std::map<std::string, int> tuple);

public:
    ConstraintInt(std::string n, std::vector<Variable*> vars, XCSP3Core::Tree* tree)
        : Constraint(n, vars)
        , t(tree)
    {
        sort(scope.begin(), scope.end(), [](const auto& lhs, const auto& rhs) {
            return lhs->domainInitSize < rhs->domainInitSize;
        });

        vpRes = new std::vector<std::map<std::string, int>>[listSize];
        for (size_t i = 0; i < listSize; ++i)
            vpRes[i] = std::vector<std::map<std::string, int>>(scope[i]->domainInitSize);

        cptRet.resize(listSize);
    }

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

#endif // CONSTRAINTINT_H_
