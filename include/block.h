//
// Created by Willi on 8/2/2020.
//
#pragma once

#include <fstream>
#include "../libs/easylogging++.h"
#include "texture.h"

/// Block ID maps .block file to a specific type of block
enum BlockID {
    PLAYER = 0,
    DIRT = 1,
    DIRT_GRASS = 2,
    BEDROCK = 3,
    STONE = 4,
    OAK_LOG = 5,
    OAK_LEAVES = 6,
    WATER = 7,
    SKYBOX = 8,
    RUBY = 9,
    GOLD = 10,
    SUN = 11
};
static constexpr BlockID allBlockIDs[] = {DIRT, DIRT_GRASS, BEDROCK, STONE, OAK_LOG, OAK_LEAVES, WATER,RUBY,GOLD};

inline std::ostream &operator<<(std::ostream &os, BlockID blockId) {
    switch (blockId) {
        case DIRT:
            os << "DIRT";
            break;
        case DIRT_GRASS:
            os << "DIRT GRASS";
            break;
        case PLAYER:
            os << "STEVE";
            break;
        case BEDROCK:
            os << "BEDROCK";
            break;
        case STONE:
            os << "STONE";
            break;
        case OAK_LOG:
            os << "OAK_LOG";
            break;
        case OAK_LEAVES:
            os << "OAK LEAVES";
            break;
        case WATER:
            os << "WATER";
            break;
        case RUBY:
            os << "RUBY";
            break;
        case GOLD:
            os << "GOLD";
            break;
        case SKYBOX:
            os << "SKYBOX";
            break;
        case SUN:
            os << "SUN";
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
    bool breakable = true; // prevent you from being able to fall through the world with unbreakable bedrock
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
            ret.modelFile = "./resources/models/" + value;
            continue;
        }

        if (identifier == "top" || identifier == "bottom" || identifier == "left" || identifier == "right" ||
            identifier == "front" || identifier == "back") {
            ret.cubeFaceFiles.emplace_back("./resources/textures/" + value);
            ret.textureType = CUBEMAP;
            continue;
        }
        if (identifier == "all") {
            ret.textureFile = "./resources/textures/" + value;
            ret.textureType = TEXTURE2D;
            break;
        }

        if (identifier == "breakable") {
            ret.breakable = value == "true";
        }
    }

    if (ret.ID < 0) {
        LOG(WARNING) << "Invalid Block ID!";
    }

    return ret;
}