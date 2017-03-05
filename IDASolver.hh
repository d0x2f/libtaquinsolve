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
            IDASolver(std::vector<size_t> board, size_t board_size);
            std::queue<Moves> solve();
            SearchResult search(std::shared_ptr<Board> board, size_t bound);
            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves);
        protected:
            std::shared_ptr<Board> initial_board;
            std::map<std::string, size_t> visited_cache;

            std::shared_ptr<Board> get_cheapest_board(std::map<std::string, std::shared_ptr<Board> > *open_set);
    };
}
