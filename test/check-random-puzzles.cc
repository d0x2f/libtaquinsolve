#include <assert.h>
#include <stdlib.h>
#include <string>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_solve_random_3_3_puzzle()
{
    std::string random_puzzle = taquin_generate_string(3);

    //should be solvable
    assert(taquin_check_solvable(random_puzzle, 3));

    //Should find a solution within 31 moves
    assert(taquin_solve(random_puzzle, 3).size() <= 31);
}

static void test_solve_random_4_4_puzzle()
{
    std::string random_puzzle = taquin_generate_string(4);

    //should be solvable
    assert(taquin_check_solvable(random_puzzle, 4));

    //Should find a solution within 80 moves
    assert(taquin_solve(random_puzzle, 4).size() <= 80);
}

int main (void)
{
    test_solve_random_3_3_puzzle();
    test_solve_random_4_4_puzzle();

    return EXIT_SUCCESS;
}