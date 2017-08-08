#include <assert.h>
#include <stdlib.h>
#include <string>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_invalid_3_3_puzzle()
{
    std::string invalid_puzzle = "1 2 3 5 6 7 8 0";
    Board invalid(taquin_tokenise_board_string(invalid_puzzle), 3);

    //Should not be solvable
    assert(!taquin_check_solvable(invalid_puzzle, 3));

    //Should have 0 inversions
    assert(taquin_get_inversion_count(invalid.get_state(), 3) == 0);

    //Should have state hash 0x8765321
    assert(invalid.get_state_hash() == 0x8765321);

    //Should not be solved
    assert(!invalid.check_solved());

    //Should throw an exception when a solve is attempted
    bool exception_thrown = false;
    try {
        taquin_solve(invalid_puzzle, 3);
    } catch (std::string e) {
        exception_thrown = true;
    }
    assert(exception_thrown);
}

static void test_invalid_4_4_puzzle()
{
    std::string invalid_puzzle = "6 11 12 7 10 1 15 16 14 9 13 3 4 0";
    Board invalid(taquin_tokenise_board_string(invalid_puzzle), 4);

    //Should not be solvable
    assert(!taquin_check_solvable(invalid_puzzle, 4));

    //Should have 40 inversions
    assert(taquin_get_inversion_count(invalid.get_state(), 4) == 40);

    //Should have (invalid) state hash 0x043d9f0f1a7cb6
    assert(invalid.get_state_hash() == 0x043d9f0f1a7cb6);

    //Should not be solved
    assert(!invalid.check_solved());

    //Should throw an exception when a solve is attempted
    bool exception_thrown = false;
    try {
        taquin_solve(invalid_puzzle, 4);
    } catch (std::string e) {
        exception_thrown = true;
    }
    assert(exception_thrown);
}

static void test_invalid_1_1_puzzle()
{
    std::string invalid_puzzle = "0";
    Board invalid(taquin_tokenise_board_string(invalid_puzzle), 1);

    //Should not be solvable
    assert(!taquin_check_solvable(invalid_puzzle, 1));

    //Should have 0 inversions
    assert(taquin_get_inversion_count(invalid.get_state(), 1) == 0);

    //Should have state hash 0x0
    assert(invalid.get_state_hash() == 0x0);

    //Should be solved
    assert(invalid.check_solved());

    //Should throw an exception when a solve is attempted
    bool exception_thrown = false;
    try {
        taquin_solve(invalid_puzzle, 1);
    } catch (std::string e) {
        exception_thrown = true;
    }
    assert(exception_thrown);
}

int main (void)
{
    test_invalid_3_3_puzzle();
    test_invalid_4_4_puzzle();
    test_invalid_1_1_puzzle();

    return EXIT_SUCCESS;
}