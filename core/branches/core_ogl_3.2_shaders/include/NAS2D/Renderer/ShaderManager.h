// ==================================================================================
// = NAS2D
// = Copyright � 2008 - 2012 New Age Software
// ==================================================================================
// = NAS2D is distributed under the terms of the zlib license. You are free to copy,
// = modify and distribute the software under the terms of the zlib license.
// = 
// = Acknowledgement of your use of NAS2D is appriciated but is not required.
// ==================================================================================

#ifndef __NAS2D__ShaderManager__
#define __NAS2D__ShaderManager__

#include "NAS2D/Common.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#elif defined(WIN32)
#include "GLee.h"
#define NO_SDL_GLEXT
#include "SDL/SDL_opengl.h"
#else
#include "SDL/SDL_opengl.h"
#endif


#include <iostream>
#include <string>
#include <vector>


/**
 * Document me!
 */
class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	
	void loadShader(const std::string& vertexShader, const std::string& fragShader);
	void attachShader(GLuint shader);
	GLuint getShaderProgram();
	
protected:
	void compileShader(GLuint shader);
	void printShaderInfoLog(GLuint obj);
	void printProgramInfoLog(GLuint obj);
	
private:

	GLuint		mShaderProgram;
	
	GLuint		mFragShader;
	GLuint		mVertShader;

	std::vector<GLuint>	mShaderProgramList;
};

#endif /* defined(__NAS2D__ShaderManager__) */