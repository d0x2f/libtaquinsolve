#include <assert.h>
#include <stdlib.h>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_generate_standard_pattern_db()
{
    generate_standard_pattern_databases();
}

static void test_generate_pattern_db()
{
    std::vector<uint8_t> goal_board = taquin_tokenise_board_string("1 2 3 0");
    std::set<uint8_t> group_tiles = {1,2,3};
    generate_pattern_database(goal_board, group_tiles, 2, "./123.db.bin");
}

int main (void)
{
    test_generate_standard_pattern_db();
    test_generate_pattern_db();

    return EXIT_SUCCESS;
}