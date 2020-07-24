#include "uniform.h"

#include <gtc/type_ptr.hpp>

namespace annileen
{
	bgfx::UniformHandle Uniform::createUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype)
	{
		UniformData u{ bgfx::createUniform("s_texColor",  uniformtype), uniformtype };
		m_Uniforms[uniformname] = u;
		return u.m_Handle;
	}

	bgfx::UniformHandle Uniform::getOrCreateUniform(const std::string& uniformname, bgfx::UniformType::Enum uniformtype)
	{
		try
		{
			return m_Uniforms[uniformname].m_Handle;
		}
		catch (std::out_of_range& e) { }

		return createUniform(uniformname, uniformtype);
	}


	bgfx::UniformHandle Uniform::getSamplerUniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Sampler);
	}

	bgfx::UniformHandle Uniform::getVec4UniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Vec4);
	}

	bgfx::UniformHandle Uniform::getSMat3UniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Mat3);
	}

	bgfx::UniformHandle Uniform::getMat4UniformHandle(const std::string& uniformname)
	{
		return getOrCreateUniform(uniformname, bgfx::UniformType::Mat4);
	}


	void Uniform::setTextureUniform(const std::string& uniformname, const Texture* value)
	{
		bgfx::setTexture(0, getSamplerUniformHandle(uniformname), value->getHandle());
	}

	void Uniform::setVec4Uniform(const std::string& uniformname, const glm::vec4& value)
	{
		bgfx::setUniform(getVec4UniformHandle(uniformname), glm::value_ptr(value), 4);
	}

	void Uniform::setVec3Uniform(const std::string& uniformname, const glm::vec3& value)
	{
		bgfx::setUniform(getVec4UniformHandle(uniformname), glm::value_ptr(value), 3);
	}

	void Uniform::setVec2Uniform(const std::string& uniformname, const glm::vec2& value)
	{
		bgfx::setUniform(getVec4UniformHandle(uniformname), glm::value_ptr(value), 2);
	}

	void Uniform::setFloatUniform(const std::string& uniformname, const float& value)
	{
		bgfx::setUniform(getVec4UniformHandle(uniformname), &value, 1);
	}


	void Uniform::destroy()
	{
		for (const auto& [k, v] : m_Uniforms)
		{
			bgfx::destroy(v.m_Handle);
		}
	}


	Uniform::Uniform()
	{
	}

	Uniform::~Uniform()
	{
		destroy();
	}
}