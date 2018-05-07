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
 * @CreateTime: Oct 2, 2017 3:28 PM
 * @Author: Gaël Glorian
 * @Contact: glorian@cril.fr
 * @Last Modified By: Gaël Glorian
 * @Last Modified Time: May 7, 2018 5:54 PM
 * @Description: Static class to get some informations about the solver process like CPU time and RAM used
 */

#ifndef INFOSYSTEM_H_
#define INFOSYSTEM_H_

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>

#if defined(__APPLE__) && defined(__MACH__)

#include <sys/time.h>
#include <sys/resource.h>

#endif 

class InfoSystem {
private:
    static double totalTime;

    static int parseLine(char* line)
    {
        // This assumes that a digit will be found and the line ends in " Kb".
        int i = strlen(line);
        const char* p = line;
        while (*p < '0' || *p > '9')
            p++;
        line[i - 3] = '\0';
        i = atoi(p);
        return i;
    }

public:
    /// Return the ram used by the program in KB
    static int getMemUsed()
    {
        #if defined(__APPLE__) && defined(__MACH__)
            struct rusage usage;
            if(0 == getrusage(RUSAGE_SELF, &usage))
                return usage.ru_maxrss/1000000; // MB
            else
                return -1;
        #else
            FILE* file = fopen("/proc/self/status", "r");
            int result = -1;
            char line[128];

            while (fgets(line, 128, file) != NULL) {
                if (strncmp(line, "VmRSS:", 6) == 0) {
                    result = parseLine(line);
                    break;
                }
            }
            fclose(file);
            return result;
        #endif 
    }

    /// Return total time elapsed
  static inline double timeElapsed() { return (double)clock() / CLOCKS_PER_SEC; }

    /// Return the time elapsed since last call
    static inline double elapsed_seconds()
    {
      double oldTime = totalTime;
        totalTime = timeElapsed();
        return totalTime - oldTime;
    }
};
#endif // INFOSYSTEM_H_
