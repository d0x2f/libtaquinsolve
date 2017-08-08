#include <string>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <experimental/filesystem>

#include "taquinsolve.hh"
#include "BFSDatabaseGenerator.hh"

/**
 * Generate a solvable puzzle with the given board size.
 *
 * @param board_size The desired board size.
 *
 * @return A random solvable board in string representation.
 */
std::string taquin_generate_string(uint8_t board_size)
{
    std::vector<uint8_t> board = taquin_generate_vector(board_size);

    std::string output;

    std::vector<uint8_t>::iterator it = board.begin();
    for(;it != board.end(); it++) {
        output += std::to_string(*it) + ' ';
    }

    return output;
}
/**
 * Generate a solvable puzzle with the given board size.
 *
 * @param board_size The desired board size.
 *
 * @return A random solvable board in vector representation.
 */
std::vector<uint8_t> taquin_generate_vector(uint8_t board_size)
{
    //Create a uniform board of the required size
    uint8_t len = board_size * board_size;
    std::vector<uint8_t> board;

    for (uint8_t i=0; i<len; i++) {
        board.push_back(i);
    }

    //Shuffle the board
    std::random_shuffle(board.begin(), board.end());

    //Check for solvability
    if (!taquin_check_solvable(board, board_size)) {
        //Modify if it's not solvable
        std::iter_swap(board.end()-3, board.end()-1);
    }

    return board;
}


/**
 * Check whether or not the given board is solvable.
 * Convenience function for passing a string representation.
 *
 * @param board         The board state represented as a string.
 * @param board_size    The width/height of the board.
 */
bool taquin_check_solvable(std::string board_string, uint8_t board_size)
{
    return taquin_check_solvable(taquin_tokenise_board_string(board_string), board_size);
}

/**
 * Check whether or not the given board is solvable.
 *
 * @param board         The board state.
 * @param board_size    The width/height of the board.
 *
 * @return True if the board is solvable.
 */
bool taquin_check_solvable(std::vector<uint8_t> board, uint8_t board_size)
{
    uint8_t len = board_size * board_size;
    uint8_t inversion_count = taquin_get_inversion_count(board, board_size);
    uint8_t zero_position = 0;
    for (uint8_t i=0; i<len; i++) {
        if (board.at(i) == 0) {
            zero_position = i;
            break;
        }
    }

    if (board_size % 2 == 0 && (zero_position/board_size) % 2 == 0) {
        return inversion_count % 2 == 1;
    }

    return inversion_count % 2 == 0;
}

/**
 * Returns the number of inversions in the given puzzle.
 * Can be used as a heuristic within the A* search.
 *
 * @param board         The board state.
 * @param board_size    The width/height of the board.
 *
 * @return the number of inversions
 */
uint8_t taquin_get_inversion_count(std::vector<uint8_t> board, uint8_t board_size)
{
    uint8_t len = board.size();
    uint8_t inversion_count = 0;
    for (uint8_t i=0; i<len; i++) {
        for (uint8_t j=i+1; j<len; j++) {
            if (board.at(j) !=0) {
                inversion_count += (board.at(i) > board.at(j));
            }
        }
    }
    return inversion_count;
}

/**
 * Take a board string like "0 1 3 2" and convert it into an int vector.
 *
 * @param str The board string representation.
 * @param sep The seperator used in the given string.
 *
 * @return A board state represented as a vector.
 */
std::vector<uint8_t> taquin_tokenise_board_string(std::string str, char sep)
{
    std::vector<uint8_t> ret;

    std::istringstream stm(str);
    std::string token;
    while( std::getline( stm, token, sep ) ) {
        std::stringstream sstream(token);
        unsigned int result;
        sstream >> result;
        ret.push_back((uint8_t)result);
    }

    return ret;
}

/**
 * Solve the given puzzle string.
 *
 * @param board_string              A board represented as a string e.g. "3 1 0 2"
 * @param board_size                The size of the given board e.g. 2 for the above example.
 * @param TaquinAlgorithm algorithm  The algorithm to use to solve the puzzle.
 *
 * @return A queue of moves taken to reach the solution.
 */
std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, uint8_t board_size, TaquinAlgorithm algorithm)
{
    return taquin_solve(taquin_tokenise_board_string(board_string), board_size, algorithm);
}

/**
 * Solve the given puzzle vector.
 *
 * @param board                     A board represented as a vector.
 * @param board_size                The size of the given board
 * @param TaquinAlgorithm algorithm  The algorithm to use to solve the puzzle.
 *
 * @return A queue of moves taken to reach the solution.
 */
std::queue<TaquinSolve::Moves> taquin_solve(std::vector<uint8_t> board, uint8_t board_size, TaquinAlgorithm algorithm)
{
    std::unique_ptr<TaquinSolve::Solver> solver;

    switch (algorithm) {
        case IDA:
        default:
            solver = std::make_unique<TaquinSolve::IDASolver>();
            break;
    }

    return solver->solve(board, board_size);
}

/**
 * Generate a pattern database based on the given goal using the given tile group.
 *
 * @param goal_board    A goal board represented as a vector.
 * @param group_tiles   The tiles to consider in this database.
 * @param board_size    The size of the given goal board.
 * @param output_file   The file to write the generated database data to.
 */
void generate_pattern_database(std::vector<uint8_t> goal_board, std::set<uint8_t> group_tiles, uint8_t board_size, std::string output_file)
{
    TaquinSolve::BFSDatabaseGenerator generator;
    generator.generate(goal_board, group_tiles, board_size, output_file);
}

/**
 * Generate a set of pattern databases using 6-6-3 partitioning.
 * Generated files are placed in /usr/local/share/libtaquinsolve
 *
 * @param goal_board    A goal board represented as a vector.
 * @param board_size    The size of the given goal board.
 */
void generate_standard_pattern_databases()
{
    std::experimental::filesystem::create_directory("/usr/local/share/libtaquinsolve");

    TaquinSolve::BFSDatabaseGenerator generator;
    std::vector<uint8_t> goal_board = taquin_tokenise_board_string("1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0");

    std::cout << "Generating 234.." << std::endl;
    std::set<uint8_t> group_tiles = {2,3,4};
    generator.generate(goal_board, group_tiles, 4, "/usr/local/share/libtaquinsolve/234.db.bin");

    std::cout << "Generating 15671013.." << std::endl;
    group_tiles = {1,5,6,9,10,13};
    generator.generate(goal_board, group_tiles, 4, "/usr/local/share/libtaquinsolve/15691013.db.bin");

    std::cout << "Generating 7811121415.." << std::endl;
    group_tiles = {7,8,11,12,14,15};
    generator.generate(goal_board, group_tiles, 4, "/usr/local/share/libtaquinsolve/7811121415.db.bin");
}

/**
 * I found this stub neccessary to satisfy an AC_CHECK_LIB macro in autotools.
 */
extern "C" int taquin_solve_c_stub() {}
