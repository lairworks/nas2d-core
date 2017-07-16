// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2017 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#include "NAS2D/Renderer/ShaderManager.h"

#include "NAS2D/Filesystem.h"
#include "NAS2D/Utility.h"

using namespace std;
using namespace NAS2D;

ShaderManager::ShaderManager()
{
	cout << "\tInitializing Shader Manager... ";
	mFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	mVertShader = glCreateShader(GL_VERTEX_SHADER);

	if(mFragShader == 0 || mVertShader == 0)
		cout << endl << "\t\tShaders unavailable." << endl;

	mShaderProgram = glCreateProgram();

//	loadShader("../shaders/helloworld_frag.fragmentshader", mFragShader);
//	loadShader("../shaders/helloworld_vert.vertexshader", mVertShader);

	cout << "done.\n";
}


ShaderManager::~ShaderManager()
{
}


void ShaderManager::loadShader(const std::string& src, GLuint shader)
{
	File f = Utility<Filesystem>::get().open(src);
	const GLchar* c = f.raw_bytes();
	glShaderSource(shader, 1, &c, nullptr);

	compileShader(shader);
	attachShader(shader);
	glLinkProgram(mShaderProgram);
	mShaderList.push_back(src);

	glUseProgram(mShaderProgram);
}


void ShaderManager::attachShader(GLuint shader)
{
	glAttachShader(mShaderProgram, shader);
}


void ShaderManager::compileShader(GLuint shader)
{
	glCompileShader(shader);
	printLog(shader);
}


void ShaderManager::printLog(GLuint obj)
{
	GLint result;
	glGetShaderiv(obj, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		char infoLog[1024];
		glGetShaderInfoLog(obj, 1024, nullptr, infoLog);
		cout << "The shader at " << mShaderProgram << " failed to compile with the following error:\n"
		<< infoLog << "\n";
	}
}
