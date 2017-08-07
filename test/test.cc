#include <iostream>
#include <stdlib.h>
#include <sys/time.h>

#include <taquinsolve.hh>

using namespace TaquinSolve;

int main (int argc, char **argv)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand (((unsigned long long)time.tv_sec * 1000000) + time.tv_usec);

    try {
        //Generate the standard pattern database (used for 4x4 puzzles.
        generate_standard_pattern_databases();

        uint8_t board_size;
        uint32_t solvable_count;
        std::string solved_puzzle;
        std::string solvable_puzzle;
        std::string unsolvable_puzzle;
        std::string random_puzzle;

        /******************************
        *  BOARD SIZE 3
        ******************************/
        std::cout << "BOARD SIZE 3" << std::endl;
        board_size = 3;
        solvable_count = 0;
        for (uint32_t i=0; i<1000; i++) {
            std::vector<uint8_t> puzzle = taquin_generate_vector(board_size);
            solvable_count += taquin_check_solvable(puzzle, board_size);
        }
        std::cout << solvable_count << "/1000 generated puzzles solvable" << std::endl;

        solved_puzzle = "1 2 3 4 5 6 7 8 0";
        Board solved(taquin_tokenise_board_string(solved_puzzle), board_size);
        std::cout << "Solved test: " << taquin_check_solvable(solved_puzzle, board_size) << std::endl;
        std::cout << "\tInversions: " << (unsigned int)taquin_get_inversion_count(solved.get_state(), board_size) << std::endl;
        std::cout << "\tHeuristic: " << (unsigned int)solved.get_heuristic() << std::endl;
        std::cout << "\tHash: " << std::hex << solved.get_state_hash() << std::dec << std::endl;
        std::cout << "\tCheck Solved: " << solved.check_solved() << std::endl;

        solvable_puzzle = "4 5 7 2 8 0 6 1 3";
        unsolvable_puzzle = "5 4 7 2 8 0 6 1 3";
        std::cout << "Solvable test: " << taquin_check_solvable(solvable_puzzle, board_size) << std::endl;
        std::cout << "\tHeuristic: " << (unsigned int)Board(taquin_tokenise_board_string(solvable_puzzle), board_size).get_heuristic() << std::endl;
        std::cout << "\tMoves: " << taquin_solve(solvable_puzzle, board_size).size() << std::endl;
        std::cout << "Unsolvable test: " << taquin_check_solvable(unsolvable_puzzle, board_size) << std::endl;

        random_puzzle = taquin_generate_string(board_size);
        std::cout << std::endl;
        std::cout << "Test random: " << random_puzzle << std::endl;
        std::cout << "Solvable: " << taquin_check_solvable(random_puzzle, board_size) << std::endl;
        std::cout << "Try solve... " << std::endl;
        std::cout << "\tMoves: " << taquin_solve(random_puzzle, board_size).size() << std::endl;
        std::cout << "Done!" << std::endl << std::endl;

        //return 0;
        /******************************
        *  BOARD SIZE 4
        ******************************/
        std::cout << "BOARD SIZE 4" << std::endl;
        board_size = 4;
        solvable_count = 0;
        for (uint32_t i=0; i<1000; i++) {
            std::vector<uint8_t> puzzle = taquin_generate_vector(board_size);
            solvable_count += taquin_check_solvable(puzzle, board_size);
        }
        std::cout << solvable_count << "/1000 generated puzzles solvable" << std::endl;

        solved_puzzle = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0";
        Board solved_2(taquin_tokenise_board_string(solved_puzzle), board_size);
        std::cout << "Solved test: " << taquin_check_solvable(solved_puzzle, board_size) << std::endl;
        std::cout << "\tInversions: " << (unsigned int)taquin_get_inversion_count(solved_2.get_state(), board_size) << std::endl;
        std::cout << "\tHeuristic: " << (unsigned int)solved_2.get_heuristic() << std::endl;
        std::cout << "\tHash: " << std::hex << solved_2.get_state_hash() << std::dec << std::endl;
        std::cout << "\tCheck Solved: " << solved_2.check_solved() << std::endl;

        solvable_puzzle = "12 1 10 2 7 11 4 14 5 0 9 15 8 13 6 3";
        unsolvable_puzzle = "1 2 3 4 5 6 7 8 9 10 11 12 13 15 14 0";
        std::cout << "Solvable test: " << taquin_check_solvable(solvable_puzzle, board_size) << std::endl;
        std::cout << "\tInversions: " << (unsigned int)taquin_get_inversion_count(taquin_tokenise_board_string(solvable_puzzle), board_size) << std::endl;
        std::cout << "\tHeuristic: " << (unsigned int)Board(taquin_tokenise_board_string(solvable_puzzle), board_size).get_heuristic() << std::endl;
        std::cout << "\tMoves: " << taquin_solve(solvable_puzzle, board_size).size() << std::endl;
        std::cout << "Unsolvable test: " << taquin_check_solvable(unsolvable_puzzle, board_size) << std::endl;

        random_puzzle = taquin_generate_string(board_size);
        std::cout << std::endl;
        std::cout << "Test random: " << random_puzzle << std::endl;
        std::cout << "Solvable: " << taquin_check_solvable(random_puzzle, board_size) << std::endl;
        std::cout << "Try solve... " << std::endl;
        std::cout << "\tMoves: " << taquin_solve(random_puzzle, board_size).size() << std::endl;
        std::cout << "Done!" << std::endl;
    } catch (std::string error) {
        std::cout << error << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
