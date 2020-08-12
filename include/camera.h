//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

struct Camera {
    float Yaw = YAW;
    float Pitch = PITCH;
    glm::vec3 Up = {0.0, 1.0, 0.0};
    glm::vec3 Front{1.0, 0.0, 0.0};
    glm::vec3 Right{0.0, 0.0, 1.0};
    glm::vec3 Position = {0, 4, 10};

    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, {0.0, 1.0,
                                                  0.0}));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }
};
