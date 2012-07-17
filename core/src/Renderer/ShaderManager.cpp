//
//  ShaderManager.cpp
//  NAS2D
//
//  Created by Trevor Allen on 7/11/12.
//
//

#include "NAS2D/Renderer/ShaderManager.h"

ShaderManager::ShaderManager()
{
	mFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	mVertShader = glCreateShader(GL_VERTEX_SHADER);
	
	mShaderProgram = glCreateProgram();
}
<<<<<<< .mine

ShaderManager::~ShaderManager()
{
	
}

void ShaderManager::loadShader(std::string& src, GLuint shader)
{
	const GLchar* tmp = static_cast<const GLchar*>(src.c_str());
	glShaderSource(shader, 1, &tmp, NULL);
	
	if (compileShader(shader) != -1)
	{
		attachShader(shader);
		glLinkProgram(mShaderProgram);
		printLog(mShaderProgram);
	}
	
	glUseProgram(mShaderProgram);
}

void ShaderManager::attachShader(GLuint shader)
{
	glAttachShader(mShaderProgram, shader);
}

int ShaderManager::compileShader(GLuint shader)
{
	GLint result;
	glCompileShader(shader);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	
	if (result == GL_TRUE)
	{
		std::cout << "Shader compilation was successful!" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Shader compilation failed! Here is the log:\n" << std::endl;
		printLog(shader);
		return -1;
	}
		
}

void ShaderManager::printLog(GLuint obj)
{

	int infologLength = 0;
	int maxLength;
		
	if(glIsShader(obj))
		glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
	else
		glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
		
	char infoLog[maxLength];
		
	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
		
	if (infologLength > 0)
		printf("%s\n",infoLog);
}
=======

ShaderManager::~ShaderManager()
{
	
}

void ShaderManager::loadShader(std::string& src, GLuint shader)
{
	const GLchar* tmp = static_cast<const GLchar*>(src.c_str());
	glShaderSource(shader, 1, &tmp, NULL);
	
	if (compileShader(shader) != -1)
	{
		attachShader(shader);
		glLinkProgram(mShaderProgram);
		printLog(mShaderProgram);
	}
	
	glUseProgram(mShaderProgram);
}

void ShaderManager::attachShader(GLuint shader)
{
	glAttachShader(mShaderProgram, shader);
}

int ShaderManager::compileShader(GLuint shader)
{
	GLint result;
	glCompileShader(shader);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	
	if (result == GL_TRUE)
	{
		std::cout << "Shader compilation was successful!" << std::endl;
		return 0;
	}
	else
	{
		std::cout << "Shader compilation failed! Here is the log:\n" << std::endl;
		printLog(shader);
		return -1;
	}
		
}

void ShaderManager::printLog(GLuint obj)
{
	/*

	int infologLength = 0;
	int maxLength;
		
	if(glIsShader(obj))
		glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &maxLength);
	else
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &maxLength);
		
	char infoLog[maxLength];
		
	if (glIsShader(obj))
		glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	else
		glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
		
	if (infologLength > 0)
		printf("%s\n",infoLog);
	
	*/
}
>>>>>>> .r83
