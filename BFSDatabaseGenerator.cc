#include <queue>
#include <limits>

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
    //Create/connect to an sqlite db in /tmp
    this->create_database();

    std::queue<std::shared_ptr<PartialBoard> > frontier;

    std::shared_ptr< std::vector<size_t> > group_tiles_ptr = std::shared_ptr<std::vector<size_t>>(&group_tiles);

    std::shared_ptr<PartialBoard> initial_board = std::shared_ptr<PartialBoard>(new PartialBoard(goal_board, board_size, 0, group_tiles_ptr));

    this->database_insert(initial_board->get_state_hash(), 0);
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
            size_t value = this->database_get_value(neighbor->get_state_hash());
            if (neighbor->get_cost() < value) {
                if (neighbor->get_cost() > neighbor->get_heuristic()) {
                    this->database_insert(neighbor->get_state_hash(), neighbor->get_cost());
                }
                frontier.push(neighbor);
            }
        }
    }

    this->close_database();

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
        std::shared_ptr<PartialBoard> new_board = std::shared_ptr<PartialBoard>(board->perform_move(move));
        results.push_back(new_board);
    }

    return results;
}

void BFSDatabaseGenerator::create_database()
{
    if (sqlite3_open("/tmp/15-puzzle-78-pattern-database.db", &this->db)) {
        throw std::string("Cannot open temporary database in /tmp");
    }

   std::string sql = "CREATE TABLE IF NOT EXISTS pattern(" \
                        "key             INT PRIMARY KEY    NOT NULL," \
                        "value           INT                NOT NULL );";

    char *err = 0;
    if (sqlite3_exec(this->db, sql.c_str(), NULL, 0, &err) != SQLITE_OK) {
        throw std::string("SQL Error: ") + std::string(err);
    }
}

void BFSDatabaseGenerator::database_insert(size_t index, size_t value)
{
    std::string sql = "REPLACE INTO pattern (key,value) VALUES (?,?);";

    //Create a statement
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(this->db, sql.c_str(), -1, &statement, 0) != SQLITE_OK) {
        throw std::string("SQL Error: ") + sqlite3_errmsg(this->db);
    }

    //Bind values
    sqlite3_bind_int(statement, 2, value);
    sqlite3_bind_int64(statement, 1, index);

    sqlite3_step(statement);
    sqlite3_finalize(statement);
}

bool BFSDatabaseGenerator::database_key_exists(size_t index)
{
    std::string sql = "SELECT 1 FROM pattern WHERE key = ?;";

    //Create a statement
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(this->db, sql.c_str(), -1, &statement, 0) != SQLITE_OK) {
        throw std::string("SQL Error: ") + sqlite3_errmsg(this->db);
    }

    //Bind values
    sqlite3_bind_int(statement, 1, index);

    //Step over results once
    int result = sqlite3_step(statement);
    sqlite3_finalize(statement);

    return result == SQLITE_ROW;
}

size_t BFSDatabaseGenerator::database_get_value(size_t index)
{

    std::string sql = "SELECT value FROM pattern WHERE key = ?;";

    //Create a statement
    sqlite3_stmt *statement;
    if (sqlite3_prepare_v2(this->db, sql.c_str(), -1, &statement, 0) != SQLITE_OK) {
        throw std::string("SQL Error: ") + sqlite3_errmsg(this->db);
    }

    //Bind values
    sqlite3_bind_int(statement, 1, index);

    //Step over results once
    size_t ret = std::numeric_limits<size_t>::max();
    if (sqlite3_step(statement) == SQLITE_ROW) {
        ret = sqlite3_column_int(statement, 0);
    }
    sqlite3_finalize(statement);

    return ret;
}

void BFSDatabaseGenerator::close_database()
{
    sqlite3_close(this->db);
}
