#ifndef _CHUNK_H_
#define _CHUNK_H_

#include <cstdlib>
#include <ctime>
#include <PerlinNoise.hpp>

#include "engine/material.h"
#include "engine/model.h"
#include "engine/scenenode.h"
#include "data.h"

#define CHUNK_WIDTH             16
#define CHUNK_HEIGHT            32
#define CHUNK_DEPTH             16
#define CHUNK_TOTAL_VOXELS      CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH
#define CHUNK_PERIOD            6
#define CHUNK_OCTAVE            35.8f

using namespace annileen;

enum BlockType
{
    BlockEmpty = -1,
    BlockDirt = 0,
    BlockGrass,
    BlockSand,
	BlockStone
};

class Chunk
{
private:
    int m_WorldX;
    int m_WorldZ;
    
    siv::PerlinNoise* m_Noise;

    BlockType* m_Grid;

    std::shared_ptr<Material> m_Material;
    Mesh* m_Mesh = nullptr;
    std::shared_ptr<Model> m_Model = nullptr;
    SceneNode* m_Node = nullptr;

    void generateMesh();
    float* generateMeshData(int* meshSize, int** indexes, int* indexSize);

    bool gridEmpty(int x, int y, int z);

public:
    void setNoise(siv::PerlinNoise* noise) { m_Noise = noise; };
    void setMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    void generateGrid();
    Mesh* getMesh();
    SceneNode* getSceneNode();

    Chunk(int wx, int wz);
    ~Chunk();
};

#endif
