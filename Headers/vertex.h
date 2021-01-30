#ifndef MY_VERTEX_H
#define MY_VERTEX_H

// Vertex Struct
struct vertex
{
    vertex(){};
    vertex(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double x, y, z;
};

#endif