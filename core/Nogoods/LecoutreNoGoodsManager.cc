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
 * @CreateTime: Dec 4, 2017 5:00 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:56 PM
 * @Description: Modify Here, Please 
 */

#include "LecoutreNoGoodsManager.hh"
#include "Options.hh"
#include "Variable.hh"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Add the new nogood, reinit each nogoods and clear the valuesMap.
 *
 * @param[in] sigma : the branch where the conflict happend.
 */
void LecoutreNoGoodsManager::restart(vector<vector<indVp>> negSet, vector<indVp>& posSet)
{
    vector<indVp> prefix;
    int saveNbNoGoods = Stats::nbNoGoods;

    assert(negSet[0].empty());
    assert(!cspAC->getDecisionLevel());

    for (size_t i = 0; i < posSet.size(); ++i) {
        prefix.push_back(posSet[i]);
        for (size_t j = 0; j < negSet[i + 1].size(); ++j) {
            vector<indVp> newNG(prefix);
            newNG.push_back(negSet[i + 1][j]);
            handler->addClause(newNG);
        }
    }

    fprintf(stderr, " %8llu | \n", Stats::nbNoGoods - saveNbNoGoods);
}
