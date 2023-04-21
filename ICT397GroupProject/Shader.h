#pragma once

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

/*
 * @class Shader
 * @brief Contains an ID and functions to use a shader
 *
 * Shader stores an ID for the current shader and the functions
 * necessary to create and use a shader specifically for
 * our games needs
 *
 * @author Aidan O'Connor
 * @version 01
 * @date 21/4/2023 Aidan O'Connor, finished
 *
 * @bug No bugs have currently been found
 */

class Shader
{
private:
	/*
	 * @brief Checks for shader errors
	 *
	 * Checks for errors and displays error messages given that
	 * something has gone wrong during shader compilation
	 *
	 * @param shader, type
	 * @return void
	 */
	void checkCompileErrors(GLuint shader, std::string type);

public:
	/// stores the ID of the current shader
	unsigned int ID;

	/*
	 * @brief Shader constructor given paramater values
	 *
	 * Shader constructor that takes in the file paths of the vertex
	 * and fragment shader and uses those to create the corresponding
	 * shaders
	 *
	 * @param vertexShaderpath, fragmentShaderPath
	 * @return void
	 */
	Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

	/*
	 * @brief Uses the current shader
	 *
	 * Calls the function to use the current shader
	 *
	 * @return void
	 */
	void use();

	/*
	 * @brief Sets an int value for the frament shader
	 *
	 * This functions sets the value of the texture id
	 * in the fragment shader
	 *
	 * @return void
	 */
	void setInt(const std::string& name, int value) const;

	/*
	 * @brief Sets a matrix value to the vertex shader
	 *
	 * This functions sets the value of a matrix to the
	 * vertex shader
	 *
	 * @return void
	 */
	void setMat4(const std::string& name, const glm::mat4& mat) const;

	/*
	 * @brief Sets a vector value to the vertex shader
	 *
	 * This functions sets the value of a vector with 4 values 
	 * to the vertex shader
	 *
	 * @return void
	 */
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

	/*
	 * @brief Sets a float value to the vertex shader
	 *
	 * This functions sets the value of a float
	 * to the vertex shader
	 *
	 * @return void
	 */
    void setFloat(const std::string& name, float value) const;
};