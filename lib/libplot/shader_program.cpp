// Robin Heinemann Copyright (C) 2015

#include "shader_program.h"

shader_program::shader_program(const char * vertexShaderSource,
                               const char * fragmentShaderSource) {
    m_id  = glCreateProgram();
    m_vid = new_shader(GL_VERTEX_SHADER, vertexShaderSource);
    m_fid = new_shader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    glAttachShader(m_id, m_vid);
    glAttachShader(m_id, m_fid);
    glLinkProgram(m_id);
    glValidateProgram(m_id);
}

int shader_program::vertex_attrib_pointer(const char * name, int count, int type,
                                        bool normalized, int stride,
                                        const void * offset) {
    glUseProgram(m_id);

    int attrib = glGetAttribLocation(m_id, name);

    glVertexAttribPointer(attrib, count, type,
                          static_cast<GLboolean>(normalized), stride, offset);
    glEnableVertexAttribArray(attrib);

    return 0;
}

int shader_program::use_program() {
    glUseProgram(m_id);

    return 0;
}

int shader_program::uniform2f(const char * name, float x, float y) {
    glUseProgram(m_id);

    int uniformID = glGetUniformLocation(m_id, name);

    glUniform2f(uniformID, x, y);

    return 0;
}

int shader_program::uniform1i(const char * name, int x) {
    glUseProgram(m_id);

    glUniform1i(glGetUniformLocation(m_id, name), x);

    return 0;
}

int shader_program::new_shader(int type, const char * source) {
    int id = glCreateShader(type), tmp = 0;
    int sourceLength = strlen(source);

    char * buf = (char *)malloc((sourceLength + 1) * sizeof(char));

    //    strcpy(buf, source);

    glShaderSource(id, 1, &source, &sourceLength);
    glCompileShader(id);

    glGetShaderiv(id, GL_COMPILE_STATUS, &tmp);

    glGetShaderInfoLog(id, 1024, &tmp, buf);

    free(buf);

    return id;
}
