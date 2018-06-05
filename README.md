# NACRE: Nogood And Clause Reasoning Engine

## Description

NACRE is a constraint solver written in C++. The main purpose of this solver is
to experiment nogood recording (with a clause reasoning engine) in Constraint
Programming (CP). In particular, the data structures of the solver have been
carefully designed to play around nogoods and clauses. This is the first version
of the solver and it has been submitted to the XCSP3 2018 competition under the
CSP MiniTrack.

## Usage

Go to the *core* folder and use the following line:

    ./make

You can add `-j` for parallel compilation. It will compile both the parser and solver.

The executable will be in that folder under the name `nacre_mini_release`

For help, call the executable without any parameters, use the following option for details:

    -moreHelp

Even for default configuration you need to specify the resolution method. In NACRE, there are currently 4 methods:

* ``-complete``: standard MAC approach
* ``-incng``: Increasing Nogoods (Increasing Nogoods in Restart-Based Search, Jimmy H. M. Lee, Christian Schulte,Zichen Zhu, 2016)
* ``-nld``: Negative last-decision nogoods (Recording and minimizing nogoods from restarts, Christophe Lecoutre, Lakhdar Sais, Sébastien Tabary, Vincent Vidal, 2007)
* ``-ca``: Conflict analysis, SAT-based lazy explanations

## Acknowledgements

This work has been supported by the project CPER Data from the region ”
Hauts-de-France”.

This solver is produced for my (Gaël Glorian) PhD thesis work on data structures
designed for clause-based nogood recording.

A big thank-you to Gilles Audemard, for his support and help.