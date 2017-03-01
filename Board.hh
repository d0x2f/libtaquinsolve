#include <vector>
#include <queue>
#include <string>
#include <functional>

namespace TaquinSolve
{
    /**
     * The set of all possible moves.
     */
    enum Moves {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    /**
     * Represents a board state with operations that affect it or describe it.
     */
    class Board
    {
        public:
            Board(std::vector<int> state, int board_size, std::queue<Moves> move_history = std::queue<Moves>());
            Board(const Board&) = delete;
            ~Board() = default;
            Board& operator=(const Board&) = delete;

            //Modify
            Board *perform_move(Moves move);
            void replace_move_history(std::queue<Moves> move_history);

            //Validate
            bool validate_state();
            bool check_solved();

            //Read
            std::vector<Moves> get_available_moves();
            std::queue<Moves> get_move_history();
            std::size_t get_state_hash();
            int get_cost();
            int get_heuristic();

        protected:
            //An int vector representation of the state
            std::vector<int> state;

            //The size of the board
            int board_size = 0;

            //The current position of the empty tile
            //(represented by 0)
            int zero_position = 0;

            //A queue structure that contains all the moves taken to get to this board state
            std::queue<Moves> move_history;
    };
}
