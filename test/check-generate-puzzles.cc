#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_generate_3_3_puzzles()
{
    uint32_t solvable_count = 0;
    for (uint32_t i=0; i<100; i++) {
        std::vector<uint8_t> puzzle = taquin_generate_vector(3);
        solvable_count += taquin_check_solvable(puzzle, 3);
    }
    assert(solvable_count == 100);
}

static void test_generate_4_4_puzzles()
{
    uint32_t solvable_count = 0;
    for (uint32_t i=0; i<100; i++) {
        std::vector<uint8_t> puzzle = taquin_generate_vector(4);
        solvable_count += taquin_check_solvable(puzzle, 4);
    }
    assert(solvable_count == 100);
}

int main (void)
{
    struct timeval time;
    gettimeofday(&time, NULL);
    srand (((unsigned long long)time.tv_sec * 1000000) + time.tv_usec);

    test_generate_3_3_puzzles();
    test_generate_4_4_puzzles();

    return EXIT_SUCCESS;
}