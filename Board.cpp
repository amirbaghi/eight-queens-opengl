#include "Headers/Board.h"
#include <GL/glut.h>

Board::Board(){};

Board::Board(int num_of_rows, int num_of_cols, std::vector<Square> tiles)
{
    this->num_of_rows = num_of_rows;
    this->num_of_cols = num_of_cols;

    this->tiles = tiles;
}

void Board::setTiles(std::vector<Square> tiles)
{
    this->tiles = tiles;
}

void Board::setRowsAndCols(int rows, int cols)
{
    this->num_of_cols = cols;
    this->num_of_rows = rows;
}

void Board::render()
{
    for (Square tile : tiles)
    {
        glPushMatrix();
        glLoadName(tile.name);
        tile.render();
        glPopMatrix();
    }
}
