#include <assert.h>
#include <stdlib.h>
#include <string>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_solve_solved_3_3_puzzle()
{
    std::string solved_puzzle = "1 2 3 4 5 6 7 8 0";
    Board solved(taquin_tokenise_board_string(solved_puzzle), 3);

    //Should be solvable
    assert(taquin_check_solvable(solved_puzzle, 3));

    //Should have 0 inversions
    assert(taquin_get_inversion_count(solved.get_state(), 3) == 0);

    //Should have state hash 0x87654321
    assert(solved.get_state_hash() == 0x87654321);

    //Should be solved
    assert(solved.check_solved());
}

static void test_solve_solved_4_4_puzzle()
{
    std::string solved_puzzle = "1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 0";
    Board solved(taquin_tokenise_board_string(solved_puzzle), 4);

    //Should be solvable
    assert(taquin_check_solvable(solved_puzzle, 4));

    //Should have 0 inversions
    assert(taquin_get_inversion_count(solved.get_state(), 4) == 0);

    //Should have state hash 0xfedcba987654321
    assert(solved.get_state_hash() == 0xfedcba987654321);

    //Should be solved
    assert(solved.check_solved());
}

int main (void)
{
    test_solve_solved_3_3_puzzle();
    test_solve_solved_4_4_puzzle();

    return EXIT_SUCCESS;
}