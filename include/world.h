//
// Created by nixin72 on 2020-08-07.
//
#pragma once

#include <vector>
#include "./block.h"

class Chunk {
private:
    //   x, y, z
    std::vector<std::vector<std::vector<BlockID>>> blocks;
    int north;
    int east;
public:
    // The north/eastern corner of the chunk and the world seed
    Chunk(int north, int east, int seed) {
        this->north = north;
        this->east = east;

        for (int x = 0; x < 16; x++) {
            blocks.emplace_back();
            for (int y = 0; y < 16; y++) {
                blocks[x].emplace_back();
                for (int z = 0; z < 16; z++) {
                    blocks[x][y].emplace_back();
                }
            }
        }

        for (int x = 0; x < 16; x++) {
            for (int z = 0; z < 16; z++) {
                blocks[x][0][z] = BEDROCK;
                blocks[x][1][z] = STONE;
            }
        }
    }

    void render() {
        Entity bedrock = Entity(ModelType::CUBE, BlockID::BEDROCK);
        Entity stone = Entity(ModelType::CUBE, BlockID::STONE);
        Shader basicShaderCubeMap = Shader("../resources/shaders/ModelVertexShader.glsl",
                                           "../resources/shaders/ModelFragmentShaderCubeMap.glsl");

        for (int x = 0; x < 16; x++) {
            for (int y = 0; y < 16; y++) {
                for (int z = 0; z < 16; z++) {
                    switch (blocks[x][y][z]) {
                        case BEDROCK: bedrock.draw(basicShaderCubeMap); break;
                        case STONE: stone.draw(basicShaderCubeMap); break;
                        default: {}
                    }
                }
            }
        }
    }
};

class World {
private:
    std::vector<std::vector<Chunk>> chunks;

public:
    World(int seed) {
        for (int x = 0; x < 32; x++) {
            chunks.emplace_back();
            for (int z = 0; z < 32; z++) {
                chunks[x].emplace_back(Chunk(x, z, seed));
            }
        }
    }

    // Take camera in as an argument so you can find the positioning to know which chunks to render
    void render(Camera camera) {
        int x = camera.Position.x / 16;
        int z = camera.Position.z / 16;


        chunks[x][z].render();

    }
};


