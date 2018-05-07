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
 * @CreateTime: Sep 26, 2017 2:50 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Modify Here, Please 
 */

#ifndef ARCH_XCSP3Callbacks_H
#define ARCH_XCSP3Callbacks_H

#include "CompleteSolver.hh"
#include "ConflictAnalysisSolver.hh"
#include "Constraint.hh"
#include "ConstraintAllEqual.hh"
#include "ConstraintAmong.hh"
#include "ConstraintCount.hh"
#include "ConstraintElement.hh"
#include "ConstraintExtBinary.hh"
#include "ConstraintExtN.hh"
#include "ConstraintExtNConflict.hh"
#include "ConstraintExtUnary.hh"
#include "ConstraintExtremum.hh"
#include "ConstraintInt.hh"
#include "ConstraintLexicographic.hh"
#include "ConstraintNotAllEqual.hh"
#include "ConstraintOrdered.hh"
#include "ConstraintPrimitive.hh"
#include "ConstraintSum.hh"
#include "ConstraintSumScalarBoolean.hh"
#include "ConstraintWeightedSum.hh"
#include "GeneralizedCompleteSolver.hh"
#include "IncNGCompleteSolver.hh"
#include "LecoutreCompleteSolver.hh"
#include "Options.hh"
#include "Problem.hh"
#include "Solver.hh"
#include "Variable.hh"

#include "XCSP3CoreCallbacks.h"
#include "XCSP3Variable.h"

namespace XCSP3Core {

class XCSP3Callbacks : public XCSP3CoreCallbacks {

public:
    XCSP3Callbacks()
    {
        recognizeSpecialIntensionCases = true;
        intensionUsingString = false;
        recognizeSpecialCountCases = true;
        recognizeNValuesCases = true;
        normalizeSum = true;
    }

    virtual void beginInstance(InstanceType type) override;

    virtual void buildVariableInteger(string id, int minValue, int maxValue) override;
    virtual void buildVariableInteger(string id, vector<int>& values) override;

    // GENERIC
    virtual void buildConstraintExtension(string id, vector<XVariable*> list, vector<vector<int>>& tuples, bool support, bool hasStar) override;
    virtual void buildConstraintExtension(string id, XVariable* variable, vector<int>& tuples, bool support, bool hasStar) override;
    virtual void buildConstraintExtensionAs(string id, vector<XVariable*> list, bool support, bool hasStar) override;

    virtual void buildConstraintIntension(string id, string expr) override;
    virtual void buildConstraintIntension(string id, Tree* tree) override;

    virtual void buildConstraintPrimitive(string id, OrderType op, XVariable* x, int k, XVariable* y) override;
    virtual void buildConstraintPrimitive(string id, OrderType op, XVariable* x, int k) override;
    virtual void buildConstraintPrimitive(string id, XVariable* x, bool in, int min, int max) override;

    // LANGUAGE
    virtual void buildConstraintRegular(string id, vector<XVariable*>& list, string st, vector<string>& final, vector<XTransition>& transitions) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintMDD(string id, vector<XVariable*>& list, vector<XTransition>& transitions) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }

    // COMPARISON
    virtual void buildConstraintAlldifferent(string id, vector<Tree*>& list) override;
    virtual void buildConstraintAlldifferent(string id, vector<XVariable*>& list) override;
    virtual void buildConstraintAlldifferentExcept(string id, vector<XVariable*>& list, vector<int>& except) override;
    virtual void buildConstraintAlldifferentList(string id, vector<vector<XVariable*>>& lists) override;
    virtual void buildConstraintAlldifferentMatrix(string id, vector<vector<XVariable*>>& matrix) override;

    virtual void buildConstraintAllEqual(string id, vector<XVariable*>& list) override;
    virtual void buildConstraintLex(string id, vector<vector<XVariable*>>& lists, OrderType order) override;
    virtual void buildConstraintLexMatrix(string id, vector<vector<XVariable*>>& matrix, OrderType order) override;
    virtual void buildConstraintNotAllEqual(string id, vector<XVariable*>& list) override;

    virtual void buildConstraintOrdered(string id, vector<XVariable*>& list, OrderType order) override;
    virtual void buildConstraintOrdered(string id, vector<XVariable*>& list, vector<int>& lengths, OrderType order) override;

    // SUMMING
    virtual void buildConstraintSum(string id, vector<Tree*>& list, vector<int>& coeffs, XCondition& cond) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintSum(string id, vector<Tree*>& list, XCondition& cond) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintSum(string id, vector<XVariable*>& list, vector<int>& coeffs, XCondition& cond) override;
    virtual void buildConstraintSum(string id, vector<XVariable*>& list, vector<XVariable*>& coeffs, XCondition& cond) override;
    virtual void buildConstraintSum(string id, vector<XVariable*>& list, XCondition& cond) override;

    // COUNTING
    virtual void buildConstraintAmong(string id, vector<XVariable*>& list, vector<int>& values, int k) override;
    virtual void buildConstraintAtLeast(string id, vector<XVariable*>& list, int value, int k) override;
    virtual void buildConstraintAtMost(string id, vector<XVariable*>& list, int value, int k) override;
    virtual void buildConstraintCount(string id, vector<XVariable*>& list, vector<int>& values, XCondition& xc) override;
    virtual void buildConstraintCount(string id, vector<XVariable*>& list, vector<XVariable*>& values, XCondition& xc) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintExactlyK(string id, vector<XVariable*>& list, int value, int k) override;
    virtual void buildConstraintExactlyVariable(string id, vector<XVariable*>& list, int value, XVariable* x) override;
    virtual void buildConstraintNValues(string id, vector<XVariable*>& list, vector<int>& except, XCondition& xc) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintNValues(string id, vector<XVariable*>& list, XCondition& xc) override;

    virtual void buildConstraintCardinality(string id, vector<XVariable*>& list, vector<int> values, vector<int>& occurs, bool closed) override;
    virtual void buildConstraintCardinality(string id, vector<XVariable*>& list, vector<int> values, vector<XInterval>& occurs, bool closed) override;
    virtual void buildConstraintCardinality(string id, vector<XVariable*>& list, vector<int> values, vector<XVariable*>& occurs, bool closed) override;
    virtual void buildConstraintCardinality(string id, vector<XVariable*>& list, vector<XVariable*> values, vector<int>& occurs, bool closed) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCardinality(string id, vector<XVariable*>& list, vector<XVariable*> values, vector<XInterval>& occurs, bool closed) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCardinality(string id, vector<XVariable*>& list, vector<XVariable*> values, vector<XVariable*>& occurs, bool closed) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }

    // CONNECTION
    virtual void buildConstraintChannel(string id, vector<XVariable*>& list, int startIndex, XVariable* value) override;
    virtual void buildConstraintChannel(string id, vector<XVariable*>& list, int startIndex) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintChannel(string id, vector<XVariable*>& list1, int startIndex1, vector<XVariable*>& list2, int startIndex2) override;

    virtual void buildConstraintElement(string id, vector<int>& list, int startIndex, XVariable* index, RankType rank, XVariable* value) override;
    virtual void buildConstraintElement(string id, vector<XVariable*>& list, int startIndex, XVariable* index, RankType rank, int value) override;
    virtual void buildConstraintElement(string id, vector<XVariable*>& list, int startIndex, XVariable* index, RankType rank, XVariable* value) override;
    virtual void buildConstraintElement(string id, vector<XVariable*>& list, int value) override;
    virtual void buildConstraintElement(string id, vector<XVariable*>& list, XVariable* value) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }

    virtual void buildConstraintMaximum(string id, vector<XVariable*>& list, XCondition& xc) override;
    virtual void buildConstraintMaximum(string id, vector<XVariable*>& list, XVariable* index, int startIndex, RankType rank, XCondition& xc) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintMinimum(string id, vector<XVariable*>& list, XCondition& xc) override;
    virtual void buildConstraintMinimum(string id, vector<XVariable*>& list, XVariable* index, int startIndex, RankType rank, XCondition& xc) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }

    // INSTANTIATION
    virtual void buildConstraintInstantiation(string id, vector<XVariable*>& list, vector<int>& values) override;

    // PACKING AND SCHEDULLING
    virtual void buildConstraintCircuit(string id, vector<XVariable*>& list, int startIndex, int size) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCircuit(string id, vector<XVariable*>& list, int startIndex, XVariable* size) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCircuit(string id, vector<XVariable*>& list, int startIndex) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }

    virtual void buildConstraintNoOverlap(string id, vector<vector<XVariable*>>& origins, vector<vector<int>>& lengths, bool zeroIgnored) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintNoOverlap(string id, vector<vector<XVariable*>>& origins, vector<vector<XVariable*>>& lengths, bool zeroIgnored) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintNoOverlap(string id, vector<XVariable*>& origins, vector<int>& lengths, bool zeroIgnored) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintNoOverlap(string id, vector<XVariable*>& origins, vector<XVariable*>& lengths, bool zeroIgnored) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }

    virtual void buildConstraintStretch(string id, vector<XVariable*>& list, vector<int>& values, vector<XInterval>& widths, vector<vector<int>>& patterns) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintStretch(string id, vector<XVariable*>& list, vector<int>& values, vector<XInterval>& widths) override
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }

    // CUMULATIVE
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<int>& lengths, vector<int>& heights, vector<XVariable*>& ends, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<int>& lengths, vector<int>& heights, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<int>& lengths, vector<XVariable*>& varHeights, vector<XVariable*>& ends, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<int>& lengths, vector<XVariable*>& varHeights, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<XVariable*>& lengths, vector<int>& heights, vector<XVariable*>& ends, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<XVariable*>& lengths, vector<int>& heights, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<XVariable*>& lengths, vector<XVariable*>& heights, vector<XVariable*>& ends, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
    virtual void buildConstraintCumulative(string id, vector<XVariable*>& origins, vector<XVariable*>& lengths, vector<XVariable*>& heights, XCondition& xc)
    {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported");
    }
};
}

using namespace XCSP3Core;

int typeCSP;

vector<Constraint*> vecCont;
vector<Constraint*> vecBtConst;
Problem* problem;

std::map<string, Variable*> mapping;

void toMyVariables(vector<XVariable*>& src, vector<Variable*>& dest)
{
    for (size_t i = 0, stop = src.size(); i < stop; ++i) {
        if (!mapping[src[i]->id])
            throw runtime_error("Unknow value " + src[i]->id);
        dest.push_back(mapping[src[i]->id]);
    }
}

string createExpression(string minmax, OrderType op, vector<XVariable*>& list, string value)
{
    string expr;

    switch (op) {
    case LT:
        expr = "lt";
        break;
    case LE:
        expr = "le";
        break;
    case GT:
        expr = "gt";
        break;
    case GE:
        expr = "ge";
        break;
    case EQ:
        expr = "eq";
        break;
    case NE:
        expr = "ne";
        break;
    default:
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("not yet supported");
        break;
    }

    expr += "(" + value + "," + minmax + "(";
    for (size_t i = 0, stop = list.size(); i < stop; i++)
        expr += (i != 0 ? "," : "") + list[i]->id;
    expr += "))";
    return expr;
}

void XCSP3Callbacks::beginInstance(InstanceType type)
{
    if (type == COP) {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("COP instances unsupported");
    }

    typeCSP = type;
    problem = new Problem();
}

void XCSP3Callbacks::buildVariableInteger(string id, int minValue, int maxValue)
{
    mapping[id] = (minValue == maxValue) ? new Variable(id, maxValue) : new Variable(id, minValue, maxValue);
    problem->addVariable(mapping[id]);
}

void XCSP3Callbacks::buildVariableInteger(string id, vector<int>& values)
{
    if (values.size() == 1)
        mapping[id] = new Variable(id, values[0]);
    else
        mapping[id] = new Variable(id, values);
    problem->addVariable(mapping[id]);
}

vector<int> lastTuples;
vector<vector<int>> lastTuplesN;
bool lastExtUnary;

void createExt(string id, vector<XVariable*>& list, vector<vector<int>>& tuples, bool support)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    vector<vector<ind>> t(tuples.size());
    for (size_t i = 0; i < tuples.size(); ++i) {
        vector<ind> tmp;
        for (size_t j = 0; j < tuples[i].size(); ++j)
            tmp.push_back((tuples[i][j] == STAR ? STAR : vars[j]->getVarPropIndFromValue(tuples[i][j])));
        t[i] = tmp;
    }

    if (list.size() == 2) {
        vecCont.push_back(new ConstraintExtBinary(id, vars, t, support));
    } else {
        Constraint* newCont = (support ? static_cast<Constraint*>(new ConstraintExtN(id, vars, t)) : static_cast<Constraint*>(new ConstraintExtNConflict(id, vars, t)));
        vecCont.push_back(newCont);
        vecBtConst.push_back(newCont);
    }
}

Variable* getMyVar(XVariable* x)
{
    Variable* var = mapping[x->id];
    if (!var)
        throw runtime_error("Unknow variable :" + x->id);
    return var;
}

void XCSP3Callbacks::buildConstraintExtension(string id, vector<XVariable*> list, vector<vector<int>>& tuples, bool support, bool hasStar)
{
    lastTuplesN = tuples;
    lastExtUnary = false;

    if (hasStar && !support) {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Extension conflict constraint with star is not yet supported");
    }

    if (!support && !lastTuplesN.size())
        return;
    if (support && !lastTuplesN.size()) {
        std::cout << "s UNSATISFIABLE" << endl;
        throw runtime_error("UNSAT by default");
    }

    createExt(id, list, lastTuplesN, support);
}

void XCSP3Callbacks::buildConstraintExtension(string id, XVariable* variable, vector<int>& tuples, bool support, bool hasStar)
{
    lastTuples = tuples;
    lastExtUnary = true;

    if (hasStar) {
        if (support) {
            std::cout << "s UNSATISFIABLE" << endl;
            throw runtime_error("UNSAT by default");
        }
        return;
    }

    vecCont.push_back(new ConstraintExtUnary(id, getMyVar(variable), lastTuples, support));
}

void XCSP3Callbacks::buildConstraintExtensionAs(string id, vector<XVariable*> list, bool support, bool hasStar)
{
    if (lastExtUnary) {
        assert(list.size() == 1);
        vecCont.push_back(new ConstraintExtUnary(id, getMyVar(list[0]), lastTuples, support));
    } else {
        if (!support && !lastTuplesN.size())
            return;
        createExt(id, list, lastTuplesN, support);
    }
}

void XCSP3Callbacks::buildConstraintIntension(string id, string expr)
{
    Tree* tree = new Tree(expr);
    buildConstraintIntension(id, tree);
}

void XCSP3Callbacks::buildConstraintIntension(string id, Tree* tree)
{
    std::vector<Variable*> scope;
    for (auto var_str : tree->listOfVariables) {
        Variable* var = mapping[var_str];
        if (!var)
            throw runtime_error("Unknow value " + var_str);
        scope.push_back(var);
    }

    Constraint* newCont = static_cast<Constraint*>(new ConstraintInt(id, scope, tree));
    vecCont.push_back(newCont);
}

//Recognize constraint with the form  x +- k op y (op \in {eq, le, lt, ge, gt, ne}.
void XCSP3Callbacks::buildConstraintPrimitive(string id, OrderType op, XVariable* x, int k, XVariable* y)
{
    switch (op) {
    case LT:
        vecCont.push_back(new ConstraintPrimitiveLessThan(id, getMyVar(x), getMyVar(y), k));
        break;
    case LE:
        vecCont.push_back(new ConstraintPrimitiveLessEqual(id, getMyVar(x), getMyVar(y), k));
        break;
    case GT:
        vecCont.push_back(new ConstraintPrimitiveLessThan(id, getMyVar(y), getMyVar(x), -k));
        break;
    case GE:
        vecCont.push_back(new ConstraintPrimitiveLessEqual(id, getMyVar(y), getMyVar(x), -k));
        break;
    case EQ:
        vecCont.push_back(new ConstraintPrimitiveEqual(id, getMyVar(x), getMyVar(y), k));
        break;
    case NE:
        vecCont.push_back(new ConstraintPrimitiveNotEqual(id, getMyVar(x), getMyVar(y), k));
        break;
    default:
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Primitive not supported");
        break;
    }
}

void XCSP3Callbacks::buildConstraintPrimitive(string id, OrderType op, XVariable* x, int k)
{
    assert(op == LE || op == GE);
    Variable* var = getMyVar(x);

    if (op == LE && var->removeValuesAbove(k, 0)) {
        std::cout << "s UNSATISFIABLE" << endl;
        throw runtime_error("Domain wipeout of " + x->id + " during parsing");
    } else if (op == GE && var->removeValuesUnder(k, 0)) {
        std::cout << "s UNSATISFIABLE" << endl;
        throw runtime_error("Domain wipeout of " + x->id + " during parsing");
    }
}

void XCSP3Callbacks::buildConstraintPrimitive(string id, XVariable* x, bool in, int min, int max)
{
    // x>=min and x<=max , x in/notin k
    Variable* var = getMyVar(x);
    if (in) {
        if (min == max)
            var->assignTo(min, 0);
        else {
            if (var->removeValuesAbove(max, 0)) {
                std::cout << "s UNSATISFIABLE" << endl;
                throw runtime_error("Domain wipeout of " + x->id + " during parsing");
            }
            if (var->removeValuesUnder(min, 0)) {
                std::cout << "s UNSATISFIABLE" << endl;
                throw runtime_error("Domain wipeout of " + x->id + " during parsing");
            }
        }
    } else if (min == max) {
        if (var->removeValue(min, 0)) {
            std::cout << "s UNSATISFIABLE" << endl;
            throw runtime_error("Domain wipeout of " + x->id + " during parsing");
        } else if (var->removeValues(min, max, 0)) {
            std::cout << "s UNSATISFIABLE" << endl;
            throw runtime_error("Domain wipeout of " + x->id + " during parsing");
        }
    }
}

void XCSP3Callbacks::buildConstraintAllEqual(string id, vector<XVariable*>& list)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    ConstraintAllEqual* newCont = new ConstraintAllEqual(id, vars);

    vecCont.push_back(newCont);
    vecBtConst.push_back(newCont);
}

void XCSP3Callbacks::buildConstraintNotAllEqual(string id, vector<XVariable*>& list)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    vecCont.push_back(new ConstraintNotAllEqual(id, vars));
}

void XCSP3Callbacks::buildConstraintAlldifferent(string id, vector<Tree*>& list)
{
    for (size_t i = 0; i < list.size() - 1; i++) {
        for (size_t j = i + 1; j < list.size(); j++) {
            NodeNE* n = new NodeNE();
            n->addParameter(list[i]->root);
            n->addParameter(list[j]->root);
            Tree* tmp = new Tree(n);

            // Problem with scope
            for (string s : list[i]->listOfVariables)
                tmp->listOfVariables.push_back(s);
            for (string s : list[j]->listOfVariables)
                if (std::find(tmp->listOfVariables.begin(), tmp->listOfVariables.end(), s) == tmp->listOfVariables.end())
                    tmp->listOfVariables.push_back(s);
            buildConstraintIntension(id, tmp);
        }
    }
}

void XCSP3Callbacks::buildConstraintAlldifferent(string id, vector<XVariable*>& list)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    for (size_t i = 0; i < vars.size() - 1; ++i)
        for (size_t j = i + 1; j < vars.size(); ++j)
            vecCont.push_back(new ConstraintPrimitiveNotEqual(id, vars[i], vars[j], 0));
}

void XCSP3Callbacks::buildConstraintAlldifferentExcept(string id, vector<XVariable*>& list, vector<int>& except)
{
    assert(except.size() == 1);
    for (size_t i = 0; i < list.size(); i++)
        for (size_t j = i + 1; j < list.size(); j++)
            buildConstraintIntension(id,
                new Tree("or(ne(" + list[i]->id + "," + list[j]->id + "), "
                    + "eq(" + list[i]->id + "," + to_string(except[0]) + "))"));
}

vector<Variable*> createScope(vector<Variable*> vars1, vector<Variable*> vars2)
{
    set<Variable*, variableIdCmp> varsSet(vars1.begin(), vars1.end());
    copy(vars2.begin(), vars2.end(), inserter(varsSet, varsSet.end()));
    return vector<Variable*>(varsSet.begin(), varsSet.end());
}

vector<int> varsToScope(vector<Variable*> list, vector<Variable*> scope)
{
    vector<int> ret;

    for (auto l : list) {
        auto f = find(scope.begin(), scope.end(), l);
        ret.push_back(f - scope.begin());
    }

    assert(ret.size() == list.size());
    return ret;
}

void createExtensionDistinctVector(string id, vector<XVariable*>& l1, vector<XVariable*>& l2)
{
    assert(l1.size() == l2.size());

    vector<Variable*> vars1, vars2;
    toMyVariables(l1, vars1);
    toMyVariables(l2, vars2);

    vector<Variable*> scope = createScope(vars1, vars2);
    vector<int> list1 = varsToScope(vars1, scope);
    vector<int> list2 = varsToScope(vars2, scope);

    vector<vector<indVp>> tuples;
    for (size_t i = 0; i < list1.size(); i++) {
        Variable* x = scope[list1[i]];
        Variable* y = scope[list2[i]];

        if (x->getId() == y->getId())
            continue;

        int xDomStart = x->domainStart;
        int yDomStart = y->domainStart;

        for (int j = 0; j < x->domainInitSize; j++) {
            int ivp1 = xDomStart + j;
            for (int k = 0; k < y->domainInitSize; k++) {
                int ivp2 = yDomStart + k;

                if (Variable::varProps[ivp1].val == Variable::varProps[ivp2].val)
                    continue;

                vector<indVp> tmp(scope.size(), STAR);
                tmp[list1[i]] = ivp1;
                tmp[list2[i]] = ivp2;
                tuples.push_back(tmp);
            }
        }
    }

    ConstraintExtN* newCont = new ConstraintExtN(id, scope, tuples);
    vecCont.push_back(newCont);
    vecBtConst.push_back(newCont);
}

void XCSP3Callbacks::buildConstraintAlldifferentList(string id, vector<vector<XVariable*>>& lists)
{
    for (size_t i = 0; i < lists.size() - 1; ++i)
        for (size_t j = i + 1; j < lists.size(); ++j)
            createExtensionDistinctVector("", lists[i], lists[j]);
}

void XCSP3Callbacks::buildConstraintAlldifferentMatrix(string id, vector<vector<XVariable*>>& matrix)
{
    for (auto line : matrix)
        buildConstraintAlldifferent("", line);

    for (size_t i = 0; i < matrix[0].size(); ++i) {
        vector<XVariable*> column;
        for (auto line : matrix)
            column.push_back(line[i]);
        buildConstraintAlldifferent("", column);
    }
}

void sum(string id, vector<Variable*>& vars, int limit, OrderType op)
{
    switch (op) {
    case LT:
        limit--;
    case LE:
        vecCont.push_back(new ConstraintSumLE(id, vars, limit));
        break;
    case GT:
        limit++;
    case GE:
        vecCont.push_back(new ConstraintSumGE(id, vars, limit));
        break;
    case EQ:
        vecCont.push_back(new ConstraintSumEQ(id, vars, limit));
        break;
    case NE:
        vecCont.push_back(new ConstraintSumNE(id, vars, limit));
        break;
    /* case IN:
        vecCont.push_back(new ConstraintSumGE(id, vars, cond.min));
        vecCont.push_back(new ConstraintSumLE(id, vars, cond.max));
        break; */
    default:
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for sum");
        break;
    }
}

void XCSP3Callbacks::buildConstraintSum(string id, vector<XVariable*>& list, XCondition& cond)
{
    if (cond.operandType == VARIABLE) {
        vector<int> coefficients(list.size(), 1);
        buildConstraintSum(id, list, coefficients, cond);
        return;
    }

    vector<Variable*> vars;
    toMyVariables(list, vars);

    sum(id, vars, cond.val, cond.op);
}

OrderType invertOp(OrderType op)
{
    return op == LT ? GT : op == LE ? GE : op == GE ? LE : op == GT ? LT : op;
}

void XCSP3Callbacks::buildConstraintSum(string id, vector<XVariable*>& list, vector<int>& coefficients, XCondition& cond)
{
    assert(list.size() == coefficients.size());

    vector<Variable*> vars;
    toMyVariables(list, vars);

    int limit = 0;

    if (cond.operandType == VARIABLE) {
        cond.operandType = INTEGER;
        Variable* var = mapping[cond.var];
        if (!var)
            throw runtime_error("Unknow value " + cond.var);
        vars.push_back(var);
        coefficients.push_back(-1);
    } else
        limit = cond.val;

    // Need order on coefficients
    for (size_t i = 0; i < vars.size(); ++i) {
        size_t pos = i;
        for (size_t j = i + 1; j < vars.size(); ++j) {
            if (coefficients[j] < coefficients[pos])
                pos = j;
        }
        swap(coefficients[i], coefficients[pos]);
        swap(vars[i], vars[pos]);
    }

    // if only -1 since sorted we can do a standart sum
    if (coefficients[0] == -1 && coefficients[coefficients.size() - 1] == -1) {
        sum(id, vars, -limit, invertOp(cond.op));
        return;
    }

    switch (cond.op) {
    case LT:
        limit--;
    case LE:
        vecCont.push_back(new ConstraintWeightedSumLE(id, vars, coefficients, limit));
        break;
    case GT:
        limit++;
    case GE:
        vecCont.push_back(new ConstraintWeightedSumGE(id, vars, coefficients, limit));
        break;
    case EQ:
        vecCont.push_back(new ConstraintWeightedSumEQ(id, vars, coefficients, limit));
        break;
    case NE:
        vecCont.push_back(new ConstraintWeightedSumNE(id, vars, coefficients, limit));
        break;
    /* case IN:
        vecCont.push_back(new ConstraintWeightedSumGE(id, vars, coefficients, cond.min));
        for (size_t i = 0; i < coefficients.size(); i++)
            coefficients[i] = -coefficients[i];
        vecCont.push_back(new ConstraintWeightedSumGE(id, vars, coefficients, -cond.max));
        break; */
    default:
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for sum");
        break;
    }
}

void XCSP3Callbacks::buildConstraintSum(string id, vector<XVariable*>& list, vector<XVariable*>& coefficients, XCondition& cond)
{
    assert(list.size() == coefficients.size());

    vector<Variable*> vars;
    vector<XVariable*> xVars(list.begin(), list.end());
    xVars.insert(xVars.end(), coefficients.begin(), coefficients.end());
    toMyVariables(xVars, vars);

    bool allBool = true;
    for (size_t i = 0; i < vars.size(); ++i)
        if (vars[i]->domainInitSize != 2 || vars[i]->getLowerBoundVal() != 0 || vars[i]->getUpperBoundVal() != 1) {
            allBool = false;
            break;
        }

    if (allBool) {
        switch (cond.op) {
        case LE:
            if (cond.operandType == VARIABLE) {
                Variable* var = mapping[cond.var];
                if (!var)
                    throw runtime_error("Unknow value " + cond.var);
                vecCont.push_back(new SumScalarBooleanLEVar(id, vars, var));
            } else
                vecCont.push_back(new SumScalarBooleanLE(id, vars, cond.val));
            return;
        case EQ:
            vecCont.push_back(new SumScalarBooleanEQ(id, vars, cond.val));
            return;
        default:
            break;
        }
    }

    string tmp = "add(";
    assert(list.size() == coefficients.size());
    for (size_t i = 0; i < list.size(); i++) {
        tmp = tmp + "mul(" + list[i]->id + "," + coefficients[i]->id + ")";
        if (i < list.size() - 1)
            tmp = tmp + ",";
    }

    if (cond.operandType == VARIABLE) {
        cond.operandType = INTEGER;
        cond.val = 0;
        tmp = tmp + ",neg(" + cond.var + ")";
    }

    tmp = tmp + ")";

    switch (cond.op) {
    case LT:
        tmp = "lt(" + tmp;
        break;
    case LE:
        tmp = "le(" + tmp;
        break;
    case GT:
        tmp = "gt(" + tmp;
        break;
    case GE:
        tmp = "ge(" + tmp;
        break;
    case EQ:
        tmp = "eq(" + tmp;
        break;
    case NE:
        tmp = "ne(" + tmp;
        break;
    case IN:
        // Intervals
        buildConstraintIntension(id, new Tree("ge(" + tmp + "," + to_string(cond.min) + ")"));
        buildConstraintIntension(id, new Tree("le(" + tmp + "," + to_string(cond.max) + ")"));
        return;
    default:
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for weighted sum");
        break;
    }

    tmp += "," + to_string(cond.val) + ")";
    buildConstraintIntension(id, new Tree(tmp));
}

void XCSP3Callbacks::buildConstraintLex(string id, vector<vector<XVariable*>>& lists, OrderType order)
{
    bool isStrict = false;
    bool invert = false;

    switch (order) {
    case GT:
        invert = true;
    case LT:
        isStrict = true;
        break;
    case GE:
        invert = true;
    case LE:
        break;
    default:
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for lex");
        break;
    }

    vector<Variable*> list1, list2;
    for (size_t i = 0; i < lists.size() - 1; i++) {
        toMyVariables(lists[i], list1);
        toMyVariables(lists[i + 1], list2);
        if (invert)
            vecCont.push_back(new ConstraintLexicographic(id, list2, list1, isStrict));
        else
            vecCont.push_back(new ConstraintLexicographic(id, list1, list2, isStrict));
        list1.clear();
        list2.clear();
    }
}

void XCSP3Callbacks::buildConstraintLexMatrix(string id, vector<vector<XVariable*>>& matrix, OrderType order)
{
    buildConstraintLex(id, matrix, order);

    vector<vector<XVariable*>> trans;
    for (size_t i = 0; i < matrix[0].size(); ++i) {
        vector<XVariable*> line;
        for (size_t j = 0; j < matrix.size(); ++j)
            line.push_back(matrix[j][i]);
        trans.push_back(line);
    }

    buildConstraintLex(id, trans, order);
}

void XCSP3Callbacks::buildConstraintOrdered(string id, vector<XVariable*>& list, OrderType order)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    switch (order) {
    case GT:
        vecCont.push_back(new ConstraintOrderedGT(id, vars));
        break;
    case GE:
        vecCont.push_back(new ConstraintOrderedGE(id, vars));
        break;
    case LT:
        vecCont.push_back(new ConstraintOrderedLT(id, vars));
        break;
    case LE:
        vecCont.push_back(new ConstraintOrderedLE(id, vars));
        break;
    default:
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for ordered");
        break;
    }
}

void XCSP3Callbacks::buildConstraintOrdered(string id, vector<XVariable*>& list, vector<int>& lengths, OrderType order)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    for (size_t i = 0; i < vars.size() - 2; i++) {
        int k = lengths[i];
        switch (order) {
        case LT:
            vecCont.push_back(new ConstraintPrimitiveLessThan(id, vars[i], vars[i + 1], k));
            break;
        case LE:
            vecCont.push_back(new ConstraintPrimitiveLessEqual(id, vars[i], vars[i + 1], k));
            break;
        case GT:
            vecCont.push_back(new ConstraintPrimitiveLessThan(id, vars[i + 1], vars[i], -k));
            break;
        case GE:
            vecCont.push_back(new ConstraintPrimitiveLessEqual(id, vars[i + 1], vars[i], -k));
            break;
        default:
            std::cout << "s UNSUPPORTED" << endl;
            throw runtime_error("Operator not supported for ordered");
            break;
        }
    }
}

void XCSP3Callbacks::buildConstraintMinimum(string id, vector<XVariable*>& list, XCondition& xc)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    if (xc.operandType == VARIABLE) {
        if (xc.op == EQ) {
            Variable* var = mapping[xc.var];
            if (!var)
                throw runtime_error("Unknow value " + xc.var);
            vars.push_back(var);
            vecCont.push_back(new ConstraintMinimum(id, vars, vars.size() - 1));
        } else
            buildConstraintIntension(id, new Tree(createExpression("min", xc.op, list, xc.var)));
    } else if (xc.operandType == INTEGER) {
        if (xc.op == LE || xc.op == LT || xc.op == GE || xc.op == GT) {
            std::cout << "s UNSUPPORTED" << endl;
            throw runtime_error("Operator not supported for minimum");
        }
        buildConstraintIntension(id, new Tree(createExpression("min", xc.op, list, to_string(xc.val))));
    } else {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for minimum");
    }
}

void XCSP3Callbacks::buildConstraintMaximum(string id, vector<XVariable*>& list, XCondition& xc)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    if (xc.operandType == VARIABLE) {
        if (xc.op == EQ) {
            Variable* var = mapping[xc.var];
            if (!var)
                throw runtime_error("Unknow value " + xc.var);
            vars.push_back(var);
            vecCont.push_back(new ConstraintMaximum(id, vars, vars.size() - 1));
        } else
            buildConstraintIntension(id, new Tree(createExpression("max", xc.op, list, xc.var)));
    } else if (xc.operandType == INTEGER) {
        if (xc.op == LE || xc.op == LT || xc.op == GE || xc.op == GT) {
            std::cout << "s UNSUPPORTED" << endl;
            throw runtime_error("Operator not supported for maximum");
        }
        buildConstraintIntension(id, new Tree(createExpression("max", xc.op, list, to_string(xc.val))));
    } else {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for maximum");
    }
}

void XCSP3Callbacks::buildConstraintAmong(string id, vector<XVariable*>& list, vector<int>& values, int k)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    if ((int)vars.size() == k) {
        for (auto var : vars) {
            for (int i = 0; i < var->domainCurSize; ++i) {
                int val = var->getVarPropFromLocalDomInd(i).val;
                if (find(values.begin(), values.end(), val) != values.end())
                    if (var->removeAt(i)) {
                        std::cout << "s UNSATISFIABLE" << endl;
                        throw runtime_error("UNSAT by default");
                    }
            }
        }
    } else if ((int)vars.size() >= k) {
        std::cout << "s UNSATISFIABLE" << endl;
        throw runtime_error("UNSAT by default");
    } else
        vecCont.push_back(new ConstraintAmong(id, vars, values, k));
}

void XCSP3Callbacks::buildConstraintAtLeast(string id, vector<XVariable*>& list, int value, int k)
{
    vector<Variable*> vars;
    for (size_t i = 0; i < list.size(); ++i) {
        Variable* v = mapping[list[i]->id];
        if (!v)
            throw runtime_error("Unknow value " + list[i]->id);
        if (v->isValidValue(value)) {
            if (v->isAssigned())
                k--;
            else
                vars.push_back(v);
        }
    }

    int sz = vars.size();
    if (k <= 0)
        return;
    else if (k > sz) {
        std::cout << "s UNSATISFIABLE" << endl;
        throw runtime_error("UNSAT by default");
    } else if (k == 1)
        vecCont.push_back(new ConstraintAtLeast1(id, vars, value));
    else if (k == sz)
        for (auto v : vars)
            v->assignTo(value);
    else
        vecCont.push_back(new ConstraintAtLeastK(id, vars, value, k));
}

void XCSP3Callbacks::buildConstraintAtMost(string id, vector<XVariable*>& list, int value, int k)
{
    vector<Variable*> vars;
    for (size_t i = 0; i < list.size(); ++i) {
        Variable* v = mapping[list[i]->id];
        if (!v)
            throw runtime_error("Unknow value " + list[i]->id);
        if (v->isValidValue(value)) {
            if (v->isAssigned())
                k--;
            else
                vars.push_back(v);
        }
    }

    int sz = vars.size();
    if (k < 0 || k >= sz) {
        std::cout << "s UNSATISFIABLE" << endl;
        throw runtime_error("UNSAT by default");
    } else if (k == 0) {
        for (auto v : vars)
            if (v->removeValue(value)) {
                std::cout << "s UNSATISFIABLE" << endl;
                throw runtime_error("UNSAT by default");
            }
    } else if (k == 1)
        vecCont.push_back(new ConstraintAtMost1(id, vars, value));
    else
        vecCont.push_back(new ConstraintAtMostK(id, vars, value, k));
}

void buildConstraintExactlyVariable2(string id, vector<XVariable*>& list, int value, XCondition& xc)
{
    assert(xc.operandType == VARIABLE);

    vector<Variable*> vars;
    toMyVariables(list, vars);

    Variable* var = mapping[xc.var];
    if (!var)
        throw runtime_error("Unknow variable :" + xc.var);
    vars.push_back(var);

    vecCont.push_back(new ConstraintExactlyKVariable(id, vars, value));
}

void XCSP3Callbacks::buildConstraintCount(string id, vector<XVariable*>& list, vector<int>& values, XCondition& xc)
{
    if (xc.operandType == VARIABLE) {
        if (values.size() == 1 && xc.op == EQ)
            buildConstraintExactlyVariable2(id, list, values[0], xc);
        else {
            std::cout << "s UNSUPPORTED" << endl;
            throw runtime_error("Case not supported for Count");
        }
    } else if (xc.operandType == INTEGER) {
        if (values.size() == 1) {
            if (xc.op == GE)
                buildConstraintAtLeast(id, list, values[0], xc.val);
            else if (xc.op == GT)
                buildConstraintAtLeast(id, list, values[0], xc.val + 1);
            else if (xc.op == LT)
                buildConstraintAtMost(id, list, values[0], xc.val - 1);
            else if (xc.op == LE)
                buildConstraintAtMost(id, list, values[0], xc.val);
            else if (xc.op == EQ)
                buildConstraintExactlyK(id, list, values[0], xc.val);
            else {
                std::cout << "s UNSUPPORTED" << endl;
                throw runtime_error("Case not supported for Count");
            }
        } else if (xc.op == EQ)
            buildConstraintAmong(id, list, values, xc.val);
        else {
            std::cout << "s UNSUPPORTED" << endl;
            throw runtime_error("Case not supported for Count");
        }
    } else {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Operator not supported for Count");
    }
}

void XCSP3Callbacks::buildConstraintExactlyK(string id, vector<XVariable*>& list, int value, int k)
{
    vector<Variable*> vars;
    for (size_t i = 0; i < list.size(); ++i) {
        Variable* v = mapping[list[i]->id];
        if (!v)
            throw runtime_error("Unknow value " + list[i]->id);
        if (v->isValidValue(value)) {
            if (v->isAssigned())
                k--;
            else
                vars.push_back(v);
        }
    }

    int sz = vars.size();
    if (k < 0 || k > sz) {
        std::cout << "s UNSATISFIABLE" << endl;
        throw runtime_error("UNSAT by default");
    } else if (k == 0) {
        for (auto v : vars)
            if (v->removeValue(value)) {
                std::cout << "s UNSATISFIABLE" << endl;
                throw runtime_error("UNSAT by default");
            }
    } else if (k == 1)
        vecCont.push_back(new ConstraintExactly1(id, vars, value));
    else if (k == sz)
        for (auto v : vars)
            v->assignTo(value);
    else
        vecCont.push_back(new ConstraintExactlyK(id, vars, value, k));
}

void XCSP3Callbacks::buildConstraintExactlyVariable(string id, vector<XVariable*>& list, int value, XVariable* x)
{
    vector<Variable*> vars;
    toMyVariables(list, vars);

    vars.push_back(getMyVar(x));

    vecCont.push_back(new ConstraintExactlyKVariable(id, vars, value));
}

void XCSP3Callbacks::buildConstraintNValues(string id, vector<XVariable*>& list, XCondition& xc)
{
    if (xc.operandType == INTEGER) {
        if (xc.op == GT && xc.val == 1)
            buildConstraintNotAllEqual(id, list);
        else {
            std::cout << "s UNSUPPORTED" << endl;
            throw runtime_error("Unsupported case for nValues");
        }

    } else {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported case for nValues");
    }
}

void XCSP3Callbacks::buildConstraintCardinality(string id, vector<XVariable*>& list, vector<int> values, vector<int>& intOccurs, bool closed)
{
    if (intOccurs.size() != values.size())
        throw std::logic_error("Cardinality: Occurs and values must have the same size");

    for (size_t i = 0; i < intOccurs.size(); i++)
        buildConstraintExactlyK(id, list, values[i], intOccurs[i]);
}

void XCSP3Callbacks::buildConstraintCardinality(string id, vector<XVariable*>& list, vector<int> values, vector<XVariable*>& varOccurs, bool closed)
{
    if (varOccurs.size() != values.size())
        throw std::logic_error("Cardinality: Occurs and values must have the same size");

    for (size_t i = 0; i < varOccurs.size(); i++)
        buildConstraintExactlyVariable(id, list, values[i], varOccurs[i]);
}

void XCSP3Callbacks::buildConstraintCardinality(string id, vector<XVariable*>& list, vector<int> values, vector<XInterval>& intervalOccurs, bool closed)
{
    if (intervalOccurs.size() != values.size())
        throw std::logic_error("Cardinality: Occurs and values must have the same size");

    for (size_t i = 0; i < intervalOccurs.size(); i++) {
        buildConstraintAtLeast(id, list, values[i], intervalOccurs[i].min);
        buildConstraintAtMost(id, list, values[i], intervalOccurs[i].max);
    }
}

void XCSP3Callbacks::buildConstraintElement(string id, vector<int>& list, int startIndex, XVariable* index, RankType rank, XVariable* value)
{
    vector<Variable*> vars({ getMyVar(index), getMyVar(value) });
    vector<vector<ind>> tuples;

    Variable* idx = vars[0];
    Variable* val = vars[1];

    for (int a = 0; a < idx->domainCurSize; ++a) {
        int v = idx->getVarPropFromLocalDomInd(a).val - startIndex;
        vector<ind> tmp;
        if (v >= 0 && v < (int)list.size() && val->isValidValue(list[v])) {
            tmp.push_back(idx->getVarPropIndFromValue(v + startIndex));
            tmp.push_back(val->getVarPropIndFromValue(list[v]));
        }
        tuples.push_back(tmp);
    }

    Constraint* newCont = new ConstraintExtN(id, vars, tuples);

    vecCont.push_back(newCont);
    vecBtConst.push_back(newCont);
}

void XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable*>& list, int startIndex, XVariable* i, RankType rank, int value)
{
    list.push_back(i);

    vector<Variable*> vars;
    toMyVariables(list, vars);

    vecCont.push_back(new ConstraintElementConstant(id, vars, vars.size() - 1, value, startIndex));
}

void XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable*>& list, int startIndex, XVariable* i, RankType rank, XVariable* v)
{
    list.push_back(i);
    list.push_back(v);

    vector<Variable*> vars;
    toMyVariables(list, vars);

    vecCont.push_back(new ConstraintElementVariable(id, vars, vars.size() - 2, startIndex));
}

void XCSP3Callbacks::buildConstraintElement(string id, vector<XVariable*>& list, int value)
{
    buildConstraintAtLeast(id, list, value, 1);
}

void XCSP3Callbacks::buildConstraintChannel(string id, vector<XVariable*>& list1, int startIndex1, vector<XVariable*>& list2, int startIndex2)
{
    if (startIndex1) {
        std::cout << "s UNSUPPORTED" << endl;
        throw runtime_error("Unsupported Channel with startIndex1");
    }

    vector<Variable*> X;
    vector<Variable*> Y;
    toMyVariables(list1, X);
    toMyVariables(list2, Y);

    for (size_t i = 0; i < X.size(); i++) {
        vector<Variable*> vars(Y);
        vars.push_back(X[i]);
        vecCont.push_back(new ConstraintElementConstant(id, vars, vars.size() - 1, i, startIndex2));
    }
}

void XCSP3Callbacks::buildConstraintChannel(string id, vector<XVariable*>& list, int startIndex, XVariable* value)
{
    int i = 0;
    for (XVariable* x : list) {
        string tmp = "iff(" + x->id + "," + "eq(" + value->id + "," + std::to_string(i) + "))";
        i++;
        buildConstraintIntension(id, new XCSP3Core::Tree(tmp));
    }
}

void XCSP3Callbacks::buildConstraintInstantiation(string id, vector<XVariable*>& list, vector<int>& values)
{
    assert(list.size() == values.size());

    for (size_t i = 0; i < list.size(); i++)
        vecCont.push_back(new ConstraintExtUnary(id, getMyVar(list[i]), values[i], true));
}

void show_help()
{
    cerr << "####################################" << endl;
    cerr << "#               HELP               #" << endl;
    cerr << "####################################" << endl;
    cerr << endl;
    cerr << "Advanced help with \"-moreHelp\" (Options, etc.)" << endl;
    cerr << endl;
    cerr << "    ./cspSolver BENCHNAME.xml method {-options} " << endl;
    cerr << "    methods:  " << endl;
    cerr << "       -complete" << endl;
    cerr << "       -incng" << endl;
    cerr << "       -nld" << endl;
    cerr << "       -ngen" << endl;
    cerr << "       -ca" << endl;
    cerr << endl;
    cerr << "Print bench details: " << endl;
    cerr << "    ./cspSolver BENCHNAME -infosCSP " << endl;
    cerr << endl;
} // show_help

void show_advanced_help()
{
    cerr << "####################################" << endl;
    cerr << "#     ADVANCED HELP -- OPTIONS     #" << endl;
    cerr << "####################################" << endl;
    cerr << endl;
    cerr << endl;
    cerr << "Solve XCSP3: (compile with \"make\")" << endl;
    cerr << "    ./cspSolver BENCHNAME.xml method {-options} " << endl;
    cerr << "    methods:  " << endl;
    cerr << "       -complete (standard mac)" << endl;
    cerr << "       -incng    (Increasing Nogoods -- Lee et al. 2014/2016)" << endl;
    cerr << "       -nld      (Negative Last Decision Nogoods -- Lecoutre et al. 2007)" << endl;
    cerr << "       -ngen     (Generalized Nogood from Restart)" << endl;
    cerr << "       -ca       (Conflict Analysis -- SAT-like)" << endl;
    cerr << endl;
    cerr << endl;
    cerr << "Options:" << endl;
    cerr << endl;
    cerr << "    Heuristics -- Values" << endl;
    cerr << "       -valMin   => minimum in domain " << endl;
    cerr << "       -valMax   => maximum in domain " << endl;
    cerr << "       -valRand  => random in domain " << endl;
    cerr << "       -valFirst => first one in domain sparse set " << endl;
    cerr << "       -valLast  => last one in domain sparse set " << endl;
    cerr << endl;
    cerr << "    Heuristics -- Variables" << endl;
    cerr << "       -dom " << endl;
    cerr << "       -domdeg " << endl;
    cerr << "       -domwdeg/-dwd " << endl;
    cerr << endl;
    cerr << "    Heuristics -- Miscellaneous" << endl;
    cerr << "       -saving   => Tries to assign a var at the same value as before" << endl;
    cerr << "       -refutation " << endl;
    cerr << endl;
    cerr << "    Miscellaneous" << endl;
    cerr << "       -sols     => Count Solutions (complete/nld) " << endl;
    cerr << "       -verb=n   => n in 0..3, Verbosity level (default 1)" << endl;
    cerr << endl;
} // show_help

void print_infos()
{
    cout << endl
         << "#####################################" << endl
         << " Type: " << ((typeCSP == CSP) ? "CSP" : ((typeCSP == COP) ? "COP" : "UNKNOWN")) << endl
         << " Vars: " << mapping.size() << endl
         << " VarProps: " << Variable::varProps.size() << endl
         << " Constraints: " << vecCont.size() << endl
         << " Arity: " << (Stats::minArity > Stats::maxArity ? 0 : Stats::minArity) << ".." << Stats::maxArity << endl
         << "#####################################" << endl;
}

/**
Function to create a solver.
*/
Solver* create_solver(int seed, int argc, char** argv)
{
    int method = -1;

    if (Options::load_options(argc, argv, method)) {
        show_help();
        exit(0);
    }

    problem->init(vecCont, vecBtConst);

    if (method == 1)
        return new CompleteSolver(problem);
    if (method == 2)
        return new IncNGCompleteSolver(problem);
    if (method == 3)
        return new GeneralizedCompleteSolver(problem);
    if (method == 4)
        return new LecoutreCompleteSolver(problem);
    if (method == 5)
        return new ConflictAnalysisSolver(problem);
    if (method == 10) {
        print_infos();
        exit(0);
    }

    show_help();
    exit(0);
}

#endif //ARCH_XCSP3Callbacks_H
