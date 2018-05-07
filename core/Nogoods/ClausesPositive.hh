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
 * @CreateTime: Dec 4, 2017 3:48 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:57 PM
 * @Description: Modify Here, Please 
 */

#ifndef CLAUSESPOSITIVES_H_
#define CLAUSESPOSITIVES_H_

#include <vector>

#include "Variable.hh"

class ClausesPositive {
private:
    std::vector<std::vector<ind>> clauses;
    std::vector<unsigned>* watched;

    size_t searchAnotherWatch(std::vector<ind>& cl);

public:
    /** Constructor */
    ClausesPositive()
    {
        watched = new std::vector<unsigned>[Variable::varProps.size()];
    }

    /** Propagator for nld-nogoods, do something only if @v is assigned */
    int propagate(int level, Variable* cur, std::vector<Variable*>& touched);

    /** Add the clause @cl to the base and init the watched literals */
    void addClause(std::vector<indVp>& cl);

    /** Return he number of nogoods recorded */
    size_t getNbClauses() { return clauses.size(); }
};
#endif //CLAUSESPOSITIVES_H_
