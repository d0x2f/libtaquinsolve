#include <assert.h>
#include <stdlib.h>

#include <taquinsolve.hh>

using namespace TaquinSolve;

static void test_generate_pattern_db()
{
    generate_standard_pattern_databases();
}

int main (void)
{
    test_generate_pattern_db();

    return EXIT_SUCCESS;
}