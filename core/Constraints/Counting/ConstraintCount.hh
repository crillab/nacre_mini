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
 * @CreateTime: Nov 16, 2017 4:59 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTCOUNT_H_
#define CONSTRAINTCOUNT_H_

#include "Constraint.hh"
#include "Variable.hh"

#include <string>
#include <unordered_map>
#include <vector>

class ConstraintExactlyKVariable : public Constraint {
private:
    Variable* var;
    int value;
    int positionOfVarInList; // -1 if not present

public:
    ConstraintExactlyKVariable(std::string n, std::vector<Variable*> vars, int val)
        : Constraint(n, vars, vars.size() - 1)
        , var(vars.back())
        , value(val)
        , positionOfVarInList(-1)
    {
        for (size_t i = 0; i < listSize; ++i)
            if (vars[i] == vars.back()) {
                positionOfVarInList = i;
                break;
            }
    };

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintCount : public Constraint {
protected:
    int value; // the specified (target) value
    int k; // the specified number of occurrences used as limit

public:
    ConstraintCount(std::string n, std::vector<Variable*> vars, int v, int occ)
        : Constraint(n, vars)
        , value(v)
        , k(occ){};

    virtual bool propagate(int level, Variable* cur, std::vector<Variable*>& touched) = 0;
};

class ConstraintAtLeastK : public ConstraintCount {
protected:
    unsigned* sparse; // Used for storing (k+1) sentinels ; stored values correspond to variable positions in scope
    unsigned* dense;
    int limit;

    std::unordered_map<Variable*, int> varToScopePosition; //map between values and varProp indices

public:
    ConstraintAtLeastK(std::string n, std::vector<Variable*> vars, int v, int occ)
        : ConstraintCount(n, vars, v, occ)
    {
        dense = new unsigned[vars.size()];
        sparse = new unsigned[vars.size()];
        for (size_t i = 0; i < vars.size(); ++i) {
            dense[i] = i;
            varToScopePosition[scope[i]] = i;
            sparse[i] = i;
        }

        limit = -1;

        for (int e = 0; e <= k; ++e) {
            int i = sparse[e];
            if (i <= limit)
                continue;
            limit++;
            if (i > limit) {
                int f = dense[limit];
                dense[i] = f;
                dense[limit] = e;
                sparse[e] = limit;
                sparse[f] = i;
            }
        }
    }

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintAtLeast1 : public ConstraintCount {
private:
    // Two sentinels for tracking the presence of the value.
    Variable* sentinel1;
    Variable* sentinel2;

    inline Variable* findAnotherSentinel()
    {
        for (size_t i = 0; i < listSize; i++)
            if (scope[i] != sentinel1 && scope[i] != sentinel2 && scope[i]->isValidValue(value))
                return scope[i];
        return nullptr;
    }

public:
    ConstraintAtLeast1(std::string n, std::vector<Variable*> vars, int v)
        : ConstraintCount(n, vars, v, 1)
        , sentinel1(vars[0])
        , sentinel2(vars[1]){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintAtMostK : public ConstraintCount {
public:
    ConstraintAtMostK(std::string n, std::vector<Variable*> vars, int v, int occ)
        : ConstraintCount(n, vars, v, occ){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintAtMost1 : public ConstraintAtMostK {
public:
    ConstraintAtMost1(std::string n, std::vector<Variable*> vars, int v)
        : ConstraintAtMostK(n, vars, v, 1){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintExactlyK : public ConstraintCount {
public:
    ConstraintExactlyK(std::string n, std::vector<Variable*> vars, int v, int occ)
        : ConstraintCount(n, vars, v, occ){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintExactly1 : public ConstraintExactlyK {
public:
    ConstraintExactly1(std::string n, std::vector<Variable*> vars, int v)
        : ConstraintExactlyK(n, vars, v, 1){};
};

#endif // CONSTRAINTCOUNT_H_