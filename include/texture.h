//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <string>

class Texture {
private:
    GLuint texID = 0;

    void loadFromFile(const std::string& filePath);
public:
    Texture() = default;
    Texture(const std::string& file);

    void bindTexture() const ;
};
