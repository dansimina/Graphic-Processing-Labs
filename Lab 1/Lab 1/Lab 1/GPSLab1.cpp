//
//  GPSLab1.cpp
//
//  Copyright © 2017 CGIS. All rights reserved.
//

#include "GPSLab1.hpp"
#include <cmath>

namespace gps {
    glm::vec4 TransformPoint(const glm::vec4 &point)
    {
        glm::mat4 translationRotation(1.0f);

        translationRotation = glm::rotate(translationRotation, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        translationRotation = glm::translate(translationRotation, glm::vec3(2.0f, 0.0f, 1.0f));
        
        glm::vec4 result = translationRotation * point;
        
        return result;
    }
    
    float ComputeAngle(const glm::vec3 &v1, const glm::vec3 &v2)
    {
        return glm::degrees(acos(glm::dot(v1, v2) / (glm::length(v1) * glm::length(v2))));
    }
    
    bool IsConvex(const std::vector<glm::vec2> &vertices)
    {
        for (int i = 0; i < vertices.size() - 1; i++) 
        {
            if (i < vertices.size() - 2) {
                glm::vec3 e1 = glm::vec3(vertices[i + 1].x - vertices[i].x, vertices[i + 1].y - vertices[i].y, 0);
                glm::vec3 e2 = glm::vec3(vertices[i + 2].x - vertices[i + 1].x, vertices[i + 2].y - vertices[i + 1].y, 0);

                if (glm::cross(e1, e2).z < 0) {
                    return false;
                }
            }
            else {
                glm::vec3 e1 = glm::vec3(vertices[i + 1].x - vertices[i].x, vertices[i + 1].y - vertices[i].y, 0);
                glm::vec3 e2 = glm::vec3(vertices[0].x - vertices[i + 1].x, vertices[0].y - vertices[i + 1].y, 0);

                if (glm::cross(e1, e2).z < 0) {
                    return false;
                }
            }

        }

        return true;
    }
    
    std::vector<glm::vec2> ComputeNormals(const std::vector<glm::vec2> &vertices)
    {
        std::vector<glm::vec2> normalsList;

        for (int i = 0; i < vertices.size(); i++) {
            if (i < vertices.size() - 1) {
                glm::vec3 edge = glm::vec3(vertices[i + 1].x - vertices[i].x, vertices[i + 1].y - vertices[i].y, 0);
                glm::vec3 crossProduct = glm::cross(edge, glm::vec3(0, 0, 1));
                normalsList.push_back(glm::normalize(glm::vec2(crossProduct.x, crossProduct.y)));
            }
            else {
                glm::vec3 edge = glm::vec3(vertices[0].x - vertices[i].x, vertices[0].y - vertices[i].y, 0);
                glm::vec3 crossProduct = glm::cross(edge, glm::vec3(0, 0, 1));
                normalsList.push_back(glm::normalize(glm::vec2(crossProduct.x, crossProduct.y)));
            }
        }

        return normalsList;
    }
}
