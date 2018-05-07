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
 * @CreateTime: Nov 27, 2017 9:38 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTSUMSCALARBOOLEAN_H_
#define CONSTRAINTSUMSCALARBOOLEAN_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"

class ConstraintSumScalarBoolean : public Constraint {
protected:
    long min;
    long max;
    int half;

    int* set01vs1;
    int limitSet;

    inline void filter(int level, std::vector<Variable*>& touched, int valToRm)
    {
        for (int i = limitSet; i >= 0; i--) {
            int j = set01vs1[i];
            assert((scope[j]->domainCurSize == 2 && scope[half + j]->isAssignedTo(1)) || (scope[half + j]->domainCurSize == 2 && scope[j]->isAssignedTo(1)));

            if (scope[j]->domainCurSize == 2) {
                scope[j]->removeValue(valToRm, level, this);
                touched.push_back(scope[j]);
            } else {
                scope[half + j]->removeValue(valToRm, level, this);
                touched.push_back(scope[half + j]);
            }
        }
    }

public:
    ConstraintSumScalarBoolean(std::string n, std::vector<Variable*> vars) //Coeffs are at the end of scope
        : Constraint(n, vars)
        , min(0)
        , max(0)
        , half(vars.size() / 2)
    {
        assert(vars.size() % 2 == 0);
        set01vs1 = new int[half];
        for (int i = 0; i < half; ++i)
            set01vs1[i] = i;
        limitSet = -1;
    };

    virtual bool propagate(int level, Variable* cur, std::vector<Variable*>& touched) = 0;
};

class SumScalarBooleanLE : public ConstraintSumScalarBoolean {
private:
    int limit;

    inline void recomputeBounds()
    {
        min = max = 0;
        limitSet = -1;

        for (int i = 0; i < half; i++) {
            Variable* v1 = scope[i];
            Variable* v2 = scope[i + half];

            if (v1->getUpperBoundVal() && v2->getUpperBoundVal()) { // if one 1 is missing nothing to do because the product is necessarily 0
                max++;
                if (v1->getLowerBoundVal() && v2->getLowerBoundVal()) //if v1 and v2 assigned to 1
                    min++;
                else if (v1->isAssigned() || v2->isAssigned())
                    set01vs1[++limitSet] = i; // we add i iff we have (0,1) versus 1 (or equivalently 1 versus (0,1)) ; the only way to filter here
            }
        }
    }

public:
    SumScalarBooleanLE(std::string n, std::vector<Variable*> vars, int lim)
        : ConstraintSumScalarBoolean(n, vars)
        , limit(lim){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class SumScalarBooleanEQ : public ConstraintSumScalarBoolean {
private:
    int limit;

    int* set01vs01;
    int limitSetEq;

    inline void recomputeBounds()
    {
        min = max = 0;
        limitSet = limitSetEq = -1;

        for (int i = 0; i < half; i++) {
            Variable* v1 = scope[i];
            Variable* v2 = scope[i + half];

            if (v1->getUpperBoundVal() && v2->getUpperBoundVal()) { // if one 1 is missing nothing to do because the product is necessarily 0
                max++;
                if (v1->getLowerBoundVal() && v2->getLowerBoundVal())
                    min++;
                else if (v1->isAssigned() || v2->isAssigned())
                    set01vs1[++limitSet] = i; // we add i iff we have (0,1) versus 1 (or equivalently 1 versus (0,1)) ; the only way to filter here

                else
                    set01vs01[++limitSetEq] = i; // we add i because we have (0,1) versus (0,1)
            }
        }
    }

public:
    SumScalarBooleanEQ(std::string n, std::vector<Variable*> vars, int lim)
        : ConstraintSumScalarBoolean(n, vars)
        , limit(lim)
    {
        set01vs01 = new int[half];
        for (int i = 0; i < half; ++i)
            set01vs01[i] = i;
        limitSetEq = -1;
    };

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class SumScalarBooleanLEVar : public ConstraintSumScalarBoolean {
private:
    Variable* limit;

    inline void recomputeBounds()
    {
        min = max = 0;
        limitSet = -1;

        for (int i = 0; i < half; i++) {
            Variable* v1 = scope[i];
            Variable* v2 = scope[i + half];

            if (v1->getUpperBoundVal() && v2->getUpperBoundVal()) { // if one 1 is missing nothing to do because the product is necessarily 0
                max++;
                if (v1->getLowerBoundVal() && v2->getLowerBoundVal())
                    min++;
                else if (v1->isAssigned() || v2->isAssigned())
                    set01vs1[++limitSet] = i; // we add i iff we have (0,1) versus 1 (or equivalently 1 versus (0,1)) ; the only way to filter here
            }
        }
    }

public:
    SumScalarBooleanLEVar(std::string n, std::vector<Variable*> vars, Variable* lim)
        : ConstraintSumScalarBoolean(n, vars)
        , limit(lim){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

#endif // CONSTRAINTSUMSCALARBOOLEAN_H_
