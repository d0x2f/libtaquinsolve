#include "taquinsolve.hh"

std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, int board_size)
{
    TaquinSolve::Solver solver(board_string, board_size);
    return solver.solve();
}
