#include "Object3D.h"

class QueenPiece
{
public:
    QueenPiece();
    QueenPiece(int row, int col, int name, Object3D::vertex pos);

    void render();

    Object3D model;
    int row, col;
};