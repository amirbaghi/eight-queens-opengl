#ifndef MY_QUEENPIECE_H
#define MY_QUEENPIECE_H

#include "Object3D.h"

class QueenPiece
{
public:

    enum Motion
    {
        ASCENDING,
        DESCENDING,
        FORWARD,
        MOTIONLESS
    };

    QueenPiece();
    QueenPiece(int row, int col, int name, vertex pos);

    void startMoving(int time, vertex position);
    void update(int time);
    void setIsSelected(bool isSelected);
    void render();

    Object3D model;
    int row, col;
    bool isSelected;

    bool isMoving;
    Motion motion;
    int motion_init_time;
    vertex destination;
};

#endif