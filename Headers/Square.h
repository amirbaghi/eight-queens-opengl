#ifndef MY_SQUARE_H
#define MY_SQUARE_H

#include <vector>
#include "vertex.h"
#include "color4.h"
#include <GL/glut.h>

class Square
{
public:
    Square();
    Square(vertex v1, vertex v2, vertex v3, vertex v4, color4 color, int name, int row, int col);

    std::vector<vertex> getVertices() { return vertices; };
    std::vector<color4> getColor() { return color; };

    void setColor(color4 color);
    void initBuffers();
    void render();

    int name, row, col;
    GLuint vertex_vbo_id, element_vbo_id, normal_vbo_id, color_vbo_id;

private:
    std::vector<vertex> vertices;
    std::vector<GLuint> face;
    std::vector<vertex> normals;
    std::vector<color4> color;
};

#endif