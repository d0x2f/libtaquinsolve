#pragma once

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>

#include "Board.hh"

namespace TaquinSolve
{
    struct SearchResult {
        bool solved;
        size_t cost;
        std::shared_ptr<Board> board;

        SearchResult(bool solved, size_t cost, std::shared_ptr<Board> board)
            : solved(solved), cost(cost), board(board)
        {
        }
    };

    class IDASolver
    {
        public:
            std::queue<Moves> solve(std::vector<size_t> board, size_t board_size);
            SearchResult search(std::shared_ptr<Board> board, size_t bound);
            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves);
        protected:
            std::map<std::string, size_t> visited_cache;
    };
}
