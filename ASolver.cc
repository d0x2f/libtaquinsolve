#include <algorithm>
#include <unordered_set>

#include "ASolver.hh"

using namespace TaquinSolve;

ASolver::ASolver() : Solver() {}

/**
 * Solve the board state given to this object.
 * Uses an A* search algorithm.
 *
 * @return  A queue structure representing moves taken to reach the solution.
 */
std::queue<Moves> ASolver::solve(std::vector<uint8_t> board, uint8_t board_size)
{
    //Check if the board size is 4 and load the pattern db if it is.
    if (board_size == 4) {
        this->load_pattern_database();
    }

    std::shared_ptr<Board> initial_board = std::shared_ptr<Board>(new Board(board, board_size));

    //Ensure the given board state is valid
    initial_board->validate_state();

    //Open set of unexplored board states
    std::map<uint64_t, std::shared_ptr<Board> > open;

    //Closed set of already explored states
    std::unordered_set<uint64_t> closed;

    open.insert(std::pair<uint64_t, std::shared_ptr<Board> >(initial_board->get_state_hash(), initial_board));

    //Continue to check open states until there are none left
    while(!open.empty()) {
        //Get the best cost board in the open set
        std::shared_ptr<Board> current = this->get_cheapest_board(&open);

        //Check if it's solved
        if (current->check_solved()) {
            return current->get_move_history();
        }

        //And so add it to the closed set
        closed.insert(current->get_state_hash());

        //Remove from the open set (since it's now explored)
        open.erase(open.find(current->get_state_hash()));

        //Find the neighbors by applying each possible move
        std::vector< std::shared_ptr<Board> > neighbors = this->perform_moves(current.get(), current->get_available_moves());

        //Check each neighbor for new states or more efficient cost
        for (
            std::vector< std::shared_ptr<Board> >::iterator it = neighbors.begin();
            it != neighbors.end();
            ++it
        ) {
            std::shared_ptr<Board> neighbor = *it;
            uint64_t neighbor_hash = neighbor->get_state_hash();

            //If this state has been explored, skip it
            if (closed.find(neighbor_hash) != closed.end()) {
                continue;
            }

            //If this board is not already in the open set, then add it.
            //Otherwise replace the existing one with this one if it's cost is better.
            std::map<uint64_t, std::shared_ptr<Board> >::iterator existing_entry = open.find(neighbor_hash);
            if (existing_entry == open.end()) {
                open.insert(std::pair<uint64_t, std::shared_ptr<Board> >(neighbor_hash, neighbor));
            } else if (neighbor->get_cost() < existing_entry->second->get_cost()) {
                //Replace the existing board by replacing it's move history.
                existing_entry->second->replace_move_history(neighbor->get_move_history());
            }
        }
    }

    //The puzzle is unsolvable
    throw std::string("The puzzle in unsolvable");
}

/**
 * Find the board state in the open set that has the cheapest cost + heuristic.
 *
 * @return A pointer to the cheapest board.
 */
std::shared_ptr<Board> ASolver::get_cheapest_board(std::map<uint64_t, std::shared_ptr<Board> > *open_set)
{
    if (open_set->size() < 1) {
        throw std::string("No elements in open set");
    }

    std::map<uint64_t, std::shared_ptr<Board> >::iterator it = open_set->begin();

    uint8_t lowest_found = it->second->get_cost() + it->second->get_heuristic();
    std::shared_ptr<Board> lowest_found_ref = it->second;
    uint8_t cost;

    it++;

    for (;it != open_set->end(); it++) {
        cost = it->second->get_cost() + it->second->get_heuristic();
        if (cost < lowest_found) {
            lowest_found = cost;
            lowest_found_ref = it->second;
        }
    }

    return lowest_found_ref;
}

/**
 * Create a list of new boards by applied each of the given moves to the given board.
 *
 * @param board The reference board state.
 * @paran moves A list of moves to apply.
 */
std::vector< std::shared_ptr<Board> > ASolver::perform_moves(Board *board, std::vector<Moves> moves) {
    std::vector< std::shared_ptr<Board> > results;

    for (Moves move : moves) {
        results.push_back(std::shared_ptr<Board>(board->perform_move(move)));
    }

    return results;
}
