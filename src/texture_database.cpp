//
// Created by Willi on 8/2/2020.
//
#include "../include/texture_database.h"

TextureDatabase TextureDatabase::instance;

TextureDatabase::TextureDatabase() {
    this->textures = std::unordered_map<BlockID, std::shared_ptr<TextureInterface>>();
}

void TextureDatabase::init() {

    LOG(INFO) << "Initializing Texture Database ...";

    for (const auto &file : fs::directory_iterator(fs::current_path().string() + "/resources/blocks/")) {
        LOG(INFO) << "Processing " + file.path().filename().string() << " for textures.";

        // Inserts BlockId => Texture
        BlockFileData texData = readBlockFile("./resources/blocks/" + file.path().filename().string());
        if (texData.textureType == TEXTURE2D) {
            instance.textures.insert({texData.ID, std::make_shared<Texture2D>(texData.textureFile)});
        } else if (texData.textureType == CUBEMAP) {
            instance.textures.insert({texData.ID, std::make_shared<CubeMap>(texData.cubeFaceFiles)});
        }

        LOG(INFO) << "Finished processing " + file.path().filename().string() + ".";
    }
}

std::shared_ptr<TextureInterface> &TextureDatabase::getTextureByBlockId(BlockID id) {
    return instance.textures[id];
}