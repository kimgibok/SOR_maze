#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "HandleGLError.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

//Set up the namespace to prevent duplication
namespace ShaderNamespace2 {
	class ShaderSrc
	{
		std::string src;
		unsigned int shaderId;
	public:
		//Error handling
		void PrintShaderErrorLog();

		//Get source code from a file
		bool ReadFile(std::string filename);

		//Compile the shader
		//Returns the id that OpenGL gives the compiled shader
		unsigned int CompileSrc(GLenum shaderType);
	};

	class Shader
	{
		ShaderSrc vert, frag;
		unsigned int programId; //ID for the program
	public:
		//Error handling
		void PrintProgramErrorLog();

		unsigned int GetId();

		//Get the location of a uniform
		int GetUniformLocation(std::string uniformName);

		void use() const
		{
			glUseProgram(programId);
		}

		void setMat4(const std::string& name, const glm::mat4& matrix) const
		{
			glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
		}

		// Set a 3D vector uniform in the shader
		void setVec3(const std::string& name, const glm::vec3& value) const
		{
			glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, glm::value_ptr(value));
		}

		//Constructor
		Shader(std::string vertFile, std::string fragFile, bool use);

		//Default constructor
		Shader();
	};
}


#endif // SHADER_H