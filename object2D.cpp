#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float height,
    float width,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}


Mesh* object2D::CreateRomb(
    const std::string& name,
    glm::vec3 center,
    float length,
    glm::vec3 color,
    bool fill)
{
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(center - glm::vec3(length / 2, 0, 0), color),
        VertexFormat(center + glm::vec3(0, length, 0), color + glm::vec3(0, 0.3f, 0)),
        VertexFormat(center + glm::vec3(length / 2, 0, 0), color),
        VertexFormat(center - glm::vec3(0, length, 0), color - glm::vec3(0, 0.3f, 0))
    };

    Mesh* romb = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        romb->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    romb->InitFromData(vertices, indices);
    return romb;
}

Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        
        VertexFormat(corner + glm::vec3(length/6.0f, length/2.0f, 0), color),
        VertexFormat(corner + glm::vec3(length / 3.0f, length, 0), color),
        VertexFormat(corner + glm::vec3(2.0f*length/3, 0, 0), color),

        VertexFormat(corner + glm::vec3(length / 3.0f, length / 3.0f, 0), color),
        VertexFormat(corner + glm::vec3(-length/6.0f, 2.0f*length/3, 0), color + glm::vec3(0, 0.3f, 0)),
        VertexFormat(corner + glm::vec3(5.0f * length / 6, 2.0f * length / 3, 0), color - glm::vec3(0, 0.3f, 0)),
        VertexFormat(corner + glm::vec3(length / 3.0f, length / 3.0f, 0), color),

        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length / 3.0f, length, 0), color),
        VertexFormat(corner + glm::vec3(length / 2.0f, length / 2.0f, 0), color)
    };

    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    if (!fill) {
        star->SetDrawMode(GL_LINE_STRIP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
       
    }

    star->InitFromData(vertices, indices);
    return star;
}

Mesh* object2D::CreateHexagon(const std::string& name, float radius, glm::vec3 color)
{
    const float PI = 3.1415926f;
    float n = 2 * PI / 6;

    float currAngle;
    std::vector<VertexFormat> vertices = {};
    /* add the center of the circle */
    vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
    /* iterate thro the next point of the circle */
    for (currAngle = 0; currAngle < 2 * PI; currAngle += n)
    {
        vertices.push_back(VertexFormat(glm::vec3(radius * cos(currAngle), radius * sin(currAngle), 0), color));
    }
    vertices.push_back(VertexFormat(glm::vec3(radius * cos(2 * PI), radius * sin(2 * PI), 0), color));

    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices = {};

    for (int i = 1; i <= vertices.size(); i += 1) {
        indices.push_back(i);
        indices.push_back(0);
        indices.push_back(i + 1);
    }

    circle->SetDrawMode(GL_TRIANGLES);
    circle->InitFromData(vertices, indices);
    return circle;
}
