#pragma once

#include <vector>
#include <map>
#include <memory>
#include <sqlite3.h>

#include "PartialBoard.hh"

namespace TaquinSolve
{
    class BFSDatabaseGenerator
    {
        public:
            void generate(std::vector<size_t> goal_board, std::vector<size_t> group_tiles, size_t board_size);
            std::vector< std::shared_ptr<PartialBoard> > perform_moves(PartialBoard *board, std::vector<Moves> moves);
        protected:
            sqlite3 *db;

            void create_database();
            void database_insert(size_t index, size_t value);
            bool database_key_exists(size_t index);
            size_t database_get_value(size_t index);
            void close_database();
    };
}
