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
 * @CreateTime: Dec 22, 2017 11:17 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTEXTBINARY_H_
#define CONSTRAINTEXTBINARY_H_

#include "Constraint.hh"
#include "Variable.hh"
#include "XCSP3Constants.h"

class ConstraintExtBinary : public Constraint {
protected:
    std::vector<int> resx; //residue for x
    std::vector<int> resy; //residue for y
    Variable *x, *y;
    std::vector<std::vector<indVp>> tuples;

    std::vector<std::vector<bool>> matrix;
    bool isSupport;

public:
    ConstraintExtBinary(std::string n, std::vector<Variable*> vars, std::vector<std::vector<indVp>>& tuples, bool support)
        : Constraint(n, vars)
        , x(vars[0])
        , y(vars[1])
        , tuples(tuples)
        , isSupport(support)
    {
        int xSz = x->domainInitSize;
        int ySz = y->domainInitSize;

        for (int i = 0; i < xSz; ++i)
            matrix.push_back(std::vector<bool>(ySz, false));

        for (auto t : tuples) {
            if (t[0] == STAR) {
                assert(t[1] != STAR);
                for (int i = 0; i < xSz; ++i)
                    matrix[i][y->indVpToIndVpLocal(t[1])] = true;
            } else if (t[1] == STAR) {
                for (int i = 0; i < ySz; ++i)
                    matrix[x->indVpToIndVpLocal(t[0])][i] = true;
            } else
                matrix[x->indVpToIndVpLocal(t[0])][y->indVpToIndVpLocal(t[1])] = true;
        }

        resx.resize(xSz, -1);
        resy.resize(ySz, -1);
    };

    bool init()
    {
        if (isSupport)
            for (size_t j = 0; j < listSize; ++j)
                if (scope[j]->keepOnlyVarProp(tuples, j, 0, this))
                    return true;

        return false;
    };

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

#endif // CONSTRAINTEXTBINARY_H_
