#include "Headers/QueenPiece.h"
#include <GL/glut.h>

QueenPiece::QueenPiece(){

};

QueenPiece::QueenPiece(int row, int col, int name, Object3D::vertex pos) : QueenPiece()
{
    this->model = Object3D("./EightQueens/Resources/queen.obj");
    this->model.setPosition(pos);
    this->model.setColor(Object3D::vertex(0.0, 0.0, 0.0));
    this->model.name = name;

    this->row = row;
    this->col = col;
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