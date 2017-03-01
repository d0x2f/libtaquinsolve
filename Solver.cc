#include <iostream>
#include <sstream>
#include <algorithm>

#include "Solver.hh"

using namespace TaquinSolve;

/**
 * Constructor.
 * Takes a string representing a new puzzle and it's intended size.
 *
 * @param board_string  A string representing a puzzle e.g. "0 2 3 1".
 * @param board_size    The size of the given puzzle board e.g 2 for the example above.
 */
Solver::Solver(std::string board_string, int board_size)
{
    this->initial_board = new Board(Solver::tokenise_board_string(board_string), board_size);
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
    std::map<std::size_t, std::shared_ptr<Board> > open;

    //Closed set of already explored states
    std::map<std::size_t, bool > closed;

    //Create shared pointer to pass along
    //Scoped so it will delete
    {
        std::shared_ptr<Board> initial_board = std::shared_ptr<Board>(this->initial_board);
        open.insert(std::pair<std::size_t, std::shared_ptr<Board> >(this->initial_board->get_state_hash(), initial_board));
    }

    //Continue to check open states until there are none left
    while(!open.empty()) {
        //Get the best cost board in the open set
        std::shared_ptr<Board> current = this->get_cheapest_board(&open);

        //Check if it's solved
        if (current->check_solved()) {
            return current->get_move_history();
        }

        //Remove from the open set (since it's now explored)
        open.erase(open.find(current->get_state_hash()));

        //And so add it to the closed set
        closed.insert(std::pair<std::size_t, bool>(current->get_state_hash(), true));

        //Find the neighbors by applying each possible move
        std::vector< std::shared_ptr<Board> > neighbors = this->perform_moves(current.get(), current->get_available_moves());

        //Check each neighbor for new states or more efficient cost
        for (
            std::vector< std::shared_ptr<Board> >::iterator it = neighbors.begin();
            it != neighbors.end();
            ++it
        ) {
            std::shared_ptr<Board> neighbor = *it;
            std::size_t neighbor_hash = neighbor->get_state_hash();

            //If this state has been explored, skip it
            if (closed.find(neighbor_hash) != closed.end()) {
                continue;
            }

            //If this board is not already in the open set, then add it.
            //Otherwise replace the existing one with this one if it's cost is better.
            std::map<std::size_t, std::shared_ptr<Board> >::iterator existing_entry = open.find(neighbor_hash);
            if (existing_entry == open.end()) {
                open.insert(std::pair<std::size_t, std::shared_ptr<Board> >(neighbor_hash, neighbor));
                existing_entry = open.find(neighbor_hash);
            } else if (current->get_cost() + 1 >= existing_entry->second->get_cost()) {
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
std::shared_ptr<Board> Solver::get_cheapest_board(std::map<std::size_t, std::shared_ptr<Board> > *open_set)
{
    std::map<std::size_t, std::shared_ptr<Board> >::iterator it = open_set->begin();
    int lowest_found = it->second->get_cost() + it->second->get_heuristic();
    std::shared_ptr<Board> lowest_found_ref = it->second;
    int cost;

    it++;

    while (it != open_set->end()) {
        cost = it->second->get_cost() + it->second->get_heuristic();
        if (cost < lowest_found) {
            lowest_found = cost;
            lowest_found_ref = it->second;
        }
        it++;
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

/**
 * Take a board string like "0 1 3 2" and convert it into an int vector.
 *
 * @param str The board string representation.
 * @param sep The seperator used in the given string.
 */
std::vector<int> Solver::tokenise_board_string(std::string str, char sep)
{
    std::vector<int> ret;

    std::istringstream stm(str);
    std::string token;
    while( std::getline( stm, token, sep ) ) {
        ret.push_back(std::stoi(token));
    }

    return ret;
}
