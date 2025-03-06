#include "object2D_2.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D_2::CreateSquare(
    const std::string &name,
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
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);

    }

    square->InitFromData(vertices, indices);
    return square;
}
Mesh* object2D_2::CreateRECT(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float width,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),

        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color),

        VertexFormat(corner + glm::vec3(0, width, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);

    }

    square->InitFromData(vertices, indices);
    return square;
}

/// This function creates a trapezoid

Mesh* object2D_2::CreateT(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float topBase,
    float lowerBase,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    float offBase = (lowerBase - topBase)/2;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color), /// left-down

        VertexFormat(corner + glm::vec3(lowerBase, 0, 0), color),  ///right-down
        VertexFormat(corner + glm::vec3(offBase + topBase, height, 0), color), ///right-up

        VertexFormat(corner + glm::vec3(offBase, height, 0), color) ///left-up

    };

    Mesh* trapezoid = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        trapezoid->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);

    }

    trapezoid->InitFromData(vertices, indices);
    return trapezoid;
}


Mesh* object2D_2::CreateCircle(const std::string &name,
    glm::vec3 center,
    float R,
    glm::vec3 color,
    bool fill,
    int nrElements)
{
    std::vector<VertexFormat> vertices;


    for(int i=0;i<nrElements;i++)
    {
        float angle = 2.0 * M_PI * i / nrElements;
        float x = center.x + R * cos(angle);
        float y = center.y + R * sin(angle);
        vertices.push_back(VertexFormat(glm::vec3(x, y, center.z), color));
    }
    /// this the circle


    Mesh* circle = new Mesh(name);
    std::vector<unsigned int> indices;
    for(int i=0; i<nrElements; i++) {
        indices.push_back(i);
    }

    if (!fill) {
        circle->SetDrawMode(GL_LINE_LOOP);
    } else {
        for(int i=0;i<nrElements;i++)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
    }

    circle->InitFromData(vertices, indices);
    return circle;
}
