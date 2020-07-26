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