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
        this->pattern_database = std::shared_ptr< std::map<uint64_t, uint8_t> >(new std::map<uint64_t, uint8_t>());

        this->load_database("/usr/local/share/libtaquinsolve/234.db.bin");
        this->load_database("/usr/local/share/libtaquinsolve/15691013.db.bin");
        this->load_database("/usr/local/share/libtaquinsolve/7811121415.db.bin");
    }
}

void Solver::load_database(std::string path)
{
    off_t length;

    //Buffer storage
    uint64_t hash;
    uint8_t cost;

    //File handle
    std::ifstream db_234(path, std::ios::binary);

    //Get file length
    db_234.seekg(0, std::ios::end);
    length = db_234.tellg();
    db_234.seekg(0, std::ios::beg);

    //Loop over the whole file and import each entry.
    off_t position = 0;
    while (position < length) {
        db_234.read((char *) &hash, 8);
        db_234.read((char *) &cost, 1);

        this->pattern_database->insert(std::pair<uint64_t, uint8_t>(hash, cost));

        position += 9;
    }
}
