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
        if (this->isThreatened)
        {
            this->model.setColor(color4(1.0, 0.0, 0.0, 1.0));
        }
        else 
        {
            this->model.setColor(color4(0.0, 0.0, 0.0, 1.0));
        }
    }
}

void QueenPiece::setRowAndCol(int row, int col)
{
    this->row = row;
    this->col = col;
}

void QueenPiece::startMoving(int time, Square *position)
{
    this->motion_init_time = time;
    this->isMoving = true;
    this->motion = ASCENDING;
    this->destination = position;
    this->destination_color = position->getColor()[0];

    // Highlight the destination square
    this->destination->setColor(color4(1.0, 0.0, 0.0, 1.0));
}

void QueenPiece::update(int time)
{
    // Update piece if it is moving
    if (isMoving)
    {
        auto diffTime = (time - motion_init_time) / 1000.0;

        // Get the position for the center of the destination square
        auto dest_x = destination->getVertices()[0].x + 0.5;
        auto dest_z = destination->getVertices()[0].z + 0.5;

        switch (motion)
        {
        // If the piece is ascending
        case ASCENDING:
            // If the piece has reached high enough
            if (model.getPosition().y >= 1.6)
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
            if (abs(model.getPosition().x - dest_x) < 0.1 && abs(model.getPosition().z - dest_z) < 0.1)
            {
                // Fix the piece at the position
                vertex p = model.getPosition();
                p.x = dest_x;
                p.z = dest_z;

                model.setPosition(p);

                // Change the motion to descending
                this->motion = DESCENDING;
                this->motion_init_time = time;
            }
            else
            {
                vertex p = model.getPosition();

                if (dest_x < model.getPosition().x)
                {
                    p.x -= diffTime * 0.1;
                }
                else if (dest_x > model.getPosition().x)
                {
                    p.x += diffTime * 0.1;
                }

                if (dest_z < model.getPosition().z)
                {
                    p.z -= diffTime * 0.1;
                }
                else if (dest_z > model.getPosition().z)
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

                // Set no motion for the piece
                this->motion = MOTIONLESS;
                this->isMoving = false;

                // Set back the original color of the destination square
                this->destination->setColor(this->destination_color);
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

void QueenPiece::setIsThreatened(bool isThreatened)
{
    this->isThreatened = isThreatened;

    if (this->isThreatened)
    {
        this->model.setColor(color4(1.0, 0.0, 0.0, 1.0));
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