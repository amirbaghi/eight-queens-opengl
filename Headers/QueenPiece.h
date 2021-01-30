#ifndef MY_QUEENPIECE_H
#define MY_QUEENPIECE_H

#include "Object3D.h"

class QueenPiece
{
public:
    QueenPiece();
    QueenPiece(int row, int col, int name, vertex pos);

    void setIsSelected(bool isSelected);
    void render();

    Object3D model;
    int row, col;
    bool isSelected;
};

#endif