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
 * @CreateTime: Oct 27, 2017 9:48 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTSUM_H_
#define CONSTRAINTSUM_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"

class ConstraintSum : public Constraint {
protected:
    long min;
    long max;
    long limit;

    inline void recomputeBounds()
    {
        min = max = 0;
        for (size_t i = 0; i < listSize; i++) {
            min += scope[i]->getLowerBoundVal();
            max += scope[i]->getUpperBoundVal();
        }
    }

public:
    ConstraintSum(std::string n, std::vector<Variable*> vars, int lim)
        : Constraint(n, vars)
        , min(0)
        , max(0)
        , limit(lim){};

    virtual bool propagate(int level, Variable* cur, std::vector<Variable*>& touched) = 0;
};

class ConstraintSumEQ : public ConstraintSum {
public:
    ConstraintSumEQ(std::string n, std::vector<Variable*> vars, int lim)
        : ConstraintSum(n, vars, lim){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintSumGE : public ConstraintSum {
public:
    ConstraintSumGE(std::string n, std::vector<Variable*> vars, int lim)
        : ConstraintSum(n, vars, lim){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintSumLE : public ConstraintSum {
public:
    ConstraintSumLE(std::string n, std::vector<Variable*> vars, int lim)
        : ConstraintSum(n, vars, lim){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintSumNE : public ConstraintSum {
private:
    int sentinel1;
    int sentinel2;

    inline long currentSum()
    {
        long sum = 0;
        for (size_t i = 0; i < listSize; i++)
            sum += scope[i]->LocalDomIndToVal(0);
        return sum;
    }

    inline long currentSumExcept(int except)
    {
        long sum = 0;
        for (int i = 0; i < (int)listSize; i++)
            if (i != except)
                sum += scope[i]->LocalDomIndToVal(0);
        return sum;
    }

    inline int findAnotherSentinel()
    {
        for (int i = 0; i < (int)listSize; i++)
            if (i != sentinel1 && i != sentinel2 && !scope[i]->isAssigned())
                return i;
        return -1;
    }

public:
    ConstraintSumNE(std::string n, std::vector<Variable*> vars, int lim)
        : ConstraintSum(n, vars, lim)
        , sentinel1(0)
        , sentinel2(vars.size() - 1){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

#endif // CONSTRAINTSUM_H_
