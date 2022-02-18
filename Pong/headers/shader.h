#pragma once

#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

// Stores shader sources
struct ProgramShaderSource {
	std::string vertex_shader;
	std::string fragment_shader;
};

class Shader {
private:
	std::string m_filepath;
	unsigned int m_rendererID;
	std::unordered_map<std::string, int> m_uniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string& name, int value);
	void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	ProgramShaderSource parseShader(const std::string& filepath);
	unsigned int compileShader(unsigned int type, const std::string& source);
	int createShader(const std::string& vertex_shader, const std::string& fragment_shader);
	int getUniformLocation(const std::string& name);
};