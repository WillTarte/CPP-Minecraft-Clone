//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <vector>
#include "../libs/stb_image.h"

/// The types of textures supported.
enum TextureType {
    ABSTRACT,
    TEXTURE2D,
    CUBEMAP
};

/// An interface for Texture classes
class TextureInterface {
private:
    static const TextureType texTypeBase = ABSTRACT;
protected:
    GLuint texID;

public:
    /** Loads a texture from a single file
     *
     * @param filePath the path to the texture file (an image).
     */
    virtual void loadFromFile(const std::string &filePath) = 0;

    /** Loads multiple files into a texture (cubemap)
     *
     * @param filePaths the set of texture file paths to generate the texture from.
     */
    virtual void loadFromFaceFiles(const std::vector<std::string> &filePaths) = 0;

    /// Binds this texture for rendering
    virtual void bindTexture() = 0;

    /// Gets the type of texture (Currently 2D texture or CubeMap)
    virtual TextureType getTextureType() const { return texTypeBase; };

    inline GLuint getTexId() { return texID; }

    inline void setTexId(GLuint id) { this->texID = id; }
};

/// A simple 2D texture
class Texture2D : public TextureInterface {
private:
    static const TextureType texTypeInstance = TEXTURE2D;
public:
    Texture2D() = default;

    Texture2D(const std::string &filePath);

    void loadFromFile(const std::string &filePaths) override;

    void loadFromFaceFiles(const std::vector<std::string> &filePaths) override;

    void bindTexture() override;

    TextureType getTextureType() const override { return texTypeInstance; }
};

/// A cubemap: composed of 6 textures, 1 for each face.
class CubeMap : public TextureInterface {
private:
    static const TextureType texTypeInstance = CUBEMAP;
public:
    CubeMap() = default;

    CubeMap(const std::vector<std::string> &filePaths);

    void loadFromFile(const std::string &filePaths) override;

    void loadFromFaceFiles(const std::vector<std::string> &filePaths) override;

    void bindTexture() override;

    TextureType getTextureType() const override { return texTypeInstance; }
};