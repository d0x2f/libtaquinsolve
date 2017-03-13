#include <algorithm>
#include <unordered_set>
#include <iostream>

#include "IDASolver.hh"

using namespace TaquinSolve;

IDASolver::IDASolver() : Solver() {}

/**
 * Solve the board state given to this object.
 * Uses an Iterative Deepening A* search algorithm.
 *
 * @return  A queue structure representing moves taken to reach the solution.
 */
std::queue<Moves> IDASolver::solve(std::vector<uint8_t> board, uint8_t board_size)
{
    //Since we're starting a new solve, clear the visited cache.
    this->visited_cache.clear();

    std::shared_ptr<Board> initial_board = std::shared_ptr<Board>(new Board(board, board_size, this->pattern_database));

    //Ensure the given board state is valid
    initial_board->validate_state();

    uint32_t bound = initial_board->get_heuristic();

    while (true) {
        SearchResult result = this->search(initial_board, bound);
        if (result.solved) {
            return result.board->get_move_history();
        }
        if (result.cost == std::numeric_limits<std::uint8_t>::max()) {
            throw std::string("Puzzle is unsolvable.");
        }

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
SearchResult IDASolver::search(std::shared_ptr<Board> board, uint32_t bound)
{
    uint8_t board_cost = board->get_cost() + board->get_heuristic();

    //If this board cost is above the bound return it.
    if (board_cost > bound) {
        return SearchResult(
            false,
            board_cost,
            board
        );
    }

    //If this board is solved return it.
    if (board->check_solved()) {
        return SearchResult(
            true,
            board_cost,
            board
        );
    }

    //Find the neighbors by applying each possible move
    std::vector< std::shared_ptr<Board> > neighbors = this->perform_moves(board.get(), board->get_available_moves());

    //Find the neighbor with the minimum search() value
    SearchResult min_result(
        false,
        std::numeric_limits<uint8_t>::max(),
        board
    );

    for (
        std::vector< std::shared_ptr<Board> >::iterator it = neighbors.begin();
        it != neighbors.end();
        ++it
    ) {
        std::shared_ptr<Board> neighbor = *it;
        SearchResult neighbor_result = this->search(neighbor, bound);

        //If this neighbor produced a solved state, return it.
        if (neighbor_result.solved) {
            return neighbor_result;
        }

        //Check if this neighbor is the new minimum
        if (neighbor_result.cost < min_result.cost) {
            min_result = neighbor_result;
        }
    }

    //Return the minimum found result.
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
        std::map<uint64_t, uint8_t>::iterator it = this->visited_cache.find(new_board->get_state_hash());
        uint8_t new_cost = new_board->get_cost() + new_board->get_heuristic();

        if (it != this->visited_cache.end()) {
            if ( new_cost > it->second) {
                continue;
            }
        }
        this->visited_cache.insert(std::pair<uint64_t, uint8_t>(new_board->get_state_hash(), new_cost));

        results.push_back(new_board);
    }

    std::sort (results.begin(), results.end(), [](std::shared_ptr<Board> l, std::shared_ptr<Board> r){
        return (l->get_cost() + l->get_heuristic()) < (r->get_cost() + r->get_heuristic());
    });
    return results;
}
