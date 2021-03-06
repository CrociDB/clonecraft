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
#define CHUNK_HEIGHT            80
#define CHUNK_DEPTH             16
#define CHUNK_TOTAL_VOXELS      CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_DEPTH
#define CHUNK_PERIOD            6
#define CHUNK_OCTAVE            35.8f

using namespace annileen;

enum BlockType : uint8_t
{
    BlockEmpty = 255,
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
    MeshGroup* m_MeshGroup = nullptr;
    ModelPtr m_Model = nullptr;
    SceneNodePtr m_Node = nullptr;

    void generateMesh();
    float* generateMeshData(int* meshSize);

    bool gridEmpty(int x, int y, int z);

public:
    void setNoise(siv::PerlinNoise* noise) { m_Noise = noise; };
    void setMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    void generateGrid();
    SceneNodePtr getSceneNode();

    Chunk(int wx, int wz);
    ~Chunk();
};

#endif
