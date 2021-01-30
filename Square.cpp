#include <GL/glew.h>
#include "Headers/Square.h"

Square::Square(){};

Square::Square(vertex v1, vertex v2, vertex v3, vertex v4, color4 color, int name)
{
    std::vector<vertex> vertices;
    vertices.reserve(4);
    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);
    vertices.push_back(v4);

    this->vertices = vertices;

    std::vector<GLuint> face;
    face.reserve(4);
    face.push_back(0);
    face.push_back(1);
    face.push_back(2);
    face.push_back(3);

    this->face = face;

    std::vector<vertex> normals;
    normals.reserve(4);
    normals.push_back(vertex(0.0, 1.0, 0.0));
    normals.push_back(vertex(0.0, 1.0, 0.0));
    normals.push_back(vertex(0.0, 1.0, 0.0));
    normals.push_back(vertex(0.0, 1.0, 0.0));

    this->normals = normals;

    std::vector<color4> colors;
    colors.reserve(4);
    colors.push_back(color);
    colors.push_back(color);
    colors.push_back(color);
    colors.push_back(color);

    this->color = colors;

    this->name = name;

    Square::initBuffers();
}

void Square::initBuffers()
{
    glGenBuffers(1, &vertex_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &color_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, color.size() * sizeof(color4), &color[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normal_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normal_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, normals.size() * sizeof(vertex), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &element_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, face.size() * sizeof(GLuint), &face[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Square::render()
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
    glVertexPointer(3, GL_DOUBLE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, color_vbo_id);
    glColorPointer(4, GL_FLOAT, 0, NULL);

    glEnable (GL_COLOR_MATERIAL);
    glColorMaterial (GL_FRONT, GL_DIFFUSE);
    glColorMaterial (GL_FRONT, GL_SPECULAR);
    glColorMaterial (GL_FRONT, GL_AMBIENT);

    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_id);
    glNormalPointer(GL_DOUBLE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_id);
    glDrawElements(GL_QUADS, (face.size() * sizeof(GLuint)) / 4, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}