#pragma once

#include <string>
#include <vector>
#include <queue>

#include "Board.hh"

std::vector<size_t> taquin_tokenise_board_string(std::string str, char sep = ' ');

std::string taquin_generate_string(size_t board_size);
std::vector<size_t> taquin_generate_vector(size_t board_size);

size_t taquin_get_inversion_count(std::vector<size_t> board, size_t board_size);

bool taquin_check_solvable(std::vector<size_t> board, size_t board_size);
bool taquin_check_solvable(std::string board, size_t board_size);

std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, size_t board_size);
std::queue<TaquinSolve::Moves> taquin_solve(std::vector<size_t> board, size_t board_size);

extern "C" int taquin_solve_c_stub();
