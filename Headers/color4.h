#ifndef MY_COLOR_H
#define MY_COLOR_H

// Color4 Struct
struct color4
{
    color4(){};
    color4(float r, float g, float b, float a)
    {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }

    float r, g, b, a;
};

#endif