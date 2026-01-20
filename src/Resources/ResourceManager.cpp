#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

ResourceManager::ResourceManager(const std::string& exePath)
{
	size_t found = exePath.find_last_of("/\\");
	m_path = exePath.substr(0, found);


}

std::string ResourceManager::getFileText(const std::string& filePath) const
{
	std::ifstream f;
	f.open(m_path + "/" + filePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		std::cerr << "ERROR::RESOURCE MANAGER::FILE::Failed to open " << filePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();
	return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexShaderCode = getFileText(vertexPath);
	if (vertexShaderCode.empty())
	{
		std::cerr << "ERROR::RESOURCE MANAGER::VERTEX SHADER FILE::" << vertexPath << " shader code not found" << std::endl;
		return nullptr;
	}

	std::string fragmentShaderCode = getFileText(fragmentPath);
	if (fragmentShaderCode.empty())
	{
		std::cerr << "ERROR::RESOURCE MANAGER::FRAGMENT SHADER FILE::" << fragmentPath << " shader code not found" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexShaderCode, fragmentShaderCode)).first->second;
	if (!newShader->isCompiled())
	{
		std::cerr << "ERROR::RESOURCE MANAGER::SHADER PROGRAM:: can't create shader program\n"
			<< "\tVERTEX " << vertexPath << "\n"
			<< "\tFRAGMENT " << fragmentPath << std::endl;

		return nullptr;
	}

	return newShader;

}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
	if (it != m_shaderPrograms.end())
	{
		return it->second;
	}

	std::cerr << "ERROR::RESOURCE MANAGER::SHADER PROGRAM MAP:: " << shaderName << " is not exist" << std::endl;
	return nullptr;
}