#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <GL/glew.h>
#include "Headers/Object3D.h"

Object3D::Object3D()
{
    this->position = vertex(0, 0, 0);
}

Object3D::Object3D(const char *filename) : Object3D()
{
    this->readFile(filename);
    this->initBuffers();
}

void Object3D::initBuffers()
{
    glGenBuffers(1, &vertex_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertex), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normals_vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo_id);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vertex), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &element_vbo_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(GLuint), &faces[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Object3D::setPosition(vertex pos)
{
    this->position = pos;
}

void Object3D::setColor(vertex color)
{
    this->color = color;
}

void Object3D::render()
{

    glColor3f(this->color.x, this->color.y, this->color.z);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_id);
    glVertexPointer(3, GL_DOUBLE, 0, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, normals_vbo_id);
    glNormalPointer(GL_DOUBLE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_vbo_id);

    glDrawElements(GL_QUADS, (faces.size() * sizeof(GLuint)) / 4, GL_UNSIGNED_INT, NULL);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Object3D::readFile(const char *filename)
{

    std::string temp;
    std::string line;
    double d_temp;
    double current_v;

    std::ifstream fin(filename);

    if (!fin)
        return;

    while (getline(fin, line))
    {
        std::istringstream iss(line);

        if (line.substr(0, 2) == "v ")
        {
            vertex v;
            iss >> temp >> v.x >> v.y >> v.z;
            this->vertices.push_back(v);
        }

        else if (line.substr(0, 2) == "f ")
        {

            iss >> temp;
            iss >> current_v;
            this->faces.push_back(current_v - 1);
            iss.ignore(1);
            iss >> d_temp;
            iss.ignore(1);
            iss >> d_temp;
            iss.ignore(1);
            iss >> current_v;
            this->faces.push_back(current_v - 1);
            iss.ignore(1);
            iss >> d_temp;
            iss.ignore(1);
            iss >> d_temp;
            iss.ignore(1);
            iss >> current_v;
            this->faces.push_back(current_v - 1);
            iss.ignore(1);
            iss >> d_temp;
            iss.ignore(1);
            iss >> d_temp;
            iss.ignore(1);
            iss >> current_v;
            this->faces.push_back(current_v - 1);
            iss.ignore(1);
            iss >> d_temp;
            iss.ignore(1);
            iss >> d_temp;
        }

        else if (line.substr(0, 3) == "vn ")
        {

            vertex vn;
            iss >> temp >> vn.x >> vn.y >> vn.z;

            this->normals.push_back(vn);
        }
    }
}