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

    this->isMoving = false;
    this->motion = MOTIONLESS;
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

void QueenPiece::startMoving(int time, vertex position)
{
    this->motion_init_time = time;
    this->isMoving = true;
    this->motion = ASCENDING;
    this->destination = position;
}

void QueenPiece::update(int time)
{
    // Update piece if it is moving
    if (isMoving)
    {
        auto diffTime = (time - motion_init_time) / 1000.0;
        switch (motion)
        {
        // If the piece is ascending
        case ASCENDING:
            // If the piece has reached high enough
            if (model.getPosition().y >= 0.8)
            {
                // Change motion to going forward
                this->motion = FORWARD;
                this->motion_init_time = time;
            }
            else
            {
                vertex p = model.getPosition();
                p.y += diffTime * 0.1;
                model.setPosition(p);
            }
            break;
        // If the piece is going forward
        case FORWARD:
            // If the piece is at the destination
            if (abs(model.getPosition().x - destination.x) < 0.1 && abs(model.getPosition().z - destination.z) < 0.1)
            {
                // Fix the piece at the position
                vertex p = model.getPosition();
                p.x = destination.x;
                p.z = destination.z;

                model.setPosition(p);

                this->motion = DESCENDING;
                this->motion_init_time = time;
            }
            else
            {
                vertex p = model.getPosition();

                if (destination.x < model.getPosition().x)
                {
                    p.x -= diffTime * 0.1;
                }
                else if (destination.x > model.getPosition().x)
                {
                    p.x += diffTime * 0.1;
                }

                if (destination.z < model.getPosition().z)
                {
                    p.z -= diffTime * 0.1;
                }
                else if (destination.z > model.getPosition().z)
                {
                    p.z += diffTime * 0.1;
                }

                model.setPosition(p);
            }
            break;
        // If the piece is descending
        case DESCENDING:
            if (model.getPosition().y <= 0.1)
            {
                vertex p = model.getPosition();
                p.y = 0;

                model.setPosition(p);

                this->motion = MOTIONLESS;
                this->isMoving = false;
            }
            else
            {
                vertex p = model.getPosition();
                p.y -= diffTime * 0.1;
                model.setPosition(p);
            }
            break;
        default:
            break;
        }
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