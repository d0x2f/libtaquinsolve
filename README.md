# TaquinSolver
C++ library for solving taquin picture puzzles

## TODO
* Look into optimising storage for the pattern databases (currently 9 bytes per entry).
* Only generate the databases if they haven't been already (check hash).
* Make use of the pattern databases for 4x4 puzzles.
    * Is now being used, but generating non-optimal results.
    * Only load the pattern databases for 4x4 puzzles (currently loading for all).
* Include a packaged set of databases with the source to be installed in /usr/local/share and the windows equivalent.

## Resources
* Additive pattern database papers
    * https://arxiv.org/abs/1107.0050
    * http://www.sciencedirect.com/science/article/pii/S0004370201000923
