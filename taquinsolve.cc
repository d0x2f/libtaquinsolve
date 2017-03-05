#include <string>
#include <algorithm>
#include <sstream>

#include "taquinsolve.hh"
#include "IDASolver.hh"

/**
 * Generate a solvable puzzle with the given board size.
 *
 * @param board_size The desired board size.
 *
 * @return A random solvable board in string representation.
 */
std::string taquin_generate_string(size_t board_size)
{
    std::vector<size_t> board = taquin_generate_vector(board_size);

    std::string output;

    std::vector<size_t>::iterator it = board.begin();
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
std::vector<size_t> taquin_generate_vector(size_t board_size)
{
    //Create a uniform board of the required size
    size_t len = board_size * board_size;
    std::vector<size_t> board;

    for (size_t i=0; i<len; i++) {
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
bool taquin_check_solvable(std::string board_string, size_t board_size)
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
bool taquin_check_solvable(std::vector<size_t> board, size_t board_size)
{
    size_t len = board_size * board_size;
    size_t inversion_count = taquin_get_inversion_count(board, board_size);
    size_t zero_position = 0;
    for (size_t i=0; i<len; i++) {
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
size_t taquin_get_inversion_count(std::vector<size_t> board, size_t board_size)
{
    size_t len = board.size();
    size_t inversion_count = 0;
    for (size_t i=0; i<len; i++) {
        for (size_t j=i+1; j<len; j++) {
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
std::vector<size_t> taquin_tokenise_board_string(std::string str, char sep)
{
    std::vector<size_t> ret;

    std::istringstream stm(str);
    std::string token;
    while( std::getline( stm, token, sep ) ) {
        std::stringstream sstream(token);
        size_t result;
        sstream >> result;
        ret.push_back(result);
    }

    return ret;
}

/**
 * Solve the given puzzle string.
 *
 * @param board_string A board represented as a string e.g. "3 1 0 2"
 * @param board_size   The size of the given board e.g. 2 for the above example.
 *
 * @return A queue of moves taken to reach the solution.
 */
std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, size_t board_size)
{
    return taquin_solve(taquin_tokenise_board_string(board_string), board_size);
}

/**
 * Solve the given puzzle vector.
 *
 * @param board         A board represented as a vector.
 * @param board_size    The size of the given board
 *
 * @return A queue of moves taken to reach the solution.
 */
std::queue<TaquinSolve::Moves> taquin_solve(std::vector<size_t> board, size_t board_size)
{
    TaquinSolve::IDASolver solver(board, board_size);
    return solver.solve();
}

/**
 * I found this stub neccessary to satisfy an AC_CHECK_LIB macro in autotools.
 */
extern "C" int taquin_solve_c_stub() {}
