#pragma once

#include <string>
#include <set>
#include <vector>
#include <queue>

namespace TaquinSolve
{
    /**
     * The set of all possible moves.
     */
    enum Moves {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum Algorithm
    {
        IDA
    };
}

std::vector<uint8_t> taquin_tokenise_board_string(std::string str, char sep = ' ');

std::string taquin_generate_string(uint8_t board_size);
std::vector<uint8_t> taquin_generate_vector(uint8_t board_size);

uint8_t taquin_get_inversion_count(std::vector<uint8_t> board, uint8_t board_size);

bool taquin_check_solvable(std::vector<uint8_t> board, uint8_t board_size);
bool taquin_check_solvable(std::string board, uint8_t board_size);

std::queue<TaquinSolve::Moves> taquin_solve(
    std::string board_string,
    uint8_t board_size,
    TaquinSolve::Algorithm algorithm = TaquinSolve::Algorithm::IDA
);

std::queue<TaquinSolve::Moves> taquin_solve(
    std::vector<uint8_t> board,
    uint8_t board_size,
    TaquinSolve::Algorithm algorithm = TaquinSolve::Algorithm::IDA
);

void generate_pattern_database(std::vector<uint8_t> goal_board, std::set<uint8_t> group_tiles, uint8_t board_size, std::string output_file);
void generate_standard_pattern_databases();

extern "C" int taquin_solve_c_stub();
