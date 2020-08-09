//
// Created by Willi on 8/2/2020.
//
#pragma once

#include "texture.h"

/// Block ID maps .block file to a specific type of block
enum BlockID {
    DIRT = 1,
    DIRT_GRASS = 2
};
static constexpr BlockID allBlockIDs[] = {DIRT, DIRT_GRASS};

inline std::ostream &operator<<(std::ostream &os, BlockID blockId) {
    switch (blockId) {
        case DIRT:
            os << "DIRT";
            break;
        case DIRT_GRASS:
            os << "DIRT GRASS";
            break;
    }
    return os;
}

/** Struct that holds the data read from a .block file.
 *
 * Depending on what value textureType holds, either use cubeFaceFiles or textureFile to create a Texture.
 */
struct BlockFileData {
    BlockID ID;
    TextureType textureType;
    std::vector<std::string> cubeFaceFiles{};
    std::string textureFile;
    std::string blockName;
    std::string modelFile;
};

/** Reads a .block file
 *
 * @param blockFilePath the path to the .block file
 * @return a BlockFileData struct
 */
inline BlockFileData readBlockFile(const std::string &blockFilePath) {
    std::ifstream blockFile;
    blockFile.open(blockFilePath);

    BlockFileData ret;

    std::string line;
    while (std::getline(blockFile, line)) {

        size_t delimPos = line.find(':');
        std::string identifier = line.substr(0, delimPos);
        std::string value = line.substr(delimPos + 1, line.size() - delimPos);
        value.erase(std::remove(value.begin(), value.end(), ' '), value.end());

        if (identifier.empty() || value.empty() || identifier == "\n" || value == "\n")
            continue;

        if (identifier == "ID") {
            ret.ID = (BlockID) stoi(value);
            continue;
        }

        if (identifier == "name") {
            ret.blockName = value;
        }

        if (identifier == "model") {
            ret.modelFile = "../resources/models/" + value;
            continue;
        }

        if (identifier == "top" || identifier == "bottom" || identifier == "left" || identifier == "right" ||
            identifier == "front" || identifier == "back") {
            ret.cubeFaceFiles.emplace_back("../resources/textures/" + value);
            ret.textureType = CUBEMAP;
            continue;
        }
        if (identifier == "all") {
            ret.textureFile = "../resources/textures/" + value;
            ret.textureType = TEXTURE2D;
            break;
        }
    }

    if (ret.ID <= 0) {
        LOG(WARNING) << "Invalid Block ID!";
    }

    return ret;
}