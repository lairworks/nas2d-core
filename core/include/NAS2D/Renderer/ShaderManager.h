//
//  ShaderManager.h
//  NAS2D
//
//  Created by Trevor Allen on 7/11/12.
//
//

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