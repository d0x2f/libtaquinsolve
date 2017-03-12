#include <fstream>
#include <iostream>

#include "Solver.hh"

using namespace TaquinSolve;

/**
 * Constructor.
 * Checks that the pattern databases exist and loads them.
 */
Solver::Solver()
{
    if (this->pattern_database == NULL) {
        this->pattern_database = std::shared_ptr< std::map<size_t, int> >(new std::map<size_t, int>());

        this->load_database("/usr/local/share/libtaquinsolve/234.db.bin");
        this->load_database("/usr/local/share/libtaquinsolve/15691013.db.bin");
        this->load_database("/usr/local/share/libtaquinsolve/7811121415.db.bin");
    }
}

void Solver::load_database(std::string path)
{
    size_t length;

    //Buffer storage
    size_t hash;
    int cost;

    //File handle
    std::ifstream db_234(path, std::ios::binary);

    //Get file length
    db_234.seekg(0, std::ios::end);
    length = db_234.tellg();
    db_234.seekg(0, std::ios::beg);

    //Loop over the whole file and import each entry.
    size_t position = 0;
    while (position < length) {
        db_234.read((char *) &hash, 8);
        db_234.read((char *) &cost, 4);

        this->pattern_database->insert(std::pair<size_t, int>(hash, cost));

        position += 12;
    }
}
