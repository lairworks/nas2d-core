// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/ShaderManager.h"

#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

ShaderManager::ShaderManager()
{
	mFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	mVertShader = glCreateShader(GL_VERTEX_SHADER);
	
	mShaderProgram = glCreateProgram();
	
	// Read in the shader source code
	std::string tmpFrag = "shaders/helloworld/helloworld.frag";
	std::string tmpVert = "shaders/helloworld/helloworld.vert";
	loadShader(tmpFrag, mFragShader);
	loadShader(tmpVert, mVertShader);
}


ShaderManager::~ShaderManager()
{
	
}


void ShaderManager::loadShader(std::string& src, GLuint shader)
{
	File f = Utility<Filesystem>::get().open(src);
	const GLchar* c = f.raw_bytes();
	int len = strlen(f.raw_bytes());
	glShaderSource(shader, 1, &c, &len);
	
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
	int maxLength = 0;
		
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
}
