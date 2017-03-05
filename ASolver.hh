#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>

#include "Board.hh"

namespace TaquinSolve
{
    class ASolver
    {
        public:
            ASolver(std::vector<size_t> board, size_t board_size);
            std::queue<Moves> solve();
            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves);

        protected:
            Board *initial_board;

            std::shared_ptr<Board> get_cheapest_board(std::map<std::string, std::shared_ptr<Board> > *open_set);
    };
}
