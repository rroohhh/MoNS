// Robin Heinemann Copyright (C) 2015

#ifndef _GLPlot_H
#define _GLPlot_H

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

#include "config.h"
#include "AxisScale.h"
#include "NumberRenderer.h"
#include "ShaderProgram.h"
#include "io/log.h"

using namespace io;

class GLPlot {
public:
    GLPlot();
    int plot(float * /*data*/, unsigned int count);

    ~GLPlot();

private:
    struct vo {
        unsigned int vao;
        unsigned int vbo;
    };

    int m_createWindow();
    int m_renderLoop(sf::RenderWindow * /*window*/);

    inline float * m_pixelsToCoords(int x, int y, int screenWidth,
                                    int screenHeight);
    inline float m_pixelToScreen(int x, int screenSize);

    int m_generateAxisData(int screenWidth, int screenHeight, float centerX,
                           float centerY, float scaleX, float scaleY);

    int     m_count    = 0;
    float * m_data     = nullptr;
    float * m_axisData = nullptr;
    bool    m_changed  = false;

    vo m_points{};
    vo m_axis{};
    vo m_axisLabels{};

    ShaderProgram *  m_shaderProgram  = nullptr;
    NumberRenderer * m_numberRenderer = nullptr;
};

#endif
