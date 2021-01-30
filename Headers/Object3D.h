#ifndef MY_OBJECT3D_H
#define MY_OBJECT3D_H

#include <vector>
#include <GL/glut.h>
#include "vertex.h"
#include "color4.h"

// 3D Objects Model Class
class Object3D
{
public:
    Object3D();
    Object3D(const char *filename);

    vertex getPosition() const { return position; }

    void setPosition(vertex pos);
    void setColor(color4 color);
    void readFile(const char *filename);
    void initBuffers();
    void render();

    int name;
    GLuint vertex_vbo_id, normals_vbo_id, element_vbo_id, colors_vbo_id;

private:
    std::vector<vertex> vertices;
    std::vector<GLuint> faces;
    std::vector<vertex> normals;
    std::vector<color4> colors;
    vertex position;

};

#endif