//
// Created by Willi on 7/21/2020.
//
#include "scene.h"

SceneNode::SceneNode(Drawable *noderoot, std::string tag) {
    this->drawable = noderoot;
    this->tag = std::move(tag);
    this->children = std::vector<SceneNode *>();
};

void SceneNode::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, LightParams lp,
                     const glm::vec3 &cameraPos) {
    this->drawable->draw(model, view, projection, lp, cameraPos);
    for (auto *child : children) {
        child->draw(glm::translate(model, this->drawable->getPosition()) * this->drawable->getTransform(), view,
                    projection, lp, cameraPos);
    }
}

void SceneNode::addChild(SceneNode *node) {
    this->children.emplace_back(node);
}

Scene::Scene() {

    depthShader = Shader("resources/shaders/DepthVertexShader.glsl", "resources/shaders/DepthFragmentShader.glsl");

    // Set up depth map
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                 nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Scene::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection, LightParams lp,
                 const glm::vec3 &cameraPos) {
    for (auto *node : nodes) {
        node->draw(model, view, projection, lp, cameraPos);
    }
}

void Scene::addNode(SceneNode *node) {
    this->nodes.emplace_back(node);
    this->taggedNodes[node->getTag()] = node;
    for (auto *child : node->getChildren()) {
        this->taggedNodes[child->getTag()] = child;
    }
}

void Scene::changeRenderMode(GLenum renderMode) {
    for (auto *node : nodes) {
        node->setRenderMode(renderMode);
        for (auto *child : node->getChildren()) {
            child->setRenderMode(renderMode);
        }
    }
}

std::optional<SceneNode *> Scene::getNodeByTag(const std::string &tag) {
    return this->taggedNodes[tag];
};

void Scene::drawShadows(const glm::mat4 &model, const glm::mat4 &lightSpaceMatrix) {

    // render scene from light's point of view
    depthShader.use();
    depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, woodTexture);

    for (auto &node : nodes) {
        node.drawShadows(model, depthShader);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}