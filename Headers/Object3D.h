#include <vector>

// 3D Objects Model Class
class Object3D
{
public:

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


    Object3D();
    Object3D(const char *filename);

    vertex getColor() const { return color; }
    vertex getPosition() const { return position; }

    void setPosition(vertex pos);
    void setColor(vertex color);
    void readFile(const char *filename);
    void initBuffers();
    void render();

    int name;
    unsigned int vertex_vbo_id, normals_vbo_id, element_vbo_id;

private:
    std::vector<vertex> vertices;
    std::vector<unsigned int> faces;
    std::vector<vertex> normals;
    vertex color;
    vertex position;

};