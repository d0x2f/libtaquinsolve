#pragma once

#include <string>
#include <set>
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

void generate_pattern_database(std::vector<size_t> goal_board, std::set<size_t> group_tiles, size_t board_size, std::string output_file);
void generate_standard_pattern_databases();

extern "C" int taquin_solve_c_stub();
