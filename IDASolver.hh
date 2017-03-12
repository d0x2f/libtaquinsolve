#pragma once

#include <string>
#include <queue>

#include "Solver.hh"

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

    class IDASolver : public Solver
    {
        public:
            IDASolver();
            std::queue<Moves> solve(std::vector<size_t> board, size_t board_size);
            SearchResult search(std::shared_ptr<Board> board, size_t bound);
            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves);
        protected:
            std::map<size_t, size_t> visited_cache;
    };
}
