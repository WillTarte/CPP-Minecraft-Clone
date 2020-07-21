//
// Created by Willi on 7/21/2020.
//

#ifndef COMP_371_PROJECT_SCENE_H
#define COMP_371_PROJECT_SCENE_H

#include <glm/glm.hpp>
#include <memory>
#include "draw.h"

/** A node in the Scene. Composed of a root drawable object, and its children.
 *
 */
class SceneNode {
private:
    /// The node's "name"
    std::string tag;
    /// The underlying drawable object
    Drawable *drawable{};
    /// The children of this node
    std::vector<SceneNode> children;

    Sphere *bubble;

public:
    SceneNode() = default;

    ~SceneNode() = default;

    SceneNode(Drawable *noderoot, std::string tag);

    void addChild(Drawable *child, std::string childTag);

    void addChild(SceneNode &node);

    void linkSphere(Sphere *bubble);

    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);

    inline std::string getTag() { return tag; }

    inline void setRenderMode(GLenum renderMode) { this->drawable->setRenderMode(renderMode); }
};

/** Defines the current scene to be rendered.
 *  A scene is composed of many renderable objects, with each of them having children objects.
 *  Every object moves within reference to itself or to its parent/ancestors.
 */
class Scene {
private:
    std::vector<SceneNode> nodes;
public:
    Scene() = default;

    ~Scene() = default;

    /** Draws this scene
     *
     * @param model the model matrix (local space -> world space)
     * @param view the view matrix (world space -> view space)
     * @param projection the pojection matrix (view space -> clip space)
     */
    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection);

    /// Adds a node to the scene
    void addNode(SceneNode &node);

    /// Changes the scene's render mode (GL_TRIANGLES, GL_LINES, GL_POINTS)
    void changeRenderMode(GLenum renderMode);
};


#endif //COMP_371_PROJECT_SCENE_H
