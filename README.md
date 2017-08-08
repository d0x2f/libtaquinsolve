# TaquinSolver

[![Build Status](https://travis-ci.org/d0x2f/libtaquinsolve.svg?branch=master)](https://travis-ci.org/d0x2f/libtaquinsolve)
[![Coverage Status](https://coveralls.io/repos/github/d0x2f/libtaquinsolve/badge.svg?branch=master)](https://coveralls.io/github/d0x2f/libtaquinsolve?branch=master)

C++ library for solving taquin picture puzzles

## TODO
* Look into optimising storage for the pattern databases (currently 9 bytes per entry).
    * The board state hash is 8 bytes (64 bits) with each 4 bits representing a position in the puzzle and it's value.
    * The board cost/heuristic is stored in 1 byte (max 255) which seems reasonable (cost max 80, but this uses 7 bits anyway).
    * Generates a set of databases totaling ~100MB or ~10MB when compressed.
* Include a packaged set of databases with the source to be installed in /usr/local/share/libtaquinsolve/ and the windows equivalent.
    * The pattern databases are available in pattern-databases.tar.xz for you to copy if you don't want to generate them.
    * Currently only looks in /usr/local/share/libtaquinsolve/, so this won't work on windows.

## Resources
* Additive pattern database papers
    * https://arxiv.org/abs/1107.0050
    * http://www.sciencedirect.com/science/article/pii/S0004370201000923
