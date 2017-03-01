#include "taquinsolve.hh"

/**
 * Solve the given puzzle string.
 *
 * @param board_string A board represented as a string e.g. "3 1 0 2"
 * @param board_size   The size of the given board e.g. 2 for the above example.
 */
std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, int board_size)
{
    TaquinSolve::Solver solver(board_string, board_size);
    return solver.solve();
}

/**
 * I found this stub neccessary to satisfy an AC_CHECK_LIB macro in autotools.
 */
extern "C" int taquin_solve_c_stub() {}
