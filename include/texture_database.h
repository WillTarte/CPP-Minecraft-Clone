//
// Created by Willi on 8/2/2020.
//
#pragma once

#include <fstream>
#include <algorithm>
#include <filesystem>
#include <unordered_map>
#include "../libs/easylogging++.h"
#include "block.h"
#include "texture.h"

namespace fs = std::filesystem;

/// The Texture Database loads available textures according to the defined blocks and stores them for easy access.
class TextureDatabase {
private:
    TextureDatabase();

    TextureDatabase(const TextureDatabase &) = delete;

    TextureDatabase &operator=(const TextureDatabase &) = delete;

    static TextureDatabase instance;
    std::unordered_map<BlockID, std::shared_ptr<TextureInterface>> textures;

public:

    /// Initializes the texture database. Should only be called once during program execution.
    static void init();

    /** Returns a texture based on the block ID provided.
     *
     * @param id the block to fetch a texture for.
     * @return NULL if blockId is not registered. Otherwise returns a pointer to a texture.
     */
    static std::shared_ptr<TextureInterface> getTextureByBlockId(BlockID id);
};
