#ifndef MY_BOARD_H
#define MY_BOARD_H

#include <vector>
#include "Square.h"

class Board
{
public:
    Board();
    Board(int num_of_rows, int num_of_cols, std::vector<Square> tiles);

    void setTiles(std::vector<Square> tiles);
    void setRowsAndCols(int rows, int cols);
    void render();

    std::vector<Square> tiles;
    int num_of_rows, num_of_cols;
};

#endif