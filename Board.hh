#pragma once

#include <vector>
#include <queue>
#include <string>
#include <map>
#include <set>
#include <memory>
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
            Board(
                std::vector<size_t> state,
                size_t board_size,
                std::shared_ptr< std::map<size_t, int> > pattern_database = NULL,
                std::queue<Moves> move_history = std::queue<Moves>()
            );
            Board(const Board&) = delete;
            ~Board() = default;
            Board& operator=(const Board&) = delete;

            //Modify
            virtual Board *perform_move(Moves move);
            void replace_move_history(std::queue<Moves> move_history);

            //Validate
            void validate_state();
            bool check_solved();

            //Read
            std::vector<Moves> get_available_moves();
            std::queue<Moves> get_move_history();
            std::vector<size_t> get_state();
            size_t get_state_hash();
            size_t get_partial_state_hash(std::shared_ptr<std::set<size_t> > group_tiles);
            int get_cost();
            int get_heuristic();
            int get_pattern_db_heuristic();

        protected:
            //An int vector representation of the state
            std::vector<size_t> state;

            //The size of the board
            size_t board_size = 0;

            //The current position of the empty tile
            //(represented by 0)
            size_t zero_position = 0;

            //Cached heuristic value
            int heuristic = 99;

            //Heuristic dirty flag
            bool heuristic_dirty = true;

            //Cached hash value
            size_t state_hash = 0;

            //Hash dirty flag
            bool state_hash_dirty = true;

            //A queue structure that contains all the moves taken to get to this board state
            std::queue<Moves> move_history;

            //A pointer to the pattern database given during construction.
            std::shared_ptr< std::map<size_t, int> > pattern_database;
    };
}
