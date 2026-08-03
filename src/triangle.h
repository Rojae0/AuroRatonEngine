#pragma once
#include <glm/glm.hpp>

struct custom_triangle
{
public:
    glm::vec3 p1;
    glm::vec3 p2;
    glm::vec3 p3;
    
    custom_triangle(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) : p1(p1), p2(p2), p3(p3) {}
    glm::vec3 cross() const {return glm::cross(p2 - p1, p3 - p2);}
    float get_minx() const {return glm::min(p1.x, glm::min(p2.x, p3.x));}
    float get_miny() const {return glm::min(p1.y, glm::min(p2.y, p3.y));}
    float get_maxx() const {return glm::max(p1.x, glm::max(p2.x, p3.x));}
    float get_maxy() const {return glm::max(p1.y, glm::max(p2.y, p3.y));}
};
