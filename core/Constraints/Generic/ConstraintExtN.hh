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
 * @CreateTime: Sep 28, 2017 8:37 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:58 PM
 * @Description: Modify Here, Please 
 */

#ifndef CONSTRAINTEXTN_H_
#define CONSTRAINTEXTN_H_

#include "Constraint.hh"
#include "Variable.hh"

struct ExtSave {
    int level;
    size_t sizeDom;
    std::vector<int> lastSize;
};

class ConstraintExtN : public Constraint {
private:
    std::vector<int> sSup;
    std::vector<int> sVal;

    std::vector<std::vector<indVp>> tuples;

    size_t currentLimit;

    std::stack<ExtSave> saveSize;
    std::vector<size_t> position;
    std::vector<int> lastSize;

    void restoreTuples(int level);
    void removeTuple(size_t tuplePos, int level);
    bool isValidTuple(std::vector<ind> tuple);

public:
    ConstraintExtN(std::string n, std::vector<Variable*> vars, std::vector<std::vector<ind>>& t);

    void backtrack(int level) { restoreTuples(level); }

    bool init();
    bool propagate(int level, Variable* cur, std::vector<Variable*>& touched);
};

#endif // CONSTRAINTEXTN_H_
