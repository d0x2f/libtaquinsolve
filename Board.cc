#include <iostream>
#include <cmath>
#include <algorithm>

#include "Board.hh"

using namespace TaquinSolve;

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

std::vector<Moves> Board::get_available_moves()
{
    std::vector<Moves> output;

    int empty_x = this->zero_position % this->board_size;
    int empty_y = this->zero_position / this->board_size;

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
    this->move_history.push(move);
    return new Board(new_state, this->board_size, this->move_history);
}

std::queue<Moves> Board::get_move_history()
{
    return this->move_history;
}

void Board::replace_move_history(std::queue<Moves> move_history)
{
    this->move_history = move_history;
}

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

int Board::get_cost()
{
    return this->state.size();
}

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
