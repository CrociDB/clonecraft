#pragma once

#include "engine.h"
#include "skybox.h"

namespace annileen
{
	void Skybox::createModel()
	{
		annileen::Shader* shader = Engine::getInstance()->getAssetManager()->loadShader("unlit.vs", "unlit.fs");
		std::shared_ptr<Material> material = std::make_shared<Material>();
		material->init(shader);

		bgfx::VertexLayout vlayout;
		vlayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.end();

		auto vdata = bgfx::makeRef(s_skyboxCubeVertices, sizeof(s_skyboxCubeVertices));
		auto idata = bgfx::makeRef(s_skyboxCubeTriList, sizeof(s_skyboxCubeTriList));

		Mesh* mesh = new Mesh();
		mesh->init(vdata, vlayout, idata, sizeof(s_skyboxCubeTriList) / sizeof(uint16_t));

		m_Model = std::make_shared<Model>();
		m_Model->init(mesh, material);
	}

	Skybox::Skybox(Cubemap* cubemap) : m_Cubemap(cubemap)
	{
		createModel();
	}
	
	Skybox::~Skybox()
	{
	}
}
