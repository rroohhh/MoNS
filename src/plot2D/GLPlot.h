// Robin Heinemann Copyright (C) 2015

#ifndef _GLPlot_H
#define _GLPlot_H

#include <GL/glew.h>
#include <GL/gl.h>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cstring>
#include <cstdio>
#include <thread>

#include "NumberRenderer.h"
#include "ShaderProgram.h"
#include "AxisScale.h"
#include "../config.h"

class GLPlot {
public:
	GLPlot();
	int plot(float *, unsigned int count);

	~GLPlot();
private:
	typedef struct {
		unsigned int vao;
		unsigned int vbo;
	} vo;
	
    int m_createWindow();
	int m_renderLoop(sf::RenderWindow *);

	inline float * m_pixelsToCoords(int x, int y, int screenWidth, int screenHeight);
	inline float m_pixelToScreen(int x, int screenWidth);

	int m_generateAxisData(int screenWidth, int screenHeight, float centerX, float centerY, float scaleX, float scaleY);
	
	int m_count = 0;
	float * m_data = nullptr;
	float * m_axisData = nullptr;
	bool m_changed = false;
	
	vo m_points;
	vo m_axis;
	vo m_axisLabels;
	
    ShaderProgram * m_shaderProgram = nullptr;
	NumberRenderer * m_numberRenderer = nullptr;
};

#endif
