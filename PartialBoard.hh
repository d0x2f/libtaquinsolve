#pragma once

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
                std::queue<Moves> move_history = std::queue<Moves>(),
                std::vector<size_t> group_tiles = std::vector<size_t>()
            );

            size_t get_state_hash() override;
            PartialBoard *perform_move(Moves move) override;

        protected:
            std::vector<size_t> group_tiles;
    };
}
