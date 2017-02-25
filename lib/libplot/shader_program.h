
// Robin Heinemann Copyright (C) 2015

#ifndef __SHADERPROGRAM_H_
#define __SHADERPROGRAM_H_

#include <cstring>
#include <iostream>

#include <GL/glew.h>

class shader_program {
public:
    shader_program(const char * vertexShaderSource,
                   const char * fragmentShaderSource);
    int vertex_attrib_pointer(const char * name, int count, int type,
                              bool normalized, int stride, const void * offset);
    int use_program();
    int uniform2f(const char * name, float x, float y);
    int uniform1i(const char * name, int x);

private:
    int new_shader(int type, const char * source);

    int m_id, m_vid, m_fid;
};

#endif
