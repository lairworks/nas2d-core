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
	mFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	mVertShader = glCreateShader(GL_VERTEX_SHADER);

	if(mFragShader == 0 || mVertShader == 0)
		cout << "\t\tShaders unavailable." << endl;
	
	mShaderProgram = glCreateProgram();
	
	// Read in the shader source code
	cout << "\t\tBuilding shaders:" << endl;
//	loadShader("shaders/helloworld/helloworld_frag.shader", mFragShader);
//	loadShader("shaders/helloworld/helloworld_vert.shader", mVertShader);

	cout << "\t\tAvailable Shaders:" << endl;
	for(size_t i = 0; i < mShaderList.size(); i++)
		cout << "\t\t- " << mShaderList[i] << endl;
}


ShaderManager::~ShaderManager()
{
	
}


void ShaderManager::loadShader(const string& src, GLuint shader)
{
	File f = Utility<Filesystem>::get().open(src);
	const GLchar* c = f.raw_bytes();
	int len = strlen(f.raw_bytes());
	glShaderSource(shader, 1, &c, NULL);
	
	if(compileShader(shader) != -1)
	{
		attachShader(shader);
		glLinkProgram(mShaderProgram);
		printLog(mShaderProgram);
		// I guess this is if the shader compiled properly?
		mShaderList.push_back(src);
	}
	
	glUseProgram(mShaderProgram);
}


void ShaderManager::attachShader(GLuint shader)
{
	glAttachShader(mShaderProgram, shader);
}


// Does this need a return value? I'm not so sure...
int ShaderManager::compileShader(GLuint shader)
{
	GLint result;
	glCompileShader(shader);
	
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	
	if(result == GL_TRUE)
	{
		//cout << "\t\t- Shader compilation was successful." << endl;
		return 0; // Should be bool type, not C style returns.
	}
	else
	{
		cout << "\t\t- Shader compilation failed." << endl;
		printLog(shader);
		return -1; // Should be bool type, not C style returns.
	}
		
}


void ShaderManager::printLog(GLuint obj)
{
	vector<char> infoLog;
	GLint infoLen;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infoLen);
	infoLog.resize(infoLen);
	
	std::cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
	glGetShaderInfoLog(obj, sizeof(infoLog), &infoLen, &infoLog[0]);
	
	std::cerr << string(infoLog.begin(), infoLog.end()) << std::endl;
}
