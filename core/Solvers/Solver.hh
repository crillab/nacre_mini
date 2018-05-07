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
 * @CreateTime: Oct 2, 2017 11:33 AM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Abstract class for solvers
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "ConsistencyHandler.hh"
#include "Problem.hh"

class Solver {
protected:
    Problem* problem;
    ConsistencyHandler* cspAC;

public:
    /// Constructor
    Solver(Problem* p, ConsistencyHandler* ch)
        : problem(p)
        , cspAC(ch){};

    inline void setConsistencyHandler(ConsistencyHandler* ch) { cspAC = ch; }
    inline ConsistencyHandler* getConsistencyHandler() { return cspAC; }

    /// Function used for initialization and preproccessing
    virtual bool init() = 0;

    /// Function used for the main loop of the solver
    virtual int search(int zeroing = 0) = 0;

    /// Call at the end of the execution or when interrupted, must print infos about solver
    virtual int showFinal(int res) = 0;

    /// Fonction used to launch the solver
    virtual int solve()
    {
        if (init())
            return 1;
        return search();
    }
};

#endif // SOLVER_H_
