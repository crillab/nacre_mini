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
 * @CreateTime: Dec 14, 2017 2:53 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#ifndef INCNG_H_
#define INCNG_H_

#include "Variable.hh"

#include <map>
#include <set>
#include <vector>

typedef struct
{
    int m;
    int alpha;
    int beta;
    int level;
} incNgNode;

/**
   A compressed storage for increasing nogoods
*/
class IncNG {
public:
    ///Vector of incNgNode, usefull for backtrack
    std::vector<incNgNode> incNgList;
    ///The positive varProp of the incNg
    std::vector<indVp> pos;
    ///The negative varProp of the incNg
    std::vector<std::vector<indVp>> neg;

    /// If the nogood is deleted
    bool deleted;
    /// The level of deletion
    int dlevel;
    /// A stamp for revision
    unsigned long long stamp;
    /// The map between the vp and the watched
    int* watchMap;
    int mapSize;

    //Constructor
    IncNG(std::vector<indVp> p, std::vector<std::vector<indVp>> n, int mapSz)
        : pos(p)
        , neg(n)
        , deleted(false)
        , dlevel(-1)
        , stamp(-1)
        , mapSize(mapSz)
    {
        incNgList.push_back({ (int)pos.size(), -1, -1, 0 });
        watchMap = new int[mapSize];
    }

    ///Current Alpha getter
    inline int alpha() { return incNgList.back().alpha; }
    ///Current Alpha setter
    inline void setAlpha(int a) { incNgList.back().alpha = a; }

    ///Current Beta getter
    inline int beta() { return incNgList.back().beta; }
    ///Current Beta setter
    inline void setBeta(int b) { incNgList.back().beta = b; }

    ///Current branch size getter
    inline int m() { return incNgList.back().m; }
    ///Current branch size setter
    inline void setM(int m) { incNgList.back().m = m; }

    ///Current level getter
    inline int level() { return incNgList.back().level; }
    ///Current level setter
    inline void setLevel(int level) { incNgList.back().level = level; }
};
#endif // INCNG_H_
