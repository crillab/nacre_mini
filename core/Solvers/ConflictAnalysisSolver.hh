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

#ifndef CONFLICTANALYSISSOLVER_H_
#define CONFLICTANALYSISSOLVER_H_

#include "AnalysisConsistencyHandler.hh"
#include "GeneralizedCompleteSolver.hh"
#include "GeneralizedNoGoodsManager.hh"

class ConflictAnalysisSolver : public GeneralizedCompleteSolver {
private:
    ConflictAnalysis* confAnalysis;

public:
    /// Constructor
    ConflictAnalysisSolver(Problem* p)
        : GeneralizedCompleteSolver(p, new AnalysisConsistencyHandler(p))
    {
        manager = new GeneralizedNoGoodsManager(cspAC);
        confAnalysis = new ConflictAnalysis();

        cspAC->setGenHandler(manager->getNgHandler());
        confAnalysis->setHandler(manager->getNgHandler());
    };

    /// Exhaustive research based on MAC, return 0 if the problem is satisfiable, 1 if it is unsatisfiable.
    int search(int zeroing = 0);
};
#endif // CONFLICTANALYSISSOLVER_H_
