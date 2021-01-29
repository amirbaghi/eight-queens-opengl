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

    // Face Struct
    struct face
    {
        unsigned int v1, v2, v3, v4;
    };

    Object3D();
    Object3D(const char *filename);

    vertex getColor() const { return color; }
    vertex getPosition() const { return position; }

    void setPosition(vertex pos);
    void setColor(vertex color);
    void readFile(const char *filename);
    void render();

    int name;

private:
    std::vector<vertex> vertices;
    std::vector<face> faces;
    std::vector<vertex> normals;
    vertex color;
    vertex position;

};