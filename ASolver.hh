#pragma once

#include <string>
#include <queue>

#include "Solver.hh"

namespace TaquinSolve
{
    class ASolver : public Solver
    {
        public:
            ASolver();

            std::queue<Moves> solve(std::vector<size_t> board, size_t board_size);
            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves);

        protected:
            std::shared_ptr<Board> get_cheapest_board(std::map<size_t, std::shared_ptr<Board> > *open_set);
    };
}
