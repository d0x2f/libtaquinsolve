#pragma once

#include <vector>
#include <queue>
#include <string>
#include <map>
#include <set>
#include <memory>
#include <functional>
#include <cstdint>

#include "taquinsolve.hh"

namespace TaquinSolve
{
    /**
     * Represents a board state with operations that affect it or describe it.
     */
    class Board
    {
        public:
            Board(
                std::vector<uint8_t> state,
                uint8_t board_size,
                std::shared_ptr< std::map<uint64_t, uint8_t> > pattern_database = NULL,
                std::queue<Moves> move_history = std::queue<Moves>()
            );
            Board(const Board&) = delete;
            ~Board() = default;
            Board& operator=(const Board&) = delete;

            //Modify
            virtual Board *perform_move(Moves move, std::shared_ptr<std::set<uint8_t> > group_tiles = NULL);
            void replace_move_history(std::queue<Moves> move_history);

            //Validate
            void validate_state();
            bool check_solved();

            //Read
            std::vector<Moves> get_available_moves();
            std::queue<Moves> get_move_history();
            std::vector<uint8_t> get_state();
            uint64_t get_state_hash();
            uint64_t get_partial_state_hash(std::shared_ptr<std::set<uint8_t> > group_tiles);
            uint8_t get_cost();
            uint8_t get_heuristic();
            uint8_t get_pattern_db_heuristic();

        protected:
            //An int vector representation of the state
            std::vector<uint8_t> state;

            //The size of the board
            uint8_t board_size = 0;

            //The current position of the empty tile
            //(represented by 0)
            uint8_t zero_position = 0;

            //Cached heuristic value
            uint8_t heuristic = 99;

            //Heuristic dirty flag
            bool heuristic_dirty = true;

            //Cached hash value
            uint64_t state_hash = 0;

            //Hash dirty flag
            bool state_hash_dirty = true;

            //A queue structure that contains all the moves taken to get to this board state
            std::queue<Moves> move_history;

            //A pointer to the pattern database given during construction.
            std::shared_ptr< std::map<uint64_t, uint8_t> > pattern_database;
    };
}
