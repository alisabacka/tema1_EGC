#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);
    Mesh* CreateRectangle(const std::string& name, glm::vec3 leftBottomCorner, float height, float width, glm::vec3 color, bool fill = true);
    Mesh* CreateRomb(const std::string& name, glm::vec3 center, float length, glm::vec3 color, bool fill = true);
    Mesh* CreateStar(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = true);
    Mesh* CreateHexagon(const std::string& name, float radius, glm::vec3 color);
}
