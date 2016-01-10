
// Robin Heinemann Copyright (C) 2015

#ifndef _SHADERPROGRAM_H
#define _SHADERPROGRAM_H

#include <cstring>
#include <iostream>

#include <GL/glew.h>

class ShaderProgram {
public:
	ShaderProgram(const char * vertexShaderSource, const char * fragmentShaderSource);
	int vertexAttribPointer(const char * name, int count, int type, bool normalized, int stride, const void * offset);
	int useProgram();
	int uniform2f(const char * name, float x, float y);
	int uniform1i(const char * name, int x);
private:
	int m_newShader(int type, const char * source);
	
	int m_id, m_vid, m_fid;
};

#endif
