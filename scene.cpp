//
// Created by Willi on 7/21/2020.
//

#include "scene.h"

#include <utility>

SceneNode::SceneNode(Drawable *noderoot, std::string tag) {
    this->drawable = noderoot;
    this->tag = std::move(tag);
    this->children = std::vector<SceneNode>();
};

void SceneNode::addChild(Drawable *child, std::string childTag) {
    this->children.emplace_back(SceneNode(child, std::move(childTag)));
}

void SceneNode::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
    this->drawable->draw(model, view, projection);
    for (auto child : children) {
        child.draw(model * this->drawable->getTransform(), view, projection);
    }
}

void SceneNode::addChild(SceneNode &node) {
    this->children.emplace_back(node);
}

void Scene::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
    for (auto &node : nodes) {
        node.draw(model, view, projection);
    }
}

void Scene::addNode(SceneNode &node) {
    this->nodes.emplace_back(node);
}

void Scene::changeRenderMode(GLenum renderMode) {
    for (auto &node : nodes) {
        node.setRenderMode(renderMode);
    }
}
