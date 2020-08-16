//
// Created by Willi on 7/30/2020.
//
#include "../include/texture.h"


void Texture2D::loadFromFile(const std::string &filePath) {
    GLuint tempId = 0;
    glGenTextures(1, &tempId);
    setTexId(tempId);
    glBindTexture(GL_TEXTURE_2D, getTexId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        texID = 0;
        return;
    }
    GLenum format = 0;
    if (nrChannels == 1) {
        format = GL_RED; //oof
    } else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
}

void Texture2D::bindTexture() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, getTexId());
}

void Texture2D::loadFromFaceFiles(const std::vector<std::string> &filePaths) {
    GLuint tempId = 0;
    glGenTextures(1, &tempId);
    setTexId(tempId);
    glBindTexture(GL_TEXTURE_2D, getTexId());
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePaths[0].c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture" << std::endl;
        texID = 0;
        return;
    }
    GLenum format = 0;
    if (nrChannels == 1) {
        format = GL_RED; //oof
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
    } else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);

}

Texture2D::Texture2D(const std::string &filePath) {
    loadFromFile(filePath);
}

CubeMap::CubeMap(const std::vector<std::string> &filePaths) {
    loadFromFaceFiles(filePaths);
}

void CubeMap::loadFromFile(const std::string &filePath) {
    GLuint tempId = 0;
    glGenTextures(1, &tempId);
    setTexId(tempId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, getTexId());

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load texture " + filePath << std::endl;
        texID = 0;
        return;
    }

    for (unsigned int i = 0; i < 6; i++) {
        GLenum format = 0;
        if (nrChannels == 1) {
            format = GL_RED; //oof
        } else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    stbi_image_free(data);
}

void CubeMap::loadFromFaceFiles(const std::vector<std::string> &filePaths) {
    GLuint tempId = 0;
    glGenTextures(1, &tempId);
    setTexId(tempId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, getTexId());

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data;

    for (unsigned int i = 0; i < 6; i++) {
        data = stbi_load(filePaths[i].c_str(), &width, &height, &nrChannels, 0);
        if (!data) {
            std::cout << "Failed to load texture " + filePaths[i] << std::endl;
            texID = 0;
            return;
        }
        GLenum format = 0;
        if (nrChannels == 1) {
            format = GL_RED; //oof
        } else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glTexImage2D(
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    stbi_image_free(data);
}

void CubeMap::bindTexture() {
    glActiveTexture(GL_TEXTURE1); // Cannot have different type of textures bound to same texture unit
    glBindTexture(GL_TEXTURE_CUBE_MAP, getTexId());
}