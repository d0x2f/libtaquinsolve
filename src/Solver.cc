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
}

/**
 * Load a pattern database file into memory.
 *
 * @param path The path to find the database file.
 */
void Solver::load_database(std::string path)
{
    off_t length;

    //Buffer storage
    uint64_t hash;
    uint8_t cost;

    //File handle
    std::ifstream db(path, std::ios::binary);

    if (!db.is_open()) {
        throw std::string("Error: database file doesn't exist.\nMake sure you generate the pattern databases first.");
    }

    //Get file length
    db.seekg(0, std::ios::end);
    length = db.tellg();
    db.seekg(0, std::ios::beg);

    //Loop over the whole file and import each entry.
    off_t position = 0;
    while (position < length) {
        db.read((char *) &hash, 8);
        db.read((char *) &cost, 1);

        this->pattern_database->insert(std::pair<uint64_t, uint8_t>(hash, cost));

        position += 9;
    }
}

/**
 * Load the pattern databases.
 */
void Solver::load_pattern_database()
{
    if (this->pattern_database == NULL) {
        this->pattern_database = std::shared_ptr< std::map<uint64_t, uint8_t> >(new std::map<uint64_t, uint8_t>());

        this->load_database("/usr/local/share/libtaquinsolve/234.db.bin");
        this->load_database("/usr/local/share/libtaquinsolve/15691013.db.bin");
        this->load_database("/usr/local/share/libtaquinsolve/7811121415.db.bin");
    }
}
