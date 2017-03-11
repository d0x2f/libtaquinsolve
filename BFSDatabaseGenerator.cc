#include <queue>
#include <limits>
#include <fstream>

#include "BFSDatabaseGenerator.hh"

using namespace TaquinSolve;

/**
 * Solve the board state given to this object.
 * Uses an Iterative Deepening A* search algorithm.

 * @param goal_board    The indented goal board that represents a sovled solution.
 * @param group_tiles   The set of tiles to consider for this database.
 * @param board_size    The size of the game board.
 * @param output_file   The file to write the generated database data to.
 *
 * @return  A queue structure representing moves taken to reach the solution.
 */
void BFSDatabaseGenerator::generate(std::vector<size_t> goal_board, std::set<size_t> group_tiles, size_t board_size, std::string output_file)
{
    std::queue<std::shared_ptr<PartialBoard> > frontier;

    std::shared_ptr< std::set<size_t> > group_tiles_nozero_ptr = std::shared_ptr< std::set<size_t> >(new std::set<size_t>(group_tiles));
    group_tiles.insert(0);
    std::shared_ptr< std::set<size_t> > group_tiles_ptr = std::shared_ptr< std::set<size_t> >(new std::set<size_t>(group_tiles));

    std::shared_ptr<PartialBoard> initial_board = std::shared_ptr<PartialBoard>(new PartialBoard(goal_board, board_size, 0));

    this->database_clear();
    this->database_insert(initial_board, group_tiles_nozero_ptr, group_tiles_ptr);
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
            if (this->check_visited(neighbor_hash)) {
                this->database_insert(neighbor, group_tiles_nozero_ptr, group_tiles_ptr);
                frontier.push(neighbor);
            }
        }
    }

    this->save_database(output_file);
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
 * Clear the database.
 */
void BFSDatabaseGenerator::database_clear()
{
    this->visited.clear();
    this->database.clear();
}

/**
 * Insert into the visited list.
 * Insert into the database only if it's cost is lower than
 * already found.
 *
 * @param board                 A board object.
 * @param group_tiles_nozero    A pointer to a list of board tiles in the group without 0 included.
 * @param group_tiles           The same list but including zero.
 */
void BFSDatabaseGenerator::database_insert(
    std::shared_ptr<PartialBoard> board,
    std::shared_ptr< std::set<size_t> > group_tiles_nozero,
    std::shared_ptr< std::set<size_t> > group_tiles
) {
    this->visited.insert(board->get_partial_state_hash(group_tiles));
    size_t db_index = board->get_partial_state_hash(group_tiles_nozero);

    if (board->get_cost() < this->database_get_value(db_index)) {
        this->database.insert(
            std::pair<size_t, int>(
                db_index,
                board->get_cost()
            )
        );
    }
}

/**
 * Check if the given board hash has been visited.
 *
 * @param index The key to search for.
 *
 * @return True or False.
 */
bool BFSDatabaseGenerator::check_visited(size_t index)
{
    return this->visited.find(index) == this->visited.end();
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
void BFSDatabaseGenerator::save_database(std::string output_file)
{
    std::ofstream file (output_file, std::ios::out | std::ios::binary);

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
