#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Shader
{
private:
    void checkCompileErrors(GLuint shader, std::string type);

public:
    unsigned int ID;

    Shader() {};

    Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

    void use();

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setMat4(const std::string& name, const glm::mat4& mat) const;

    void setVec4(const std::string& name, const glm::fvec4 color) const;

	/*
	 * @brief Sets a vector value to the vertex shader
	 *
	 * This functions sets the value of a vector with 3 values
	 * to the vertex shader
	 *
	 * @return void
	 */
	void setVec3(const std::string& name, const glm::vec3& value) const;

	/*
	 * @brief Sets a vector value to the fragment shader
	 *
	 * This functions sets the value of a vector with 3 values
	 * to the fragment shader without using a specialised data type
	 *
	 * @return void
	 */
	void setVec3(const std::string& name, float x, float y, float z) const;
};