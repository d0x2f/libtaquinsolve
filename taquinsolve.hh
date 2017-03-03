#pragma once

#include <string>
#include "Solver.hh"

std::vector<std::size_t> taquin_tokenise_board_string(std::string str, char sep = ' ');

std::string taquin_generate_string(std::size_t board_size);
std::vector<std::size_t> taquin_generate_vector(std::size_t board_size);

bool taquin_check_solvable(std::vector<std::size_t> board, std::size_t board_size);
bool taquin_check_solvable(std::string board, std::size_t board_size);

std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, std::size_t board_size);
std::queue<TaquinSolve::Moves> taquin_solve(std::vector<std::size_t> board, std::size_t board_size);

extern "C" int taquin_solve_c_stub();
