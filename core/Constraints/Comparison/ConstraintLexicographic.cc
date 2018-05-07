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
 * @CreateTime: Nov 7, 2017 4:04 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:59 PM
 * @Description: Modify Here, Please 
 */
#include <set>

#include "ConstraintLexicographic.hh"

using namespace std;

vector<Variable*> ConstraintLexicographic::createScope(vector<Variable*> l1, vector<Variable*> l2)
{
    set<Variable*> varsSet(l1.begin(), l1.end());
    copy(l2.begin(), l2.end(), inserter(varsSet, varsSet.end()));
    return vector<Variable*>(varsSet.begin(), varsSet.end());
}

vector<int> ConstraintLexicographic::varsToScope(vector<Variable*> list)
{
    vector<int> ret;

    for (auto l : list) {
        auto f = find(scope.begin(), scope.end(), l);
        ret.push_back(f - scope.begin());
    }

    assert(ret.size() == list.size());
    return ret;
}

bool ConstraintLexicographic::isNotConsistentPair(int x, int v, int y, int w)
{
    time++;
    setRes(x, v);
    setRes(y, w);

    for (size_t i = alpha + 1; i < list1.size(); ++i) {
        int p1 = list1[i];
        int p2 = list2[i];

        assert(p1 >= 0 && p1 < (int)listSize);
        assert(p2 >= 0 && p2 < (int)listSize);

        int min1 = (times[p1] == time ? vals[p1] : scope[list1[i]]->getLowerBoundVal());
        int max2 = (times[p2] == time ? vals[p2] : scope[list2[i]]->getUpperBoundVal());

        if (min1 < max2)
            return false;
        if (min1 > max2)
            return true;

        setRes(p1, min1);
        setRes(p2, max2);
    }

    return strictOrdering;
}

bool ConstraintLexicographic::propagate(int level, Variable* cur, std::vector<Variable*>& touched)
{
    alpha = 0;

    while (alpha < list1.size()) {
        Variable* var1 = scope[list1[alpha]];
        Variable* var2 = scope[list2[alpha]];
        int before1 = var1->domainCurSize;
        int before2 = var2->domainCurSize;

        if (var1->removeValuesAbove(var2->getUpperBoundVal(), level, this) || var2->removeValuesUnder(var1->getLowerBoundVal(), level, this))
            return true;

        if (before1 != var1->domainCurSize)
            touched.push_back(var1);
        if (before2 != var2->domainCurSize)
            touched.push_back(var2);

        if (var1->isAssigned() && var2->isAssigned() && var1->getLowerBoundVal() == var2->getLowerBoundVal())
            alpha++;
        else
            break;
    }

    if (alpha >= list1.size())
        return strictOrdering;

    Variable* var1 = scope[list1[alpha]];
    Variable* var2 = scope[list2[alpha]];

    int min1 = var1->getLowerBoundVal();
    int min2 = var2->getLowerBoundVal();
    assert(min1 <= min2);
    if (min1 == min2 && isNotConsistentPair(list1[alpha], min1, list2[alpha], min2)) {
        int before = var2->domainCurSize;
        if (deleteValue(var2, var2->getLowerBoundLocalDomInd(), level))
            return true;
        if (before != var2->domainCurSize)
            touched.push_back(var2);
    }

    int max1 = var1->getUpperBoundVal();
    int max2 = var2->getUpperBoundVal();
    assert(max1 <= max2);
    if (max1 == max2 && isNotConsistentPair(list1[alpha], max1, list2[alpha], max2)) {
        int before = var1->domainCurSize;
        if (deleteValue(var1, var1->getUpperBoundLocalDomInd(), level))
            return true;
        if (before != var1->domainCurSize)
            touched.push_back(var1);
    }

    assert(var1->getLowerBoundVal() < var2->getUpperBoundVal());

    return false;
}
