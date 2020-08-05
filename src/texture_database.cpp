//
// Created by Willi on 8/2/2020.
//
#include "../include/texture_database.h"

TextureDatabase TextureDatabase::instance;

TextureDatabase::TextureDatabase() {
    this->textures = std::unordered_map<BlockID, TextureInterface *>();
}

void TextureDatabase::init() {

    LOG(INFO) << "Initializing Texture Database ...";

    for (const auto &file : fs::directory_iterator(fs::current_path().parent_path().string() + "/resources/blocks/")) {
        LOG(INFO) << "Processing " + file.path().filename().string() << " for textures.";

        // Inserts BlockId => Texture
        BlockFileData texData = readBlockFile("./resources/blocks/" + file.path().filename().string());
        TextureInterface *tex = nullptr;
        if (texData.textureType == TEXTURE2D) {
            tex = new Texture(texData.textureFile);
        } else if (texData.textureType == CUBEMAP) {
            tex = new CubeMap(texData.cubeFaceFiles);
        }
        instance.textures.insert({texData.ID, tex});

        LOG(INFO) << "Finished processing " + file.path().filename().string() + ".";
    }
}

TextureInterface *TextureDatabase::getTextureByBlockID(BlockID id) {
    return instance.textures[id];
}