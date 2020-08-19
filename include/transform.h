//
// Created by Willi on 8/6/2020.
//
#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

/// Class that holds transform information: position, scale and rotation
class Transform {
private:
    glm::vec3 scale{};
    glm::quat rotation{};
    glm::vec3 position{};
    glm::mat4 modelMatrix{};

public:
    Transform() {
        this->position = {0.0f, 0.0f, 0.0f};
        this->scale = {1.0f, 1.0f, 1.0f};
        this->rotation = glm::quat({0.0f, 0.0f, 0.0f});
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    };

    /** Parameterized constructor
     * @param pos the position in world space of this transform
     * @param scale the scale of this transform
     * @param rotation the rotation to apply to each axis for this transform
     */
    Transform(glm::vec3 pos, glm::vec3 scale, glm::vec3 rotation) {
        this->position = pos;
        this->scale = scale;
        this->rotation = glm::quat(rotation);
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    /** Translates the current position by the given displacement
     * @param displacement the displacement vector
     */
    void translate(glm::vec3 displacement) {
        this->position += displacement;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    /** Applies scaling component-wise
     * @param scaling the component-wise scaling factors
     */
    void scaleBy(glm::vec3 scaling) {
        this->scale *= scaling;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    /** Applies the same scaling factor to all components
     * @param scalingFactor the amount to scale by
     */
    void scaleBy(float scalingFactor) {
        if (scalingFactor <= 0)
            return;
        this->scale *= scalingFactor;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    /** Applies a rotation
     * @param rotationQuat the quaternion representing the rotation
     */
    void rotate(glm::quat rotationQuat) {
        this->rotation = rotationQuat * this->rotation;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    /** Rotates around the given axis by some amount
     * @param rotationAxis the rotation axis
     * @param angle the rotation angle given in radians
     */
    void rotate(glm::vec3 rotationAxis, float angle) {
        this->rotation *= glm::angleAxis(angle, glm::normalize(rotationAxis));
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    void reset() {
        *this = Transform();
    }

    inline glm::mat4 &getModelMatrix() {
        return modelMatrix;
    }

    glm::vec3 &getPosition() {
        return position;
    }

    glm::quat &getRotation() {
        return rotation;
    }

    glm::vec3 &getScale() {
        return scale;
    }

    void setScale(glm::vec3 s) {
        Transform::scale = s;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    void setRotation(glm::quat r) {
        Transform::rotation = r;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }

    void setPosition(glm::vec3 p) {
        Transform::position = p;
        this->modelMatrix = glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
                            glm::scale(glm::mat4(1.0f), this->scale);
    }
};