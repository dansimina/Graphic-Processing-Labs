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
        /*for (int i = 0; i < vertices.size() - 2; i++) {
            
        }*/
    }
    
    std::vector<glm::vec2> ComputeNormals(const std::vector<glm::vec2> &vertices)
    {
        std::vector<glm::vec2> normalsList;
        return normalsList;
    }
}
