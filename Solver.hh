#include <string>
#include <vector>
#include <queue>
#include <map>
#include <memory>

#include "Board.hh"

namespace TaquinSolve
{
    class Solver
    {
        public:
            Solver(std::string board_string, int board_size);
            std::queue<Moves> solve();
            std::vector< std::shared_ptr<Board> > perform_moves(Board *board, std::vector<Moves> moves);

        protected:
            Board *initial_board;

            std::shared_ptr<Board> get_cheapest_board(std::map<std::size_t, std::shared_ptr<Board> > *open_set);
            static std::vector<int> tokenise_board_string(std::string str, char sep = ' ');
    };
}
