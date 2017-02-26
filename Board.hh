#include <vector>
#include <queue>
#include <string>
#include <functional>

namespace TaquinSolve
{
    enum Moves {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    class Board
    {
        public:
            Board(std::vector<int> state, int board_size, std::queue<Moves> move_history = std::queue<Moves>());
            Board(const Board&) = delete;
            ~Board() = default;
            Board& operator=(const Board&) = delete;

            bool validate_state();
            std::vector<Moves> get_available_moves();
            Board *perform_move(Moves move);
            bool check_solved();
            std::queue<Moves> get_move_history();
            void replace_move_history(std::queue<Moves> move_history);
            std::size_t get_state_hash();
            int get_cost();
            int get_heuristic();

        protected:
            std::vector<int> state;
            int board_size = 0;
            int zero_position = 0;
            std::queue<Moves> move_history;
    };
}
