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
 * @CreateTime: Oct 2, 2017 1:44 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:55 PM
 * @Description: Implementation of Restart.hh
 */

#include "Restart.hh"
#include "Variable.hh"

void Restart::Inc()
{
    /* Variable::gblOrder = 0; */
    nbRst++;
    switch (Options::optRst) {
    case rstTypes::hundred_perc:
        nbConflictBfRst += nbConflictBfRst;
        break;
    case rstTypes::luby10:
        nbConflictBfRst = (Luby(nbRst + 1) * 10);
        break;
    case rstTypes::luby100:
        nbConflictBfRst = (Luby(nbRst + 1) * 100);
        break;
    case rstTypes::luby50:
        nbConflictBfRst = (Luby(nbRst + 1) * 50);
        break;
    case rstTypes::low:
        nbConflictBfRstD *= 1.03;
        nbConflictBfRst = static_cast<int>(floor(nbConflictBfRstD));
        break;
    case rstTypes::ten_perc:
        nbConflictBfRstD *= 1.1;
        nbConflictBfRst = static_cast<int>(floor(nbConflictBfRstD));
        break;
    default:
        nbConflictBfRst += nbConflictBfRst >> 1;
        break;
    }
}

int Restart::Luby(int i)
{
    int k = 1;
    int power = 2;

    // let 2^k be the least power of 2 >= (i+1)
    for (; power < (i + 1); k += 1, power *= 2)
        ;

    if (power == i + 1)
        return power / 2;
    return Luby(i - (power / 2) + 1);
}
