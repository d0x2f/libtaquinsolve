#pragma once

#include <memory>
#include <vector>
#include <map>
#include <string>

#include "Board.hh"

namespace TaquinSolve
{
    class Solver
    {
        public:
            Solver();

            virtual std::queue<Moves> solve(std::vector<size_t> board, size_t board_size) = 0;
            virtual std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves) = 0;

        protected:
            std::shared_ptr< std::map<size_t, int> > pattern_database = NULL;

        private:
            void load_database(std::string path);
    };
}
