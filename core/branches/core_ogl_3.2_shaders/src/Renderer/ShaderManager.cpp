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
	cout << "\n\tInitializing Shader Manager... ";
	
	loadDefaultShaders();
	
	cout << "\tdone.\n";
}


ShaderManager::~ShaderManager()
{
	
}

void ShaderManager::loadDefaultShaders()
{
	cout << "\n\t\tLoading default shaders... ";
	mShaderProgram = glCreateProgram();
	getError();
	
	loadShader(GL_VERTEX_SHADER, "shaders/v_shader.shader");
	loadShader(GL_FRAGMENT_SHADER, "shaders/f_shader.shader");
	
	glBindAttribLocation(mShaderProgram, 0, "position");
	glBindAttribLocation(mShaderProgram, 1, "texCoord");
	glBindAttribLocation(mShaderProgram, 2, "color");
	
	linkProgram(mShaderProgram);
	validateProgram(mShaderProgram);
	cout << "done.\n";
}


void ShaderManager::loadShader(GLenum shaderType, const std::string& shaderFile)
{
	GLuint shader = glCreateShader(shaderType);
	getError();
	File v = Utility<Filesystem>::get().open(shaderFile);
	getShaderSource(shader, v.raw_bytes());
	compileShader(shader);
	glAttachShader(mShaderProgram, shader);
	
	if (shaderType == GL_FRAGMENT_SHADER) {
		glBindFragDataLocation(mShaderProgram, 0, "fragColor");
	}
}

void ShaderManager::getShaderSource(GLuint shader, const GLchar* src)
{
	glShaderSource(shader, 1, &src, NULL);
	getError();
}
void ShaderManager::compileShader(GLuint shader)
{
	glCompileShader(shader);
	getError();
	printShaderInfoLog(shader);
}

void ShaderManager::linkProgram(GLuint program)
{
	glLinkProgram(mShaderProgram);
	getError();
	
	GLint status;
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &status);
	getError();
	
	printProgramInfoLog(mShaderProgram);
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
	getError();
	
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		getError();
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
	getError();
	
    if (infologLength > 0)
    {
        infoLog = (char *)malloc(infologLength);
        glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		getError();
        printf("%s\n",infoLog);
        free(infoLog);
    }
}

void ShaderManager::validateProgram(GLuint program)
{
	GLint logLength = 0;

	glValidateProgram(program);

	getError();
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	getError();

	cout << logLength << endl;

	if(logLength > 0)
	{
		GLchar *log = (char *)malloc(logLength);
		glGetProgramInfoLog(program, logLength, &logLength, log);
		getError();
		cout << "Program validation produced errors:\n" << log << endl;
		free(log);
	}

	GLint status = 0;
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	getError();
	if(!status)
	{
		cout << "Failed to link shader program" << endl;;
	}
}

void ShaderManager::getError()
{
	
	std::string errStr = "OpenGL Error: ";
	
	switch(glGetError())
	{
		case GL_NO_ERROR:
			return;
			break;
		case GL_INVALID_ENUM:
			errStr += "Invalid Enumerator.";
			break;
		case GL_INVALID_VALUE:
			errStr += "Invalid Value.";
			break;
		case GL_INVALID_OPERATION:
			errStr += "Invalid Operation.";
			break;
		case GL_OUT_OF_MEMORY:
			errStr += "Out of Memory.";
			break;
		default:
			errStr += "Unknown Error Code.";
			break;
	}
	
	stringstream str;
	str << errStr << endl;
	
	cout << str.str();
}
