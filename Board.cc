#include <iostream>
#include <cmath>
#include <algorithm>

#include "Board.hh"

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
Board::Board(std::vector<int> state, int board_size, std::queue<Moves> move_history)
    : state(state), board_size(board_size), move_history(move_history)
{
    //Find where the empty cell is
    for (int i = 0; i < this->state.size(); i++) {
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
bool Board::validate_state()
{
    //Ensure we have the right number of positions
    if (this->state.size() != std::pow(this->board_size, 2)) {
        std::cerr << "not enough tiles to fill board: " << this->state.size() << "/" << std::pow(this->board_size, 2) << std::endl;
        return false;
    }

    //Sort the indices and make sure they are a sequence from 0.
    std::vector<int> sorted_state = this->state;
    std::sort(sorted_state.begin(), sorted_state.end());

    int last = -1;
    for (
        std::vector<int>::iterator it = sorted_state.begin();
        it != sorted_state.end();
        ++it
    ) {
        if ((*it) != last+1) {
            std::cerr << "Improper sequence given, missing: " << last+1 << std::endl;
            return false;
        }
        last = (*it);
    }

    return true;
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
    int empty_x = this->zero_position % this->board_size;
    int empty_y = this->zero_position / this->board_size;

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
    int last = -1;
    for (
        std::vector<int>::iterator it = this->state.begin();
        it != this->state.end();
        ++it
    ) {
        if ((*it) != last+1) {
            return false;
        }
        last = (*it);
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
    std::vector<int> new_state = this->state;
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
    std::queue<Moves> new_history = this->move_history;
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
}

/**
 * Create a unique hash of the board state to act as a simple
 * identifier.
 *
 * @return A unique hash of the board state.
 */
std::size_t Board::get_state_hash()
{
    std::string state_representation;
    for (
        std::vector< int >::iterator it = this->state.begin();
        it != this->state.end();
        ++it
    ) {
        state_representation += std::to_string(*it) + ":";
    }
    return std::hash<std::string>{}(state_representation);
}

/**
 * Get the cost spent reaching this board state.
 * Consider one move as one cost unit.
 *
 * @return The number of moves taken.
 */
int Board::get_cost()
{
    return this->state.size();
}

/**
 * Returns a simple heuristic which measures how many tiles are currently misplaced.u78olkk
 *
 * @return A heuristic value.
 */
int Board::get_heuristic()
{
    int misplaced = 0;
    int i = 0;
    for (
        std::vector<int>::iterator it = this->state.begin();
        it != this->state.end();
        ++it
    ) {
        if ((*it) != i) {
            misplaced++;
        }
        i++;
    }

    return misplaced;
}
