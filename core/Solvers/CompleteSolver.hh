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
 * @CreateTime: Oct 2, 2017 1:53 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:56 PM
 * @Description: Implementation of Solver.hh for a complete MAC-based approach
 */

#ifndef COMPLETESOLVER_H_
#define COMPLETESOLVER_H_

#include "ConsistencyHandler.hh"
#include "Problem.hh"
#include "Restart.hh"
#include "Solver.hh"
#include "Stats.hh"

class CompleteSolver : public Solver {
protected:
    Restart rst;

    indDomLocal PickValue(Variable* cur);
    Variable* PickVariable();

    Variable* heuristicDom();
    Variable* heuristicDomDeg();
    Variable* heuristicDomWDeg();

public:
    /// Constructor
    CompleteSolver(Problem* p)
        : Solver(p, new ConsistencyHandler(p))
    {
        auto seed = time(0);
        if (Options::Verbose != verbose::no)
            std::cout << "c seed : " << seed << std::endl;
        srand(seed);
    };

    CompleteSolver(Problem* p, ConsistencyHandler* ch)
        : Solver(p, ch)
    {
        auto seed = time(0);
        if (Options::Verbose != verbose::no)
            std::cout << "c seed : " << seed << std::endl;
        srand(seed);
    };

    /// Perform AC once, return true if UNSAT.
    virtual bool init()
    {
        return cspAC->init();
    }

    /// Exhaustive research based on MAC, return 0 if the problem is satisfiable, 1 if it is unsatisfiable.
    virtual int search(int zeroing = 0);

    /// Display a lot of information on the solver and an instantiation if the problem is found SAT
    virtual int showFinal(int res);
};
#endif // COMPLETESOLVER_H_
