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
 * @Description: Restart handler
 */

#ifndef RESTART_H_
#define RESTART_H_

#include "Options.hh"
#include "Stats.hh"

#include <math.h>

/**
   Handle restart policy according to a parameter
*/
class Restart {
private:
    int nbRst;
    int nbConflictBfRst;
    double nbConflictBfRstD;
    int Luby(int i);

public:
    ///Constructor
    Restart()
        : nbRst(0)
        , nbConflictBfRst(Options::baseRst)
        , nbConflictBfRstD(static_cast<double>(Options::baseRst)){};

    ///Increase @nbRst
    void Inc();

    ///Return the number of allowed conflict before a restart occurs
    int GetNbConflictAllowed() { return nbConflictBfRst; };
};
#endif // RESTART_H_
