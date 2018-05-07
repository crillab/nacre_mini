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
 * @CreateTime: Oct 12, 2017 6:02 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTPRIMITIVE_H_
#define CONSTRAINTPRIMITIVE_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"

class ConstraintPrimitive : public Constraint {
protected:
    Variable* x;
    Variable* y;
    int k;

public:
    ConstraintPrimitive(std::string n, Variable* varX, Variable* varY, int intK)
        : Constraint(n, std::vector<Variable*>({ varX, varY }))
        , x(varX)
        , y(varY)
        , k(intK){};

    virtual bool propagate(int level, Variable* cur, std::vector<Variable*>& touched) = 0;
};

class ConstraintPrimitiveEqual : public ConstraintPrimitive {
public:
    ConstraintPrimitiveEqual(std::string n, Variable* varX, Variable* varY, int intK)
        : ConstraintPrimitive(n, varX, varY, intK){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintPrimitiveLessThan : public ConstraintPrimitive {
public:
    ConstraintPrimitiveLessThan(std::string n, Variable* varX, Variable* varY, int intK)
        : ConstraintPrimitive(n, varX, varY, intK){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintPrimitiveLessEqual : public ConstraintPrimitive {
public:
    ConstraintPrimitiveLessEqual(std::string n, Variable* varX, Variable* varY, int intK)
        : ConstraintPrimitive(n, varX, varY, intK){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintPrimitiveNotEqual : public ConstraintPrimitive {
public:
    ConstraintPrimitiveNotEqual(std::string n, Variable* varX, Variable* varY, int intK)
        : ConstraintPrimitive(n, varX, varY, intK){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintDisjunctive : public ConstraintPrimitive {
    int ky;

    bool filterDomain(Variable* cur, int level, int lbValue, int ubValue);

public:
    ConstraintDisjunctive(std::string n, Variable* varX, Variable* varY, int kx, int intKy)
        : ConstraintPrimitive(n, varX, varY, kx)
        , ky(intKy){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};
#endif // CONSTRAINTPRIMITIVE_H_
