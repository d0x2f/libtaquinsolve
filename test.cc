#include <iostream>
#include <taquinsolve.hh>

void print(std::queue<TaquinSolve::Moves> moves)
{
    while (!moves.empty()) {
        switch (moves.front()) {
            case TaquinSolve::Moves::UP:
                std::cout << "↑";
                break;

            case TaquinSolve::Moves::DOWN:
                std::cout << "↓";
                break;

            case TaquinSolve::Moves::LEFT:
                std::cout << "←";
                break;

            case TaquinSolve::Moves::RIGHT:
                std::cout << "→";
                break;
        }
        moves.pop();
    }
    std::cout << std::endl;
}


int main(int argc, char **argv)
{
    std::cout << "3x3 puzzle" << std::endl;
    print(taquin_solve("8 4 2 7 6 3 5 1 0", 3));

    std::cout << "4x4 puzzle" << std::endl;
    print(taquin_solve("12 11 8 4 9 2 7 10 6 3 14 15 1 13 5 0", 4));

    //std::cout << "10x10 puzzle" << std::endl;
    //print(taquin_solve("14 52 62 72 91 36 42 8 38 65 18 5 81 54 88 7 12 15 60 95 27 64 34 35 75 71 0 89 93 43 53 67 92 19 6 96 30 63 47 90 3 4 40 80 16 55 76 56 58 59 79 20 29 24 78 13 49 37 61 44 32 31 66 74 23 45 94 17 99 84 11 98 28 87 48 85 57 1 10 26 39 69 22 86 21 25 50 83 68 73 70 51 41 77 9 33 82 2 46 97", 10));
}
