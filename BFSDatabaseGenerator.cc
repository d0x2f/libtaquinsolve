#include <queue>
#include <limits>
#include <fstream>

#include "BFSDatabaseGenerator.hh"

using namespace TaquinSolve;

/**
 * Constructor.
 *
 * @param output_file   The file to write the generated database data to.
 */
BFSDatabaseGenerator::BFSDatabaseGenerator(std::string output_file)
    : output_file(output_file)
{
}

/**
 * Solve the board state given to this object.
 * Uses an Iterative Deepening A* search algorithm.
 *
 * @return  A queue structure representing moves taken to reach the solution.
 */
void BFSDatabaseGenerator::generate(std::vector<size_t> goal_board, std::set<size_t> group_tiles, size_t board_size)
{
    std::queue<std::shared_ptr<PartialBoard> > frontier;

    std::shared_ptr< std::set<size_t> > group_tiles_ptr = std::shared_ptr< std::set<size_t> >(new std::set<size_t>(group_tiles));

    std::shared_ptr<PartialBoard> initial_board = std::shared_ptr<PartialBoard>(new PartialBoard(goal_board, board_size, 0));

    this->database_insert(initial_board->get_partial_state_hash(group_tiles_ptr), 0);
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
            size_t neighbor_hash = neighbor->get_partial_state_hash(group_tiles_ptr);
            if (!this->database_key_exists(neighbor_hash)) {
                this->database_insert(neighbor_hash, neighbor->get_cost());
                frontier.push(neighbor);
            }
        }
    }

    this->save_database();
}

/**
 * Create a list of new boards by applied each of the given moves to the given board.
 *
 * @param board The reference board state.
 * @param moves A list of moves to apply.
 *
 * @return A list of new boards with the moves performed.
 */
std::vector< std::shared_ptr<PartialBoard> > BFSDatabaseGenerator::perform_moves(PartialBoard *board, std::vector<Moves> moves) {
    std::vector< std::shared_ptr<PartialBoard> > results;

    for (Moves move : moves) {
        std::shared_ptr<PartialBoard> new_board = std::shared_ptr<PartialBoard>(board->perform_move(move));
        results.push_back(new_board);
    }

    return results;
}

/**
 * Insert the given data into the database.
 *
 * @param index The key.
 * @param value The value.
 */
void BFSDatabaseGenerator::database_insert(size_t index, int value)
{
    this->database.insert(std::pair<size_t, int>(index, value));
}

/**
 * Check if the given key exists in the database.
 *
 * @param index The key to search for.
 *
 * @return True or False.
 */
bool BFSDatabaseGenerator::database_key_exists(size_t index)
{
    return this->database.find(index) != this->database.end();
}

/**
 * Returns the value for the entry with the given key.
 * If no entry is found, return max int.
 *
 * @param index The key to seach with.
 *
 * @return The value found with the given key.
 */
int BFSDatabaseGenerator::database_get_value(size_t index)
{
    std::map<size_t, int>::iterator it = this->database.find(index);
    if (it == this->database.end()) {
        return std::numeric_limits<int>::max();
    } else {
        return it->second;
    }
}

/**
 * Write the completed database to file as binary.
 */
void BFSDatabaseGenerator::save_database()
{
    std::ofstream file (this->output_file, std::ios::out | std::ios::binary);

    for (
        std::map<size_t, int>::iterator it = this->database.begin();
        it != this->database.end();
        ++it
    ) {
        file.write((char *)(&it->first), 8);
        file.write((char *)(&it->second), 4);
    }
    file.close();
}
