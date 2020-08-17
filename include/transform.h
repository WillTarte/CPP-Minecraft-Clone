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

public:
    glm::vec3 position{};

    Transform() {
        this->position = {0.0f, 0.0f, 0.0f};
        this->scale = {1.0f, 1.0f, 1.0f};
        this->rotation = glm::quat({0.0f, 0.0f, 0.0f});
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
    }

    /** Translates the current position by the given displacement
     * @param displacement the displacement vector
     */
    void translate(glm::vec3 displacement) {
        this->position += displacement;
    }

    /** Sets the position
     * @param pos
     */
    void setPosition(glm::vec3 pos) {
        this->position = pos;
    }

    /** Applies scaling component-wise
     * @param scaling the component-wise scaling factors
     */
    void scaleBy(glm::vec3 scaling) {
        this->scale *= scaling;
    }

    /** Applies the same scaling factor to all components
     * @param scalingFactor the amount to scale by
     */
    void scaleBy(float scalingFactor) {
        if (scalingFactor <= 0)
            return;
        this->scale *= scalingFactor;
    }

    /** Applies a rotation
     * @param rotationQuat the quaternion representing the rotation
     */
    void rotate(glm::quat rotationQuat) {
        this->rotation = rotationQuat * this->rotation;
    }

    /** Rotates around the given axis by some amount
     * @param rotationAxis the rotation axis
     * @param angle the rotation angle given in radians
     */
    void rotate(glm::vec3 rotationAxis, float angle) {
        this->rotation *= glm::angleAxis(angle, glm::normalize(rotationAxis));
    }

    void reset() {
        *this = Transform();
    }

    inline glm::mat4 getModelMatrix() {
        return glm::translate(glm::mat4(1.0f), this->position) * glm::toMat4(this->rotation) *
               glm::scale(glm::mat4(1.0f), this->scale);
    }

    const glm::vec3 getPosition() {
        return position;
    }

    const glm::quat getRotation() const {
        return rotation;
    }

    const glm::vec3 getScale() const {
        return scale;
    }
};