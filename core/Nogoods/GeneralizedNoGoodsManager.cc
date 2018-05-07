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
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#include "GeneralizedNoGoodsManager.hh"
#include "Clauses.hh"
#include "Options.hh"
#include "Variable.hh"

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Add the new nogood, reinit each nogoods and clear the valuesMap.
 * @param[in] sigma : the branch where the conflict happend.
 */
void GeneralizedNoGoodsManager::restart(vector<vector<unsigned>>& denied, vector<unsigned>& decided)
{
    vector<unsigned> cl;
    int saveNbNoGoods = Stats::nbNoGoods;

    for (size_t i = 0, stop = decided.size(); i < stop; i++) {
        cl.push_back(decided[i] ^ 1);
        vector<unsigned>& dv = denied[i + 1];

        for (size_t j = 0, stop_j = dv.size(); j < stop_j; j++) {
            cl.push_back(dv[j] ^ 1);

            if (cl.size() == 1) {
                assert(!cspAC->getDecisionLevel());
                assign(cl[0]);
                Stats::unit++;
            } else
                handler->addClause(cl);
            cl.pop_back();
        }
    }
    fprintf(stderr, " %8llu | \n", Stats::nbNoGoods - saveNbNoGoods);
} // restart
