#include <string>
#include <algorithm>
#include <sstream>

#include "taquinsolve.hh"

/**
 * Generate a solvable puzzle with the given board size.
 *
 * @param board_size The desired board size.
 *
 * @return A random solvable board in string representation.
 */
std::string taquin_generate_string(int board_size)
{
    std::vector<int> board = taquin_generate_vector(board_size);

    std::string output;

    std::vector<int>::iterator it = board.begin();
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
std::vector<int> taquin_generate_vector(int board_size)
{
    //Create a uniform board of the required size
    int len = board_size * board_size;
    std::vector<int> board;

    for (int i=0; i<len; i++) {
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
bool taquin_check_solvable(std::string board_string, int board_size)
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
bool taquin_check_solvable(std::vector<int> board, int board_size)
{
    int len = board_size * board_size;
    int subject;
    int inversion_count = 0;
    int zero_position = 0;
    for (int i=0; i<len; i++) {
        subject = board.at(i);

        if (subject == 0) {
            zero_position = i;
            continue;
        }

        for (int j=i+1; j<len; j++) {
            inversion_count += (subject > board.at(j));
        }
    }

    if (board_size % 2 == 0 && (zero_position/board_size) % 2 == 0) {
        inversion_count ++;
    }

    return inversion_count % 2 == 0;
}

/**
 * Take a board string like "0 1 3 2" and convert it into an int vector.
 *
 * @param str The board string representation.
 * @param sep The seperator used in the given string.
 *
 * @return A board state represented as a vector.
 */
std::vector<int> taquin_tokenise_board_string(std::string str, char sep)
{
    std::vector<int> ret;

    std::istringstream stm(str);
    std::string token;
    while( std::getline( stm, token, sep ) ) {
        ret.push_back(std::stoi(token));
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
std::queue<TaquinSolve::Moves> taquin_solve(std::string board_string, int board_size)
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
std::queue<TaquinSolve::Moves> taquin_solve(std::vector<int> board, int board_size)
{
    TaquinSolve::Solver solver(board, board_size);
    return solver.solve();
}

/**
 * I found this stub neccessary to satisfy an AC_CHECK_LIB macro in autotools.
 */
extern "C" int taquin_solve_c_stub() {}
