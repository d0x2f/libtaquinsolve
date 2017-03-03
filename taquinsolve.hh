#pragma once

#include <string>
#include "Solver.hh"

std::vector<int> taquin_tokenise_board_string(std::string str, char sep = ' ');

std::string taquin_generate_string(int board_size);
std::vector<int> taquin_generate_vector(int board_size);

bool taquin_check_solvable(std::vector<int> board, int board_size);
bool taquin_check_solvable(std::string board, int board_size);

std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, int board_size);
std::queue<TaquinSolve::Moves> taquin_solve(std::vector<int> board, int board_size);

extern "C" int taquin_solve_c_stub();
