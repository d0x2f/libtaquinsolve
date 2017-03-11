#pragma once

#include <set>
#include <vector>
#include <map>
#include <memory>

#include "PartialBoard.hh"

namespace TaquinSolve
{
    class BFSDatabaseGenerator
    {
        public:
            BFSDatabaseGenerator(std::string output_file);
            void generate(std::vector<size_t> goal_board, std::set<size_t> group_tiles, size_t board_size);
            std::vector< std::shared_ptr<PartialBoard> > perform_moves(PartialBoard *board, std::vector<Moves> moves);
        protected:
            std::map<size_t, int> database;
            std::string output_file;

            void database_insert(size_t index, int value);
            bool database_key_exists(size_t index);
            int database_get_value(size_t index);
            void save_database();
    };
}
