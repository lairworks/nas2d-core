// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
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

ShaderManager::ShaderManager()
{
	cout << "\tInitializing Shader Manager... ";
	mFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	mVertShader = glCreateShader(GL_VERTEX_SHADER);

	if(mFragShader == 0 || mVertShader == 0)
		cout << endl << "\t\tShaders unavailable." << endl;
	
	mShaderProgram = glCreateProgram();
	
	loadShader("shaders/v_shader.shader", "shaders/f_shader.shader");
	
	cout << "done.\n";
}


ShaderManager::~ShaderManager()
{
	
}


void ShaderManager::loadShader(const std::string& vertexShader, const std::string& fragShader)
{
	File v = Utility<Filesystem>::get().open(vertexShader);
	const GLchar* c = v.raw_bytes();
	glShaderSource(mVertShader, 1, &c, NULL);
	glCompileShader(mVertShader);
	printShaderInfoLog(mVertShader);
	glAttachShader(mShaderProgram, mVertShader);
	
	File f = Utility<Filesystem>::get().open(fragShader);
	const GLchar* t = f.raw_bytes();
	glShaderSource(mFragShader, 1, &t, NULL);
	glCompileShader(mFragShader);
	printShaderInfoLog(mFragShader);
	glAttachShader(mShaderProgram, mFragShader);
	
	if (!mShaderProgram) {
		mShaderProgram = glCreateProgram();
	}
	
	glLinkProgram(mShaderProgram);
	GLint status;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	printProgramInfoLog(mShaderProgram);
	mShaderProgramList.push_back(mShaderProgram);
	
	glUseProgram(mShaderProgram);
}

GLuint ShaderManager::getShaderProgram()
{
	return mShaderProgram;
}


void ShaderManager::printShaderInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
	
    glGetShaderiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
	
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void ShaderManager::printProgramInfoLog(GLuint obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;
	
    glGetProgramiv(obj, GL_INFO_LOG_LENGTH,&infologLength);
	
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
        printf("%s\n",infoLog);
        free(infoLog);
    }
}
