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
 * @CreateTime: Dec 14, 2017 4:49 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:56 PM
 * @Description: Modify Here, Please 
 */

#ifndef LECOUTRENOGOODSMANAGER_H_
#define LECOUTRENOGOODSMANAGER_H_

#include "ClausesPositive.hh"
#include "ConsistencyHandler.hh"
#include "LecoutreCompleteSolver.hh"
#include "Stats.hh"

/**
   Manage nogoods at revision, backtrack and restarts
*/
class LecoutreNoGoodsManager {
private:
    ClausesPositive* handler;
    ConsistencyHandler* cspAC;

public:
    LecoutreNoGoodsManager(ConsistencyHandler* ac)
        : cspAC(ac)
    {
        handler = new ClausesPositive();
    }

    ClausesPositive* getNgHandler()
    {
        return handler;
    }

    void blockSolution(std::vector<indVp>& solution) { handler->addClause(solution); }

    void restart(std::vector<std::vector<indVp>> negSet, std::vector<indVp>& posSet);

    friend std::ostream& operator<<(std::ostream& out, const LecoutreNoGoodsManager& lnm)
    {
        out << "c" << std::endl
            << "c # nogoods = " << Stats::nbNoGoods << std::endl
            << "c # Ng supp = " << Stats::nbSupp << std::endl
            << "c # Ng conflicts = " << Stats::nbConflict;
        return out;
    }
};
#endif // LECOUTRENOGOODSMANAGER_H_
