//
// Created by Willi on 7/30/2020.
//
#pragma once

#include <GL/glew.h>
#include <string>

/// The types of textures supported.
enum TextureType {
    TEXTURE2D,
    CUBEMAP
};

/// An interface for Texture classes
class TextureInterface {
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

    inline GLuint getTexId() { return texID; }

    inline void setTexId(GLuint id) { this->texID = id; }
};

/// A simple texture
class Texture : public TextureInterface {
public:
    Texture() = default;

    Texture(const std::string filePath);

    void loadFromFile(const std::string &filePaths) override;

    void loadFromFaceFiles(const std::vector<std::string> &filePaths) override;

    void bindTexture() override;
};

/// A cubemap: composed of 6 textures, 1 for each face.
class CubeMap : public TextureInterface {
public:
    CubeMap() = default;

    CubeMap(const std::vector<std::string> &filePaths);

    void loadFromFile(const std::string &filePaths) override;

    void loadFromFaceFiles(const std::vector<std::string> &filePaths) override;

    void bindTexture() override;
};