// Robin Heinemann Copyright (C) 2015

#ifndef __GLPLOT_H_
#define __GLPLOT_H_

#include <GL/glew.h>
#include <GL/gl.h>

// ToDo(robin): minimum sfml includes for renderwindow yet to be found
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window.hpp"

// does not work -> uses map
// #include <SFML/Graphics.hpp>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <thread>

#include "axis_scale.h"
#include "config.h"
#include "number_renderer.h"
#include "shader_program.h"

class glplot {
public:
    glplot();
    int plot(float * /*data*/, unsigned int count);

    ~glplot();

private:
    struct vo {
        unsigned int vao;
        unsigned int vbo;
    };

    int create_window();
    int render_loop(sf::RenderWindow * /*window*/);

    inline float * pixels_to_coords(int x, int y, int screenWidth,
                                    int screenHeight);
    inline float pixel_to_screen(int x, int screenSize);

    int generate_axis_data(int screenWidth, int screenHeight, float centerX,
                           float centerY, float scaleX, float scaleY);

    int     count    = 0;
    float * data     = nullptr;
    float * axis_data = nullptr;
    bool    changed  = false;

    vo points{};
    vo axis{};
    vo axis_labels{};

    shader_program * shader = nullptr;
    number_renderer * nrenderer = nullptr;
};

#endif
