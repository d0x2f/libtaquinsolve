#include <string>
#include <algorithm>

#include "PartialBoard.hh"

using namespace TaquinSolve;

/**
 * Constructor.
 * Takes on a board state, size and move history.
 * It finds the position of 0 within the board state.
 *
 * @param state         An array of integers representing the board state in row major encoding.
 * @param board_size    The width/height of the board.
 *                      The board state must contain board_size^2 entries.
 * @param move_count    The number of moves taken to get to this state.
 * @param group_tiles   The tiles to consider in this partial board group.
 */
PartialBoard::PartialBoard(
    std::vector<size_t> state,
    size_t board_size,
    size_t move_count,
    std::shared_ptr<std::vector<size_t> > group_tiles
)
    : Board(state, board_size), move_count(move_count), group_tiles(group_tiles)
{
}

/**
 * Create a unique hash of the board state to act as a simple
 * identifier.
 * Since this is a partial board, only include the appropriate
 * tiles in the hash.
 *
 * @return A unique hash of the partial board state.
 */
size_t PartialBoard::get_state_hash()
{
    std::string state_representation;
    for (
        std::vector<std::size_t>::iterator it = this->state.begin();
        it != this->state.end();
        ++it
    ) {
        std::vector<size_t>::iterator tile = std::find(this->group_tiles->begin(), this->group_tiles->end(), *it);
        if (tile == this->group_tiles->end()) {
            state_representation += std::to_string(*it) + ":";
        } else {
            state_representation += "*:";
        }
    }
    return std::hash<std::string>{}(state_representation);
}

/**
 * Perform the given move upon a copy of this board and return it.
 *
 * @param move The move to apply.
 *
 * @return A new board object with the move applied.
 */
PartialBoard *PartialBoard::perform_move(Moves move)
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

    return new PartialBoard(new_state, this->board_size, this->move_count+1, this->group_tiles);
}

/**
 * Get the cost spent reaching this board state.
 * Consider one move as one cost unit.
 *
 * @return The number of moves taken.
 */
size_t PartialBoard::get_cost()
{
    return this->move_count;
}
