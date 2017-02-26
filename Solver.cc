#include <iostream>
#include <sstream>
#include <algorithm>

#include "Solver.hh"

using namespace TaquinSolve;

Solver::Solver(std::string board_string, int board_size)
{
    this->initial_board = new Board(Solver::tokenise_board_string(board_string), board_size);
}

std::queue<Moves> Solver::solve()
{
    if (!this->initial_board->validate_state()) {
        return std::queue<Moves>();
    }

    //Create shared pointer to pass along
    //Scoped so it will delete
    {
        std::shared_ptr<Board> initial_board = std::shared_ptr<Board>(this->initial_board);
        this->open.insert(std::pair< int, std::shared_ptr<Board> >(0, initial_board));
        this->open_hash.insert(std::pair<std::size_t, std::shared_ptr<Board> >(this->initial_board->get_state_hash(), initial_board));
    }

    while(this->open.size() > 0) {
        std::shared_ptr<Board> current = this->open.begin()->second;

        if (current->check_solved()) {
            return current->get_move_history();
        }

        this->open_hash.erase(this->open_hash.find(current->get_state_hash()));
        this->open.erase(this->open.begin());

        this->closed.insert(std::pair<std::size_t, bool>(current->get_state_hash(), true));

        std::vector< std::shared_ptr<Board> > neighbors = this->perform_moves(current.get(), current->get_available_moves());

        for (
            std::vector< std::shared_ptr<Board> >::iterator it = neighbors.begin();
            it != neighbors.end();
            ++it
        ) {
            std::shared_ptr<Board> neighbor = *it;
            std::size_t neighbor_hash = neighbor->get_state_hash();
            if (this->closed.find(neighbor_hash) != this->closed.end()) {
                continue;
            }

            std::map<std::size_t, std::shared_ptr<Board> >::iterator existing_entry = this->open_hash.find(neighbor_hash);
            if (existing_entry == this->open_hash.end()) {
                this->open.insert(std::pair<int, std::shared_ptr<Board> >(neighbor->get_cost() + neighbor->get_heuristic(), neighbor));
                this->open_hash.insert(std::pair<std::size_t, std::shared_ptr<Board> >(neighbor_hash, neighbor));
                existing_entry = this->open_hash.find(neighbor_hash);
            } else if (current->get_cost() + 1 >= existing_entry->second->get_cost()) {
                continue;
            }
            existing_entry->second->replace_move_history(neighbor->get_move_history());
        }
    }

    std::cout << "no idea what happened" << std::endl;
    return std::queue<Moves>();
}

std::vector< std::shared_ptr<Board> > Solver::perform_moves(Board *board, std::vector<Moves> moves) {
    std::vector< std::shared_ptr<Board> > results;

    for (Moves move : moves) {
        results.push_back(std::shared_ptr<Board>(board->perform_move(move)));
    }

    return results;
}

std::vector<int> Solver::tokenise_board_string(std::string str, char sep)
{
    std::vector<int> ret;

    std::istringstream stm(str);
    std::string token;
    while( std::getline( stm, token, sep ) ) {
        ret.push_back(std::stoi(token));
    }

    return ret;
}
