// ==================================================================================
// = NAS2D
// = Copyright © 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef __NAS2D__ShaderManager__
#define __NAS2D__ShaderManager__

#include <iostream>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#elif defined(WIN32)
#include "GLee.h"
#else
#include "SDL/SDL_opengl.h"
#endif

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	
	void loadShader(std::string& src, GLuint shader);
	void attachShader(GLuint shader);
	
protected:
	int compileShader(GLuint shader);
	void printLog(GLuint obj);
	
private:
	GLuint mShaderProgram;
	
	GLuint mFragShader;
	GLuint mVertShader;
	
};

#endif /* defined(__NAS2D__ShaderManager__) */