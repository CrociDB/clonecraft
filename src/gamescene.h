#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_

#include <vector>
#include <unordered_map>
#include <PerlinNoise.hpp>
#include <cstdlib>
#include <ctime>

#include "scene.h"
#include "chunk.h"

#define GAME_CHUNK_RADIUS           4
#define GAME_CHUNK_MAX              GAME_CHUNK_RADIUS * GAME_CHUNK_RADIUS * 4 * 2
#define GAME_CHUNK_GRID_AT(X, Y)    X + 100 * Y

class GameScene : public Scene
{
private:
    Material* m_BlockMaterial;
    std::vector<Chunk*> m_Chunks;
    std::unordered_map<int, Chunk*> m_AvailableChunks;

    siv::PerlinNoise* m_Noise;

    void createChunkAt(int x, int z);
public:
    void buildMap();

    void update();

    GameScene();
    ~GameScene();

};

#endif