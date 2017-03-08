#pragma once

#include <memory>

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
                size_t move_count,
                std::shared_ptr<std::vector<size_t> > group_tiles
            );

            size_t get_state_hash() override;
            size_t get_cost();
            PartialBoard *perform_move(Moves move) override;

        protected:
            size_t move_count = 0;
            std::shared_ptr<std::vector<size_t> > group_tiles;
    };
}
