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
            Board(std::vector<std::size_t> state, std::size_t board_size, std::queue<Moves> move_history = std::queue<Moves>());
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
            std::size_t get_cost();
            std::size_t get_heuristic();

        protected:
            //An int vector representation of the state
            std::vector<std::size_t> state;

            //The size of the board
            std::size_t board_size = 0;

            //The current position of the empty tile
            //(represented by 0)
            std::size_t zero_position = 0;

            //A queue structure that contains all the moves taken to get to this board state
            std::queue<Moves> move_history;
    };
}
