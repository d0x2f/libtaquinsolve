#include <iostream>
#include <algorithm>
#include <unordered_set>

#include "Solver.hh"

using namespace TaquinSolve;

/**
 * Constructor.
 * Takes a string representing a new puzzle and it's intended size.
 *
 * @param board_string  A string representing a puzzle e.g. "2 3 1 0".
 * @param board_size    The size of the given puzzle board e.g 2 for the example above.
 */
Solver::Solver(std::vector<size_t> board, size_t board_size)
{
    this->initial_board = new Board(board, board_size);
}

/**
 * Solve the board state given to this object.
 * Uses an A* search algorithm.
 *
 * @return  A queue structure representing moves taken to reach the solution.
 *          If the puzzle is unsolveable and empty queue is returned.
 */
std::queue<Moves> Solver::solve()
{
    //Ensure the given board state is valid
    if (!this->initial_board->validate_state()) {
        return std::queue<Moves>();
    }

    //Open set of unexplored board states
    std::map<std::string, std::shared_ptr<Board> > open;

    //Closed set of already explored states
    std::unordered_set<std::string> closed;

    //Create shared pointer to pass along
    //Scoped so it will delete
    {
        std::shared_ptr<Board> initial_board = std::shared_ptr<Board>(this->initial_board);
        open.insert(std::pair<std::string, std::shared_ptr<Board> >(this->initial_board->get_state_hash(), initial_board));
    }

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
        open.erase(current->get_state_hash());

        //Find the neighbors by applying each possible move
        std::vector< std::shared_ptr<Board> > neighbors = this->perform_moves(current.get(), current->get_available_moves());

        //Check each neighbor for new states or more efficient cost
        for (
            std::vector< std::shared_ptr<Board> >::iterator it = neighbors.begin();
            it != neighbors.end();
            ++it
        ) {
            std::shared_ptr<Board> neighbor = *it;
            std::string neighbor_hash = neighbor->get_state_hash();

            //If this state has been explored, skip it
            if (closed.find(neighbor_hash) != closed.end()) {
                continue;
            }

            //If this board is not already in the open set, then add it.
            //Otherwise replace the existing one with this one if it's cost is better.
            std::map<std::string, std::shared_ptr<Board> >::iterator existing_entry = open.find(neighbor_hash);
            if (existing_entry == open.end()) {
                existing_entry = open.insert(std::pair<std::string, std::shared_ptr<Board> >(neighbor_hash, neighbor)).first;
            } else if (neighbor->get_cost() > existing_entry->second->get_cost()) {
                continue;
            }

            //Replace the existing board by replacing it's move history.
            existing_entry->second->replace_move_history(neighbor->get_move_history());
        }
    }

    //The puzzle is unsolvable
    return std::queue<Moves>();
}

/**
 * Find the board state in the open set that has the cheapest cost + heuristic.
 *
 * @return A pointer to the cheapest board.
 */
std::shared_ptr<Board> Solver::get_cheapest_board(std::map<std::string, std::shared_ptr<Board> > *open_set)
{
    std::map<std::string, std::shared_ptr<Board> >::iterator it = open_set->begin();

    int lowest_found = it->second->get_cost() + it->second->get_heuristic();
    std::shared_ptr<Board> lowest_found_ref = it->second;
    int cost;

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
std::vector< std::shared_ptr<Board> > Solver::perform_moves(Board *board, std::vector<Moves> moves) {
    std::vector< std::shared_ptr<Board> > results;

    for (Moves move : moves) {
        results.push_back(std::shared_ptr<Board>(board->perform_move(move)));
    }

    return results;
}
