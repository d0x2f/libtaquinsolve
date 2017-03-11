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
            void generate(std::vector<size_t> goal_board, std::set<size_t> group_tiles, size_t board_size, std::string output_file);

            std::vector< std::shared_ptr<PartialBoard> > perform_moves(PartialBoard *board, std::vector<Moves> moves);

        protected:
            std::set<size_t> visited;
            std::map<size_t, int> database;

            void database_clear();

            void database_insert(
                std::shared_ptr<PartialBoard> board,
                std::shared_ptr< std::set<size_t> > group_tiles_nozero,
                std::shared_ptr< std::set<size_t> > group_tiles
            );

            bool check_visited(size_t index);

            int database_get_value(size_t index);

            void save_database(std::string output_file);
    };
}
