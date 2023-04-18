#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H
#include "../../oglpch.h"

std::string get_file_contents(const char* filename);

class Shader
{
private:
	bool IsInit_ = false;
public:
	// Reference ID of the Shader Program
	GLuint ID;
	Shader() = default;
	// Constructor that build the Shader Program from 2 different shaders
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	GLint GetUniformLocation(const std::string& name);
	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4fv(const std::string& name, GLuint count, GLfloat* value);
	void SetUniformMatrix4fv(const std::string& name, GLfloat* value_ptr);
private:
	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
	std::unordered_map<std::string, int> LocationCache;
};


#endif