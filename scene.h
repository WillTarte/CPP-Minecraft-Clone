//
// Created by Willi on 7/21/2020.
//

#ifndef COMP_371_PROJECT_SCENE_H
#define COMP_371_PROJECT_SCENE_H

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <unordered_map>
#include "draw.h"

/** A node in the Scene. Composed of a root drawable object, and its children.
 *
 */
class SceneNode {
private:
    /// The node's "name"
    std::string tag;
    /// The children of this node
    std::vector<SceneNode *> children;
public:
    SceneNode() = default;

    ~SceneNode() = default;

    /// Constructor from a root drawable and its name
    SceneNode(Drawable *noderoot, std::string tag);

    /// Add a child Node to this Node
    void addChild(SceneNode *node);

    /// Getter for this node's children
    std::vector<SceneNode *> &getChildren() { return children; }

    void draw(MVPL mvpl, LightParams lp, const glm::vec3 &cameraPos);

    void drawShadows(const glm::mat4 &model, Shader &depthShader, const glm::mat4 &lsm);

    /// Getter for this node's tag
    inline std::string getTag() { return tag; }

    /// Changes this node's render mode
    inline void setRenderMode(GLenum renderMode) const { this->drawable->setRenderMode(renderMode); }

    /// The underlying drawable object
    Drawable *drawable{};
};

/** Defines the current scene to be rendered.
 *  A scene is composed of many renderable objects, with each of them having children objects.
 *  Every object moves within reference to itself or to its parent/ancestors.
 */
class Scene {
private:
    std::vector<SceneNode *> nodes;
    std::unordered_map<std::string, SceneNode *> taggedNodes;

public:

    GLuint depthMap{};
    const unsigned int SHADOW_WIDTH = 2048;
    const unsigned int SHADOW_HEIGHT = 2048;

    Scene();

    ~Scene() {
        glDeleteProgram(depthShader.ID);
        glDeleteTextures(1, &depthMap);
        glDeleteFramebuffers(1, &depthMapFBO);
    }

    /** Draws this scene
     *
     * @param model the model matrix (local space -> world space)
     * @param view the view matrix (world space -> view space)
     * @param projection the pojection matrix (view space -> clip space)
     */
    void draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, const glm::mat4 &lsm,
              const glm::vec3 &cameraPos, LightParams lp);

    void drawShadows(const glm::mat4 &model, const glm::mat4 &lightSpaceMatrix);

    /// Adds a node to the scene
    void addNode(SceneNode *node);

    /// Changes the scene's render mode (GL_TRIANGLES, GL_LINES, GL_POINTS)
    void changeRenderMode(GLenum renderMode);

    /** Returns a reference to a node for a given tag. TODO: handle duplicate tags?
     *
     * @param tag
     * @return
     */
    std::optional<SceneNode *> getNodeByTag(const std::string &tag);

    void disableShadows();

    void enableShadows();

    void disableTextures();

    void enableTextures();

    Shader depthShader{};
    GLuint depthMapFBO{};
};


#endif //COMP_371_PROJECT_SCENE_H
