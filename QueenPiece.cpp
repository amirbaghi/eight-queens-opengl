#include "Headers/QueenPiece.h"
#include <GL/glut.h>

QueenPiece::QueenPiece(){

};

QueenPiece::QueenPiece(int row, int col, int name, vertex pos) : QueenPiece()
{
    this->model = Object3D("./EightQueens/Resources/queen.obj");
    this->model.setPosition(pos);
    this->model.setColor(color4(0.0, 0.0, 0.0, 1.0));
    this->model.name = name;

    this->row = row;
    this->col = col;

    this->isSelected = false;
}

void QueenPiece::setIsSelected(bool isSelected)
{
    this->isSelected = isSelected;

    if (this->isSelected)
    {
        this->model.setColor(color4(0.0, 1.0, 0.0, 1.0));
    }
    else
    {
        this->model.setColor(color4(0.0, 0.0, 0.0, 1.0));
    }
}

void QueenPiece::render()
{
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(model.getPosition().x, model.getPosition().y, model.getPosition().z);
    glScalef(0.3, 0.3, 0.3);
    glLoadName(model.name);
    model.render();
    glPopMatrix();
}