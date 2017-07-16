//
//  Shader.cpp
//  NAS2D
//
//  Created by Trevor Allen on 7/30/12.
//
//

#include "NAS2D/Resources/Shader.h"

#ifdef _WIN32
#include <windows.h>
#include "GL/glew.h"
#elif __APPLE__
#include "SDL2/SDL_opengl.h"
#elif __linux
#include "GLee.h"
#endif

#include <map>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::ifstream;
using std::map;
using std::vector;

class GLSLProgram
{
public:
	struct GLSLShader
	{
		unsigned int id;
		string filename;
		string source;
	};

	GLSLProgram(const string& vertexShader, const string& fragmentShader)
	{
		m_vertexShader.filename = vertexShader;
		m_fragmentShader.filename = fragmentShader;
	}

	virtual ~GLSLProgram()
	{
	}

	void unload()
	{
		glDetachShader(m_programID, m_vertexShader.id);
		glDetachShader(m_programID, m_fragmentShader.id);
		glDeleteShader(m_vertexShader.id);
		glDeleteShader(m_fragmentShader.id);
		glDeleteShader(m_programID);
	}

	bool initialize()
	{
		m_programID = glCreateProgram();
		m_vertexShader.id = glCreateShader(GL_VERTEX_SHADER);
		m_fragmentShader.id = glCreateShader(GL_FRAGMENT_SHADER);

		m_vertexShader.source = readFile(m_vertexShader.filename);
		m_fragmentShader.source = readFile(m_fragmentShader.filename);

		if (m_vertexShader.source.empty() || m_fragmentShader.source.empty())
		{
			return false;
		}

		const GLchar* tmp = static_cast<const GLchar*>(m_vertexShader.source.c_str());
		glShaderSource(m_vertexShader.id, 1, (const GLchar**)&tmp, nullptr);

		tmp = static_cast<const GLchar*>(m_fragmentShader.source.c_str());
		glShaderSource(m_fragmentShader.id, 1, (const GLchar**)&tmp, nullptr);

		if (!compileShader(m_vertexShader) || !compileShader(m_fragmentShader))
		{
			std::cerr << "Could not compile the shaders, they are invalid" << std::endl;
			return false;
		}

		glAttachShader(m_programID, m_vertexShader.id);
		glAttachShader(m_programID, m_fragmentShader.id);

		glLinkProgram(m_programID);
		return true;
	}

	void linkProgram()
	{
		glLinkProgram(m_programID);
	}

	GLuint getUniformLocation(const string& name)
	{
		map<string, GLuint>::iterator i = m_uniformMap.find(name);
		if (i == m_uniformMap.end())
		{
			GLuint location = glGetUniformLocation(m_programID, name.c_str());
			m_uniformMap.insert(std::make_pair(name, location));
			return location;
		}

		return (*i).second;
	}

	GLuint getAttribLocation(const string& name)
	{
		map<string, GLuint>::iterator i = m_attribMap.find(name);
		if (i == m_attribMap.end())
		{
			GLuint location = glGetAttribLocation(m_programID, name.c_str());
			m_attribMap.insert(std::make_pair(name, location));
			return location;
		}

		return (*i).second;
	}

	void sendUniform(const string& name, const int id)
	{
		GLuint location = getUniformLocation(name);
		glUniform1i(location, id);
	}

	void sendUniform4x4(const string& name, const float* matrix, bool transpose=false)
	{
		GLuint location = getUniformLocation(name);
		glUniformMatrix4fv(location, 1, transpose, matrix);
	}

	void sendUniform3x3(const string& name, const float* matrix, bool transpose=false)
	{
		GLuint location = getUniformLocation(name);
		glUniformMatrix3fv(location, 1, transpose, matrix);
	}

	void sendUniform(const string& name, const float red, const float green,
					 const float blue, const float alpha)
	{
		GLuint location = getUniformLocation(name);
		glUniform4f(location, red, green, blue, alpha);
	}

	void sendUniform(const string& name, const float x, const float y,
					 const float z)
	{
		GLuint location = getUniformLocation(name);
		glUniform3f(location, x, y, z);
	}

	void sendUniform(const string& name, const float scalar)
	{
		GLuint location = getUniformLocation(name);
		glUniform1f(location, scalar);
	}

	void bindAttrib(unsigned int index, const string& attribName)
	{
		glBindAttribLocation(m_programID, index, attribName.c_str());
	}

	void bindShader()
	{
		glUseProgram(m_programID);
	}

private:
	string readFile(const string& filename)
	{
		ifstream fileIn(filename.c_str());

		if (!fileIn.good())
		{
			std::cerr << "Could not load shader: " << filename << std::endl;
			return string();
		}

		string stringBuffer(std::istreambuf_iterator<char>(fileIn), (std::istreambuf_iterator<char>()));
		return stringBuffer;
	}

	bool compileShader(const GLSLShader& shader)
	{
		glCompileShader(shader.id);
		GLint result = 0xDEADBEEF;
		glGetShaderiv(shader.id, GL_COMPILE_STATUS, &result);

		if (!result)
		{
			std::cout << "Could not compile shader: " << shader.id << std::endl;
			outputShaderLog(shader.id);
			return false;
		}

		return true;
	}

	void outputShaderLog(unsigned int shaderID)
	{
		vector<char> infoLog;
		GLint infoLen;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLen);
		infoLog.resize(infoLen);

		std::cerr << "GLSL Shader: Shader contains errors, please validate this shader!" << std::endl;
		glGetShaderInfoLog(shaderID, sizeof(infoLog), &infoLen, &infoLog[0]);

		std::cerr << string(infoLog.begin(), infoLog.end()) << std::endl;
#ifdef _WIN32
		//MessageBox(nullptr, string(infoLog.begin(), infoLog.end()).c_str(), "Error", MB_OK);
#endif
	}

	GLSLShader m_vertexShader;
	GLSLShader m_fragmentShader;
	unsigned int m_programID;

	map<string, GLuint> m_uniformMap;
	map<string, GLuint> m_attribMap;
};
