#include "Headers/Object3D.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <GL/glew.h>
#include <GL/glut.h>

Object3D::Object3D()
{
    this->position = vertex(0, 0, 0);
}

Object3D::Object3D(const char *filename) : Object3D()
{
    this->readFile(filename);
}

void Object3D::setPosition(vertex pos)
{
    this->position = pos;
}

void Object3D::setColor(vertex color)
{
    this->color = color;
}

// TODO: Render using BUFFERS
void Object3D::render()
{
    glBegin(GL_QUADS);

    glColor3f(this->color.x, this->color.y, this->color.z);

    for (int i = 0; i < faces.size(); i++)
    {
        vertex v1 = vertices[faces[i].v1 - 1];
        vertex vn1 = normals[faces[i].v1 - 1];
        vertex v2 = vertices[faces[i].v2 - 1];
        vertex vn2 = normals[faces[i].v2 - 1];
        vertex v3 = vertices[faces[i].v3 - 1];
        vertex vn3 = normals[faces[i].v3 - 1];
        vertex v4 = vertices[faces[i].v4 - 1];
        vertex vn4 = normals[faces[i].v4 - 1];

        glNormal3f(vn1.x, vn1.y, vn1.z);
        glVertex3f(v1.x, v1.y, v1.z);

        glNormal3f(vn1.x, vn1.y, vn1.z);
        glVertex3f(v2.x, v2.y, v2.z);

        glNormal3f(vn3.x, vn3.y, vn3.z);
        glVertex3f(v3.x, v3.y, v3.z);

        glNormal3f(vn4.x, vn4.y, vn4.z);
        glVertex3f(v4.x, v4.y, v4.z);
    }

    glEnd();
}


void Object3D::readFile(const char* filename)
{

    std::string temp;
    std::string line;
    double d_temp;


    std::ifstream fin(filename);

    if (!fin) return;

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
            face f;

            iss >> temp;
            iss >> f.v1; iss.ignore(1); iss >> d_temp; iss.ignore(1); iss >> d_temp; iss.ignore(1);
            iss >> f.v2; iss.ignore(1); iss >> d_temp; iss.ignore(1); iss >> d_temp; iss.ignore(1); 
            iss >> f.v3; iss.ignore(1); iss >> d_temp; iss.ignore(1); iss >> d_temp; iss.ignore(1); 
            iss >> f.v4; iss.ignore(1); iss >> d_temp; iss.ignore(1); iss >> d_temp; 
            this->faces.push_back(f);

        }

        else if (line.substr(0, 3) == "vn ")
        {
            
            vertex vn;
            iss >> temp >> vn.x >> vn.y >> vn.z;

            this->normals.push_back(vn);

        }
    }
}