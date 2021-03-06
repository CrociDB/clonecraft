#include "modelloader.h"
#include "core/logger.h"

namespace annileen
{
	ModelLoader::ModelLoader()
	{
	}

	ModelLoader::~ModelLoader()
	{
	}

	MeshGroup* ModelLoader::loadMesh(const std::string& filename, const MeshDescriptor& descriptor)
	{
		MeshGroup* meshGroup = new MeshGroup();

		Assimp::Importer importer;
		
		uint32_t flags = aiProcess_Triangulate
			| aiProcess_FlipUVs;

		if (descriptor.m_Normals == MeshDescriptor::Normals::GenerateSmooth) flags |= aiProcess_GenSmoothNormals;

		const aiScene* scene = importer.ReadFile(filename, flags);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			ANNILEEN_LOG_ERROR(LoggingChannel::Asset, "There's a problem with the mesh.");
			exit(-1);
		}

		processNode(scene->mRootNode, scene, meshGroup, descriptor);

		return meshGroup;
	}

	void ModelLoader::processNode(aiNode* node, const aiScene* scene, MeshGroup* meshGroup, const MeshDescriptor& descriptor)
	{
 		for (uint16_t i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			Mesh* mesh = new Mesh();
			convertMesh(mesh, aiMesh, scene, descriptor);
			meshGroup->m_Meshes.push_back(mesh);
		}

		for (uint16_t i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene, meshGroup, descriptor);
		}
	}

	void ModelLoader::convertMesh(Mesh* mesh, aiMesh* aiMesh, const aiScene* scene, const MeshDescriptor& descriptor)
	{
		RawMesh rawMesh;

		for (uint32_t i = 0; i < aiMesh->mNumVertices; i++)
		{
			RawVertex v;

			v.m_Position.x = aiMesh->mVertices[i].x;
			v.m_Position.y = aiMesh->mVertices[i].y;
			v.m_Position.z = aiMesh->mVertices[i].z;

			if (aiMesh->mTextureCoords[0])
			{
				v.m_UV.x = aiMesh->mTextureCoords[0][i].x;
				v.m_UV.y = aiMesh->mTextureCoords[0][i].y;
				v.m_HasUV = true;
			}
			else
			{
				v.m_HasUV = false;
			}

			if (aiMesh->mNormals)
			{
				v.m_Normal.x = aiMesh->mNormals[i].x;
				v.m_Normal.y = aiMesh->mNormals[i].y;
				v.m_Normal.z = aiMesh->mNormals[i].z;
				v.m_HasNormal = true;
			}
			else
			{
				v.m_HasNormal = false;
			}

			rawMesh.m_Vertices.push_back(v);
		}

		for (uint32_t i = 0; i < aiMesh->mNumFaces; i++)
		{
			aiFace face = aiMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; j++)
				rawMesh.m_Indices.push_back(face.mIndices[j]);
		}

		rawMesh.getMesh(mesh, descriptor);
	}
}
