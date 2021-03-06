
// Robin Heinemann Copyright (C) 2015

#include "number_renderer.h"
#include "glyphs.h"

number_renderer::number_renderer(int screenWidth, int screenHeight) {
    m_width  = screenWidth;
    m_height = screenHeight;

    char * images[13] = {
        Glyphs::zero, Glyphs::one,   Glyphs::two,   Glyphs::three, Glyphs::four,
        Glyphs::five, Glyphs::six,   Glyphs::seven, Glyphs::eight, Glyphs::nine,
        Glyphs::e,    Glyphs::minus, Glyphs::dot,
    };

    for(int i = 0; i < 13; i++) { tex[i] = load_texture(images[i], i); }

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), data.data(),
                 GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(int), elem_data.data(),
                 GL_STATIC_DRAW);

    program = new shader_program(
        "#version 150\n"
        "in vec2 TexCoord;\n"
        "in vec2 vp;\n"
        "out vec2 texCoord;\n"
        "void main() {\n"
        "    texCoord = TexCoord;\n"
        "    gl_Position = vec4(vp.x, vp.y, 0.0, 1.0);\n"
        "}",
        "#version 150\n"
        "in vec2 texCoord;\n"
        "out vec4 frag_colour;\n"
        "uniform sampler2D tex;\n"
        "void main() {\n"
        "    float dist = texture(tex, texCoord).x;"
        "    float alpha = smoothstep(0.45 - 0.05, 0.5, dist);"
        "    frag_colour = vec4(0.0, 0.0, 0.0, alpha);\n"
        "}");

    program->vertex_attrib_pointer("vp", 2, GL_FLOAT, false, 4 * sizeof(float),
                                   nullptr);
    program->vertex_attrib_pointer("TexCoord", 2, GL_FLOAT, false,
                                   4 * sizeof(float),
                                   (void *)(2 * sizeof(float)));
}

number_renderer::~number_renderer() = default;

int number_renderer::set_screen_dimension(int screenWidth, int screenHeight) {
    m_width  = screenWidth;
    m_height = screenHeight;

    return 0;
}

float number_renderer::to_coords(int x, int screenSize) {
    return 2.0 * (((float)x) / ((float)screenSize)) - 1.0;
}

int number_renderer::char_to_id(char c) {
    int ret = 0;

    switch(c) {

    case '0': {
        ret = 0;
        break;
    }

    case '1': {
        ret = 1;
        break;
    }

    case '2': {
        ret = 2;
        break;
    }

    case '3': {
        ret = 3;
        break;
    }

    case '4': {
        ret = 4;
        break;
    }

    case '5': {
        ret = 5;
        break;
    }

    case '6': {
        ret = 6;
        break;
    }

    case '7': {
        ret = 7;
        break;
    }

    case '8': {
        ret = 8;
        break;
    }

    case '9': {
        ret = 9;
        break;
    }

    case 'e': {
        ret = 10;
        break;
    }

    case '-': {
        ret = 11;
        break;
    }

    case '.': {
        ret = 12;
        break;
    }

    default: {
        ret = 0;
        break;
    }
    }

    return ret;
}

int number_renderer::add_text(int x, int y, int height, const char * text) {
    float ratio         = (float)height / (float)Glyphs::height;
    int   length        = strlen(text);
    float screen_height = (float)height / (float)m_height;
    float screen_width  = (float)(ratio * (float)Glyphs::width) / m_width;
    float cur_x         = to_coords(x, m_width);
    float cur_y         = to_coords(y, m_height);
    glyph cur_glyph{};
    cur_glyph.height = screen_height;
    cur_glyph.y      = cur_y;
    cur_glyph.width  = screen_width;

    for(int i = 0; i < length; i++) {
        cur_glyph.id = char_to_id(text[i]);
        cur_glyph.x  = cur_x;

        cur_x += screen_width;

        glyphs.push_back(cur_glyph);
    }

    glyph_update_data();

    return 0;
}

int number_renderer::clear_text() {
    glyphs.clear();

    return 0;
}

int number_renderer::glyph_update_data() {
    data.clear();
    elem_data.clear();
    data.resize(16 * glyphs.size());
    elem_data.resize(6 * glyphs.size());

    for(unsigned int i = 0; i < glyphs.size(); i++) {
        elem_data[6 * i]     = 4 * i + 0;
        elem_data[6 * i + 1] = 4 * i + 1;
        elem_data[6 * i + 2] = 4 * i + 2;
        elem_data[6 * i + 3] = 4 * i + 2;
        elem_data[6 * i + 4] = 4 * i + 3;
        elem_data[6 * i + 5] = 4 * i + 0;

        data[16 * i]      = glyphs[i].x;
        data[16 * i + 1]  = glyphs[i].y;
        data[16 * i + 2]  = 0.0;
        data[16 * i + 3]  = 1.0;
        data[16 * i + 4]  = glyphs[i].x + glyphs[i].width;
        data[16 * i + 5]  = glyphs[i].y;
        data[16 * i + 6]  = 1.0;
        data[16 * i + 7]  = 1.0;
        data[16 * i + 8]  = glyphs[i].x + glyphs[i].width;
        data[16 * i + 9]  = glyphs[i].y + glyphs[i].height;
        data[16 * i + 10] = 1.0;
        data[16 * i + 11] = 0.0;
        data[16 * i + 12] = glyphs[i].x;
        data[16 * i + 13] = glyphs[i].y + glyphs[i].height;
        data[16 * i + 14] = 0.0;
        data[16 * i + 15] = 0.0;
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    glBufferData(GL_ARRAY_BUFFER, 16 * glyphs.size() * sizeof(float),
                 data.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * glyphs.size() * sizeof(int),
                 elem_data.data(), GL_STATIC_DRAW);

    return 0;
}

int number_renderer::load_texture(const char * data, int target) {
    unsigned int id;

    glGenTextures(1, &id);

    glActiveTexture(GL_TEXTURE0 + target);
    // glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id);

    char * buf =
        (char *)malloc(16 * Glyphs::width * Glyphs::height * sizeof(char));

    for(int i = 0; i < Glyphs::width * Glyphs::height; i++) {
        buf[4 * i]     = data[3 * i];
        buf[4 * i + 1] = data[3 * i + 1];
        buf[4 * i + 2] = data[3 * i + 2];
        buf[4 * i + 3] = -1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Glyphs::width, Glyphs::height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, buf);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);

    free(buf);

    return id;
}

int number_renderer::render() {
    program->uniform1i("tex", 1);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBindTexture(GL_TEXTURE_2D, tex[0]);

    for(unsigned int i = 0; i < glyphs.size(); i++) {
        program->uniform1i("tex", glyphs[i].id);
        // m_shaderProgram->uniform1i("tex", 11);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                       (void *)(6 * i * sizeof(int)));
    }

    return 0;
}
