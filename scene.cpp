//
// Created by Willi on 7/21/2020.
//

#include "scene.h"

#include <utility>

SceneNode::SceneNode(Drawable *noderoot, std::string tag) {
    this->drawable = noderoot;
    this->tag = std::move(tag);
    this->children = std::vector<SceneNode>();
    this->bubble = NULL;
};

void SceneNode::addChild(Drawable *child, std::string childTag) {
    this->children.emplace_back(SceneNode(child, std::move(childTag)));
}

void SceneNode::draw(const glm::mat4 &model, const glm::mat4 &view, const glm::mat4 &projection) {
    this->drawable->draw(model, view, projection);
    for (auto child : children) {
        child.draw(model * this->drawable->getTransform(), view, projection);
    }
    if(this->bubble != NULL) {
        glm::mat4 output = glm::translate(glm::mat4(1.0f), glm::vec3(3.0, 7.5, 0.0));
        output = glm::scale(output, glm::vec3(2.0f, 2.0f, 2.0f));
        this->bubble->draw(projection * view * model *this->drawable->getTransform()* output);
    }
}

void SceneNode::addChild(SceneNode &node) {
    this->children.emplace_back(node);
}

void SceneNode::linkSphere(Sphere *bubble){
    this->bubble = bubble;
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
