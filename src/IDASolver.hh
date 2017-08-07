#pragma once

#include <string>
#include <queue>
#include <cstdint>

#include "Solver.hh"

namespace TaquinSolve
{
    struct SearchResult {
        bool solved;
        uint8_t cost;
        std::shared_ptr<Board> board;

        SearchResult(bool solved, uint8_t cost, std::shared_ptr<Board> board)
            : solved(solved), cost(cost), board(board)
        {
        }
    };

    class IDASolver : public Solver
    {
        public:
            IDASolver();
            std::queue<Moves> solve(std::vector<uint8_t> board, uint8_t board_size);
            SearchResult search(std::shared_ptr<Board> board, uint32_t bound);
            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves);
        protected:
            std::map<uint64_t, uint8_t> visited_cache;
    };
}
