#include <assert.h>
#include <stdlib.h>
#include <string>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_solve_unsolvable_3_3_puzzle()
{
    std::string unsolvable_puzzle = "5 4 7 2 8 0 6 1 3";
    Board solved(taquin_tokenise_board_string(unsolvable_puzzle), 3);

    //should not be solvable
    assert(!taquin_check_solvable(unsolvable_puzzle, 3));

    //Should have state hash 0x316082745
    assert(solved.get_state_hash() == 0x316082745);

    //Should not currently be solved
    assert(!solved.check_solved());

    //Should throw an exception when a solve is attempted
    bool exception_thrown = false;
    try {
        taquin_solve(unsolvable_puzzle, 3);
    } catch (std::string e) {
        exception_thrown = true;
    }
    assert(exception_thrown);
}

static void test_solve_unsolvable_4_4_puzzle()
{
    std::string unsolvable_puzzle = "1 2 3 4 5 6 7 8 9 10 11 12 13 15 14 0";
    Board solved(taquin_tokenise_board_string(unsolvable_puzzle), 4);

    //should not be solvable
    assert(!taquin_check_solvable(unsolvable_puzzle, 4));

    //Should have state hash 0x0efdcba987654321
    assert(solved.get_state_hash() == 0x0efdcba987654321);

    //Should not currently be solved
    assert(!solved.check_solved());

    //Should throw an exception when a solve is attempted
    bool exception_thrown = false;
    try {
        taquin_solve(unsolvable_puzzle, 4);
    } catch (std::string e) {
        exception_thrown = true;
    }
    assert(exception_thrown);
}

int main (void)
{
    test_solve_unsolvable_3_3_puzzle();
    test_solve_unsolvable_4_4_puzzle();

    return EXIT_SUCCESS;
}