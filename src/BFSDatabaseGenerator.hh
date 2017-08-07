#pragma once

#include <set>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>

#include "Board.hh"

namespace TaquinSolve
{
    class BFSDatabaseGenerator
    {
        public:
            void generate(std::vector<uint8_t> goal_board, std::set<uint8_t> group_tiles, uint8_t board_size, std::string output_file);

            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves, std::shared_ptr< std::set<uint8_t> > group_tiles = NULL);

        protected:
            std::set<uint64_t> visited;
            std::map<uint64_t, uint8_t> database;

            void database_clear();

            void database_insert(
                std::shared_ptr<Board> board,
                std::shared_ptr< std::set<uint8_t> > group_tiles_nozero,
                std::shared_ptr< std::set<uint8_t> > group_tiles
            );

            bool check_visited(uint64_t index);

            uint8_t database_get_value(uint64_t index);

            void save_database(std::string output_file);
    };
}
