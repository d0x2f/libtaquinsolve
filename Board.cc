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
Board::Board(std::vector<size_t> state, size_t board_size, std::queue<Moves> move_history)
    : state(state), board_size(board_size), move_history(move_history)
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

    return new Board(new_state, this->board_size, new_history);
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

    std::string state_representation;
    for (
        std::vector<std::size_t>::iterator it = this->state.begin();
        it != this->state.end();
        ++it
    ) {
        state_representation += std::to_string(*it) + ":";
    }

    this->state_hash = std::hash<std::string>{}(state_representation);
    this->state_hash_dirty = false;

    return this->state_hash;
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

    this->heuristic = manhattan_sum;
    this->heuristic_dirty = false;

    return manhattan_sum;
}
