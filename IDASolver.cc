#include <algorithm>
#include <unordered_set>
#include <iostream>

#include "IDASolver.hh"

using namespace TaquinSolve;

/**
 * Constructor.
 * Takes a string representing a new puzzle and it's intended size.
 *
 * @param board_string  A string representing a puzzle e.g. "2 3 1 0".
 * @param board_size    The size of the given puzzle board e.g 2 for the example above.
 */
IDASolver::IDASolver(std::vector<size_t> board, size_t board_size)
{
    this->initial_board = std::shared_ptr<Board>(new Board(board, board_size));
}

/**
 * Solve the board state given to this object.
 * Uses an A* search algorithm.
 *
 * @return  A queue structure representing moves taken to reach the solution.
 */
std::queue<Moves> IDASolver::solve()
{
    //Ensure the given board state is valid
    this->initial_board->validate_state();

    size_t bound = this->initial_board->get_heuristic();

    while (true) {
        SearchResult result = this->search(this->initial_board, bound);
        if (result.solved) {
            return result.board->get_move_history();
        }
        if (result.cost == std::numeric_limits<std::size_t>::max()) {
            throw std::string("Puzzle is unsolvable.");
        }

        //this->visited_cache.clear();
        bound = result.cost;
    }
}

/**
 * Recursively search until the bound is reached.
 *
 * @param board The root to search from.
 * @oaram bound The bound to stop at.
 *
 * @return a struct representing either a solution or the lowest cost board which exceeded the bound.
 */
SearchResult IDASolver::search(std::shared_ptr<Board> board, size_t bound)
{
    size_t board_cost = board->get_cost() + board->get_heuristic();
    if (board_cost > bound) {
        return SearchResult(
            false,
            board_cost,
            board
        );
    }

    if (board->check_solved()) {
        return SearchResult(
            true,
            board_cost,
            board
        );
    }

    //Find the neighbors by applying each possible move
    std::vector< std::shared_ptr<Board> > neighbors = this->perform_moves(board.get(), board->get_available_moves());

    SearchResult min_result(
        false,
        std::numeric_limits<std::size_t>::max(),
        board
    );

    //Check each neighbor for new states or more efficient cost
    for (
        std::vector< std::shared_ptr<Board> >::iterator it = neighbors.begin();
        it != neighbors.end();
        ++it
    ) {
        std::shared_ptr<Board> neighbor = *it;
        SearchResult neighbor_result = this->search(neighbor, bound);
        if (neighbor_result.solved) {
            return neighbor_result;
        }

        if (neighbor_result.cost < min_result.cost) {
            min_result = neighbor_result;
        }
    }
    return min_result;
}

/**
 * Create a list of new boards by applied each of the given moves to the given board.
 *
 * @param board The reference board state.
 * @paran moves A list of moves to apply.
 */
std::vector< std::shared_ptr<Board> > IDASolver::perform_moves(Board *board, std::vector<Moves> moves) {
    std::vector< std::shared_ptr<Board> > results;

    for (Moves move : moves) {
        std::shared_ptr<Board> new_board = std::shared_ptr<Board>(board->perform_move(move));
        std::map<std::string, size_t>::iterator it = this->visited_cache.find(new_board->get_state_hash());
        size_t new_cost = new_board->get_cost() + new_board->get_heuristic();

        if (it != this->visited_cache.end()) {
            if ( new_cost > it->second) {
                continue;
            }
        }
        this->visited_cache.insert(std::pair<std::string, size_t>(new_board->get_state_hash(), new_cost));

        results.push_back(new_board);
    }

    std::sort (results.begin(), results.end(), [](std::shared_ptr<Board> l, std::shared_ptr<Board> r){
        return (l->get_cost() + l->get_heuristic()) < (r->get_cost() + r->get_heuristic());
    });
    return results;
}
