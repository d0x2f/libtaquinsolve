#include <assert.h>
#include <stdlib.h>
#include <string>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_solve_solvable_3_3_puzzle()
{
    std::string solvable_puzzle = "4 5 7 2 8 0 6 1 3";
    Board solved(taquin_tokenise_board_string(solvable_puzzle), 3);

    //should be solvable
    assert(taquin_check_solvable(solvable_puzzle, 3));

    //Should have heuristic 17
    assert(Board(taquin_tokenise_board_string(solvable_puzzle), 3).get_heuristic() == 17);

    //should have 16 inversions
    assert(taquin_get_inversion_count(solved.get_state(), 3) == 16);

    //Should have state hash 0x316082754
    assert(solved.get_state_hash() == 0x316082754);

    //Should not currently be solved
    assert(!solved.check_solved());

    //Should find a solution with 27 moves
    assert(taquin_solve(solvable_puzzle, 3).size() == 27);
}

static void test_solve_solvable_4_4_puzzle()
{
    std::string solvable_puzzle = "12 1 10 2 7 11 4 14 5 0 9 15 8 13 6 3";
    Board solved(taquin_tokenise_board_string(solvable_puzzle), 4);

    //should be solvable
    assert(taquin_check_solvable(solvable_puzzle, 4));

    //Should have heuristic 39
    assert(solved.get_heuristic() == 39);

    //should have 49 inversions
    assert(taquin_get_inversion_count(solved.get_state(), 4) == 49);

    //Should have state hash 0x36d8f905e4b72a1c
    assert(solved.get_state_hash() == 0x36d8f905e4b72a1c);

    //Should not currently be solved
    assert(!solved.check_solved());

    //Should find a solution with 53 moves
    assert(taquin_solve(solvable_puzzle, 4).size() == 53);
}

int main (void)
{
    test_solve_solvable_3_3_puzzle();
    test_solve_solvable_4_4_puzzle();

    return EXIT_SUCCESS;
}