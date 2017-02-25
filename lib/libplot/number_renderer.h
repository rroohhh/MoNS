// Robin Heinemann Copyright 2015

#ifndef __NUMBERRENDERER_H_
#define __NUMBERRENDERER_H_

#include <GL/glew.h>
#include <cstring>
#include <vector>

#include "shader_program.h"

class number_renderer {
public:
    number_renderer(int screenWidth, int screenHeight);

    int set_screen_dimension(int screenWidth, int screenHeight);

    int add_text(int x, int y, int height, const char * text);
    int clear_text();

    int render();

    ~number_renderer();

private:
    struct glyph {
        float x;
        float y;
        float width;
        float height;
        int   id;
    };

    unsigned int vbo = 0, vao = 0, tex[13]{}, m_width = 0, m_height = 0,
                 ebo = 0;
    std::vector<int>   elem_data;
    std::vector<float> data;
    shader_program *   program = nullptr;
    std::vector<glyph> glyphs;

    int char_to_id(char c);
    float to_coords(int x, int screenSize);
    int load_texture(const char * data, int target);
    int glyph_update_data();
};

#endif
