#pragma once

#include <iostream>
#include <map>
#include <vector>

#include <bimg/decode.h>
#include <bgfx/bgfx.h>
#include <toml.hpp>

#include "asset.h"
#include "shader.h"
#include "texture.h"
#include "cubemap.h"

namespace annileen
{
	class AssetManager
	{
	private:
		std::map<std::string, AssetTableEntry> m_Assets;

		void loadAssetTable(const std::string& assetfile);
		AssetType getType(const std::string& typetext);
		AssetTableEntry* getAssetEntry(const std::string& assetname);

		void unloadAssets();

		const bgfx::Memory* loadBinaryFile(const std::string& filename);
		std::tuple<bgfx::TextureHandle, bgfx::TextureInfo, bimg::ImageContainer*> loadTextureData(const std::string& file, const TextureDescriptor& descriptor);

	public:
		AssetManager(const std::string& assetfile);
		~AssetManager();

		// Asset loading functions
		Shader* loadShader(const std::string& vertex, const std::string& fragment);
		Texture* loadTexture(const std::string& tex);
		Cubemap* loadCubemap(const std::string& name);

		// Asset descriptor loading functions
		TextureDescriptor loadTextureDescriptor(AssetTableEntry* asset);
		CubemapDescriptor loadCubemapDescriptor(AssetTableEntry* asset);
	};
}
