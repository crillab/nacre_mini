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
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTELEMENT_H_
#define CONSTRAINTELEMENT_H_

#include <string>
#include <vector>

#include "Constraint.hh"
#include "Variable.hh"

class ConstraintElement : public Constraint {
protected:
    Variable* index;
    bool startAtOne;

    inline Variable* getVariableFor(int p) { return scope[p - (startAtOne ? 1 : 0)]; }

public:
    ConstraintElement(std::string n, std::vector<Variable*> vars, int sz, Variable* i, bool one)
        : Constraint(n, vars, sz)
        , index(i)
        , startAtOne(one){};

    virtual bool propagate(int level, Variable* cur, std::vector<Variable*>& touched) = 0;
};

class ConstraintElementConstant : public ConstraintElement {
private:
    int result;

public:
    ConstraintElementConstant(std::string n, std::vector<Variable*> vars, int sz, int value, bool one = false)
        : ConstraintElement(n, vars, sz, vars.back(), one)
        , result(value){};

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

class ConstraintElementVariable : public ConstraintElement {
private:
    Variable* result;

    /// For each index a of a value v in result's domain, we store the index i of a variable from list such that v is in dom(list[i]).
    indVpLocal* resultSentinels;

    /// For each variable in scope, we store a value that is both in its domain and in result's domain
    int* vectorSentinels;

    bool findVectorSentinelFor(int posIndex)
    {
        Variable* x = getVariableFor(posIndex);
        for (int i = 0, stop = x->domainCurSize; i < stop; ++i) {
            int v = x->LocalDomIndToVal(i);
            if (result->isValidValue(v)) {
                vectorSentinels[posIndex] = v;
                return true;
            }
        }
        return false;
    }

    bool reduceIndexDomain(std::vector<Variable*>& touched, int level)
    {
        int saveSize = index->domainCurSize;
        for (int i = saveSize - 1; i >= 0; --i) {
            int idvIndex = index->indDomLocalToIndVPLocal(i);
            assert(idvIndex >= 0 && idvIndex < (int)listSize);
            int sentinel = vectorSentinels[idvIndex];
            if (sentinel == -1 || !getVariableFor(idvIndex)->isValidValue(sentinel) || !result->isValidValue(sentinel))
                if (!findVectorSentinelFor(idvIndex) && deleteValue(index, i, level))
                    return false;
        }

        if (saveSize != index->domainCurSize)
            touched.push_back(index);

        return true;
    }

    bool findResultSentinelFor(int idvResult)
    {
        int v = result->LocalDomIndToVal(idvResult);
        for (int i = 0, stop = index->domainCurSize; i < stop; i++) {
            int idx = index->LocalDomIndToVal(i);
            if (getVariableFor(idx)->isValidValue(v))
                resultSentinels[idvResult] = idx;
            return true;
        }
        return false;
    }

    bool reduceResultDomain(std::vector<Variable*>& touched, int level)
    {
        int saveSize = result->domainCurSize;
        for (int i = saveSize - 1; i >= 0; --i) {
            int idvResult = result->indDomLocalToIndVPLocal(i);
            int sentinel = resultSentinels[idvResult];
            if (sentinel == -1 || !index->isValidLocalVpInd(sentinel) || !getVariableFor(sentinel)->isValidValue(result->LocalDomIndToVal(idvResult)))
                if (!findResultSentinelFor(idvResult) && deleteValue(result, i, level))
                    return false;
        }

        if (saveSize != result->domainCurSize)
            touched.push_back(result);

        return true;
    }

public:
    ConstraintElementVariable(std::string n, std::vector<Variable*> vars, int sz, bool one = false)
        : ConstraintElement(n, vars, sz, vars[sz], one)
        , result(vars[sz + 1])
    {
        int rdis = result->domainInitSize;
        resultSentinels = new int[rdis];
        for (int i = 0; i < rdis; ++i)
            resultSentinels[i] = -1;

        vectorSentinels = new int[listSize];
        for (size_t i = 0; i < listSize; ++i)
            vectorSentinels[i] = -1;
    }

    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};
#endif // CONSTRAINTELEMENT_H_
