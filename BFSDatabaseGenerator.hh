#pragma once

#include <vector>
#include <map>
#include <memory>

#include "PartialBoard.hh"

namespace TaquinSolve
{
    class BFSDatabaseGenerator
    {
        public:
            void generate(std::vector<size_t> goal_board, std::vector<size_t> group_tiles, size_t board_size);
            std::vector< std::shared_ptr<PartialBoard> > perform_moves(PartialBoard *board, std::vector<Moves> moves);
        protected:
            std::map<size_t, size_t> visited_cache;
    };
}
