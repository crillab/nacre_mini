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
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#ifndef GENERALIZEDNOGOODSMANAGER_H_
#define GENERALIZEDNOGOODSMANAGER_H_

#include "Clauses.hh"
#include "ConsistencyHandler.hh"
#include "Stats.hh"

/**
   Manage nogoods at revision, backtrack and restarts
*/
class GeneralizedNoGoodsManager {
private:
    Clauses* handler;
    ConsistencyHandler* cspAC;

    inline void assign(unsigned lit)
    {
        Variable* v = Variable::varProps[lit >> 1].toVar;
        int pos = Variable::varProps[lit >> 1].posInVar;

        if (lit & 1)
            v->removeAt(pos, cspAC->getDecisionLevel());
        else
            v->assignAt(pos, cspAC->getDecisionLevel());

        cspAC->addToQueue(v);
    } // assign

public:
    GeneralizedNoGoodsManager(ConsistencyHandler* ac)
        : cspAC(ac)
    {
        handler = new Clauses();
    }

    GeneralizedNoGoodsManager(ConsistencyHandler* ac, Clauses* hand)
        : handler(hand)
        , cspAC(ac){};

    Clauses* getNgHandler()
    {
        return handler;
    }

    void restart(std::vector<std::vector<unsigned>>& denied, std::vector<unsigned>& decided);

    friend std::ostream& operator<<(std::ostream& out, const GeneralizedNoGoodsManager& lnm)
    {
        out << "c" << std::endl
            << "c # clauses = " << Stats::nbNoGoods << std::endl
            << "c # Ng supp = " << Stats::nbSupp << std::endl
            << "c # Ng conflicts = " << Stats::nbConflict << std::endl
            << "c # Unit clauses = " << Stats::unit << std::endl
            << "c # Unit(var) = " << Stats::unitVar << std::endl
            << "c # Bin(var) = " << Stats::binary;
        return out;
    }
};
#endif // GENERALIZEDNOGOODSMANAGER_H_
