#pragma once

#include <string>
#include "Solver.hh"

std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, int board_size);
extern "C" int taquin_solve_c_stub();
