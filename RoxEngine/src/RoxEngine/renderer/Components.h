#pragma once 
#include <glm/glm.hpp>

namespace RoxEngine {
    struct Transform
    {
    public:
        glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
        glm::vec3 position = { 0.0f, 0.0f, 0.0f };
        glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        glm::mat4 GetMatrix() {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), this->rotation.x, { 1,0,0 })
                * glm::rotate(glm::mat4(1.0f), this->rotation.y, { 0,1,0 })
                * glm::rotate(glm::mat4(1.0f), this->rotation.z, { 0,0,1 });
            return glm::translate(glm::mat4(1.0f), position) * rotation * glm::scale(glm::mat4(1.0f), scale);
        }
    };
}