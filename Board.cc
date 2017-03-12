#include <string>
#include <algorithm>
#include <cstddef>

#include "Board.hh"
#include "taquinsolve.hh"

using namespace TaquinSolve;

/**
 * Constructor.
 * Takes on a board state, size and move history.
 * It finds the position of 0 within the board state.
 *
 * @param state         An array of integers representing the board state in row major encoding.
 * @param board_size    The width/height of the board.
 *                      The board state must contain board_size^2 entries.
 * @param move_history  A queue of moves taken to get to this board state.
 */
Board::Board(
    std::vector<size_t> state,
    size_t board_size,
    std::shared_ptr< std::map<size_t, int> > pattern_database,
    std::queue<Moves> move_history
) : state(state), board_size(board_size), pattern_database(pattern_database), move_history(move_history)
{
    //Find where the empty cell is
    for (size_t i = 0; i < this->state.size(); i++) {
        if (this->state[i] == 0) {
            this->zero_position = i;
        }
    }
}

/**
 * Validates the current board state.
 * Are there the right number of tiles?
 * Are the tiles indexed sequentially?
 */
void Board::validate_state()
{
    //Ensure we have the right number of positions
    if (this->state.size() != this->board_size * this->board_size) {
        throw std::string("Not enough tiles to fill board: ") + std::to_string(this->state.size()) + "/" + std::to_string(this->board_size * this->board_size);
    }

    //Sort the indices and make sure they are a sequence from 0.
    std::vector<size_t> sorted_state = this->state;
    std::sort(sorted_state.begin(), sorted_state.end());

    size_t last = -1;
    for (
        std::vector<size_t>::iterator it = sorted_state.begin();
        it != sorted_state.end();
        ++it
    ) {
        if ((*it) != last+1) {
            throw std::string("Improper sequence given, missing: ") + std::to_string(last+1);
        }
        last = (*it);
    }

    if (!taquin_check_solvable(this->state, this->board_size)) {
        throw std::string("Board state is unsolvable");
    }

    return;
}

/**
 * Return a list of the moves that can be taken from this board state.
 *
 * @return A vector of possible moves.
 */
std::vector<Moves> Board::get_available_moves()
{
    std::vector<Moves> output;

    //Get cartesian coordinates of the zero position
    size_t empty_x = this->zero_position % this->board_size;
    size_t empty_y = this->zero_position / this->board_size;

    //Check if zero is on any boundary
    if (empty_x > 0)
        output.push_back(Moves::LEFT);

    if (empty_x < this->board_size-1)
        output.push_back(Moves::RIGHT);

    if (empty_y > 0)
        output.push_back(Moves::UP);

    if (empty_y < this->board_size-1)
        output.push_back(Moves::DOWN);

    return output;
}

/**
 * Checks whether the current board state is solved.
 *
 * @return Solved or not
 */
bool Board::check_solved()
{
    size_t compare = 1;
    for (
        std::vector<size_t>::iterator it = this->state.begin();
        it != this->state.end()-1;
        ++it
    ) {
        if ((*it) != compare++) {
            return false;
        }
    }

    return true;
}

/**
 * Perform the given move upon a copy of this board and return it.
 *
 * @param move The move to apply.
 *
 * @return A new board object with the move applied.
 */
Board *Board::perform_move(Moves move)
{
    std::vector<size_t> new_state = this->state;
    switch (move) {
        case Moves::UP:
            new_state[this->zero_position] = new_state[this->zero_position - this->board_size];
            new_state[this->zero_position - this->board_size] = 0;
            break;

        case Moves::DOWN:
            new_state[this->zero_position] = new_state[this->zero_position + this->board_size];
            new_state[this->zero_position + this->board_size] = 0;
            break;

        case Moves::LEFT:
            new_state[this->zero_position] = new_state[this->zero_position - 1];
            new_state[this->zero_position - 1] = 0;
            break;

        case Moves::RIGHT:
            new_state[this->zero_position] = new_state[this->zero_position + 1];
            new_state[this->zero_position + 1] = 0;
            break;
    }
    std::queue<Moves> new_history;
    new_history = this->move_history;
    new_history.push(move);

    return new Board(new_state, this->board_size, this->pattern_database, new_history);
}

/**
 * Fetch the move history.
 *
 * @return Move history.
 */
std::queue<Moves> Board::get_move_history()
{
    return this->move_history;
}

/**
 * Replace the move history of this board state with the given one.
 * This is used in the search algorithm when the same board state is found by a more efficient route.
 *
 * @param move_history A new move history queue.
 */
void Board::replace_move_history(std::queue<Moves> move_history)
{
    this->move_history = move_history;
    this->heuristic_dirty = true;
}


std::vector<size_t> Board::get_state()
{
    return this->state;
}

/**
 * Create a unique hash of the board state to act as a simple
 * identifier.
 *
 * @return A unique hash of the board state.
 */
size_t Board::get_state_hash()
{
    if (!this->state_hash_dirty) {
        return this->state_hash;
    }

    size_t state_representation = 0;
    int i=0;
    for (
        std::vector<std::size_t>::iterator it = this->state.begin();
        it != this->state.end();
        ++it, i++
    ) {
        state_representation += (*it) << (i*4);
    }

    this->state_hash = state_representation;
    this->state_hash_dirty = false;

    return this->state_hash;
}

/**
 * Create a unique hash of the board state to act as a simple
 * identifier.
 * Since this is a partial board, only include the appropriate
 * tiles in the hash.
 *
 * @return A unique hash of the partial board state.
 */
size_t Board::get_partial_state_hash(std::shared_ptr<std::set<size_t> > group_tiles)
{
    size_t state_representation = 0;
    size_t unused_tile;

    //Find an unused tile to mark as a tile in the out group.
    for (int n : {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}) {
        std::set<size_t>::iterator tile = group_tiles->find(n);
        if (tile == group_tiles->end()) {
            unused_tile = n;
            break;
        }
    }

    //Loop over the state and add the tile to the representation in the right position.
    int i=0;
    for (
        std::vector<std::size_t>::iterator it = this->state.begin();
        it != this->state.end();
        ++it, i++
    ) {
        std::set<size_t>::iterator tile = group_tiles->find(*it);
        if (tile == group_tiles->end()) {
            state_representation += unused_tile << (i*4);
        } else {
            state_representation += (*it) << (i*4);
        }
    }
    return state_representation;
}

/**
 * Get the cost spent reaching this board state.
 * Consider one move as one cost unit.
 *
 * @return The number of moves taken.
 */
int Board::get_cost()
{
    return this->move_history.size();
}

/**
 * Returns a simple heuristic which measures how many tiles are currently misplaced.
 *
 * @return A heuristic value.
 */
int Board::get_heuristic()
{
    if (!this->heuristic_dirty) {
        return this->heuristic;
    }

    int pattern_database_heuristic = this->get_pattern_db_heuristic();

    int manhattan_sum = 0;
    for (
        int i = 0;
        i < this->state.size();
        i++
    ) {
        int j = this->state.at(i);
        if (j == 0) {
            continue;
        }

        manhattan_sum += abs((i / this->board_size) - ((j-1) / this->board_size)) + abs((i % this->board_size) - ((j-1) % this->board_size));
    }

    this->heuristic = std::max(pattern_database_heuristic, manhattan_sum);
    this->heuristic_dirty = false;

    return this->heuristic;
}


int Board::get_pattern_db_heuristic()
{
    if (this->board_size != 4 || this->pattern_database == NULL) {
        return 0;
    }

    int pattern_database_heuristic = 0;

    std::set<size_t> group_tiles = {2,3,4};
    std::map<size_t, int>::iterator search = this->pattern_database->find(this->get_partial_state_hash(std::shared_ptr< std::set<size_t> >(new std::set<size_t>(group_tiles))));

    if (search != this->pattern_database->end()) {
        pattern_database_heuristic += search->second;
    }

    group_tiles = {1,5,6,9,10,13};
    search = this->pattern_database->find(this->get_partial_state_hash(std::shared_ptr< std::set<size_t> >(new std::set<size_t>(group_tiles))));

    if (search != this->pattern_database->end()) {
        pattern_database_heuristic += search->second;
    }

    group_tiles = {7,8,11,12,14,15};
    search = this->pattern_database->find(this->get_partial_state_hash(std::shared_ptr< std::set<size_t> >(new std::set<size_t>(group_tiles))));

    if (search != this->pattern_database->end()) {
        pattern_database_heuristic += search->second;
    }

    return pattern_database_heuristic;
}
