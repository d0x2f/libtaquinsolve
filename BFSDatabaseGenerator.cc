#include <iostream>
#include <queue>

#include "BFSDatabaseGenerator.hh"

using namespace TaquinSolve;

/**
 * Solve the board state given to this object.
 * Uses an Iterative Deepening A* search algorithm.
 *
 * @return  A queue structure representing moves taken to reach the solution.
 */
void BFSDatabaseGenerator::generate(std::vector<size_t> goal_board, std::vector<size_t> group_tiles, size_t board_size)
{
    //Since we're starting a new database, clear the visited cache.
    this->visited_cache.clear();

    std::queue<std::shared_ptr<PartialBoard> > frontier;

    std::shared_ptr<PartialBoard> initial_board = std::shared_ptr<PartialBoard>(new PartialBoard(goal_board, board_size, std::queue<Moves>(), group_tiles));

    this->visited_cache.insert(std::pair<size_t, size_t>(initial_board->get_state_hash(), 0));
    frontier.push(initial_board);

    while (!frontier.empty()) {
        std::shared_ptr<PartialBoard> current = frontier.front();
        frontier.pop();

        //Find the neighbors by applying each possible move
        std::vector< std::shared_ptr<PartialBoard> > neighbors = this->perform_moves(current.get(), current->get_available_moves());

        for (
            std::vector< std::shared_ptr<PartialBoard> >::iterator it = neighbors.begin();
            it != neighbors.end();
            ++it
        ) {
            std::shared_ptr<PartialBoard> neighbor = *it;
            std::map<size_t, size_t>::iterator existing_entry = this->visited_cache.find(neighbor->get_state_hash());
            if (existing_entry == this->visited_cache.end() || neighbor->get_cost() < existing_entry->second) {
                this->visited_cache.insert(std::pair<size_t, size_t>(neighbor->get_state_hash(), neighbor->get_cost()));
                frontier.push(neighbor);
            }
        }

        std::cout << this->visited_cache.size() << std::endl;
    }
}

/**
 * Create a list of new boards by applied each of the given moves to the given board.
 *
 * @param board The reference board state.
 * @paran moves A list of moves to apply.
 */
std::vector< std::shared_ptr<PartialBoard> > BFSDatabaseGenerator::perform_moves(PartialBoard *board, std::vector<Moves> moves) {
    std::vector< std::shared_ptr<PartialBoard> > results;

    for (Moves move : moves) {
        std::shared_ptr<PartialBoard> new_board = std::shared_ptr<PartialBoard>((PartialBoard *)board->perform_move(move));
        results.push_back(new_board);
    }

    return results;
}
