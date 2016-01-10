// Robin Heinemann Copyright 2015

#ifndef _NUMBERRENDERER_H
#define _NUMBERRENDERER_H

#include <GL/glew.h>
#include <cstring>
#include <vector>

#include "ShaderProgram.h"

class NumberRenderer {
public:
	NumberRenderer(int screenWidth, int screenHeight);

	int setScreenDimension(int screenWidth, int screenHeight);
	
	int addText(int x, int y, int height, const char * text);
	int clearText();

	int render();
	
	~NumberRenderer();
private:
	struct glyph {
		float x;
		float y;
		float width;
		float height;
		int id;
	};

	unsigned int m_vbo = 0, m_vao = 0, m_tex[13], m_width = 0, m_height = 0, m_ebo = 0;
	std::vector<int>  m_elemData;
	std::vector<float> m_data;
	ShaderProgram * m_shaderProgram = nullptr;
	std::vector<glyph> m_glyphs;

	int m_charToID(char c);
	float m_toCoords(int x, int screenSize);
	int m_loadTexture(const char * data, int target);
	int m_glyphUpdateData();
};

#endif
