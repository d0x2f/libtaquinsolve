#pragma once

#include <memory>
#include <set>

#include "Board.hh"

namespace TaquinSolve
{
    /**
     * Represents a board state with operations that affect it or describe it.
     */
    class PartialBoard : public Board
    {
        public:
            PartialBoard(
                std::vector<size_t> state,
                size_t board_size,
                int move_count
            );
            size_t get_partial_state_hash(std::shared_ptr<std::set<size_t> > group_tiles);
            int get_cost();
            PartialBoard *perform_move(Moves move) override;

        protected:
            int move_count = 0;
    };
}
