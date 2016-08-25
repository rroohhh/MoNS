// Robin Heinemann Copyright (C) 2015

#include "GLPlot.h"

GLPlot::GLPlot() {
    std::thread thread([=]() { m_createWindow(); });

    thread.detach();
}

GLPlot::~GLPlot() { glFinish(); }

int GLPlot::plot(float * data, unsigned int count) {
    m_data    = data;
    m_count   = count;
    m_changed = true;

    return 0;
}

int GLPlot::m_createWindow() {
    unsigned int status;

    sf::ContextSettings settings;
    settings.depthBits         = 24;
    settings.stencilBits       = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion      = 4;
    settings.minorVersion      = 5;

    sf::RenderWindow * window =
        new sf::RenderWindow(sf::VideoMode(800, 600), APPLICATION_NAME,
                             sf::Style::Default, settings);

    glewExperimental = true;
    status = glewInit();

    if(status != GLEW_OK) {
        std::cout << "glewInit error: " << glewGetErrorString(status)
                  << std::endl;
        exit(-1);
    }

    std::cout << "GL_VERSION : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL_VENDOR : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GL_RENDERER : " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLEW_VERSION : " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GLSL VERSION : " << glGetString(GL_SHADING_LANGUAGE_VERSION)
              << std::endl;

    if(!GLEW_VERSION_3_3) {
        std::cout << "Atleast version 3.3 of OpenGL is needed for this program "
                     "to work!"
                  << std::endl;
        exit(-1);
    }

    settings = window->getSettings();

    std::cout << "DEPTH BITS: " << settings.depthBits << std::endl;
    std::cout << "STENCIL BITS: " << settings.stencilBits << std::endl;
    std::cout << "ANTIALIASING LEVEL: " << settings.antialiasingLevel
              << std::endl;

    glGenBuffers(1, &m_points.vbo);
    glGenBuffers(1, &m_axis.vbo);
    glGenBuffers(1, &m_axisLabels.vbo);

    glGenVertexArrays(1, &m_points.vao);
    glGenVertexArrays(1, &m_axis.vao);
    glGenVertexArrays(1, &m_axisLabels.vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_points.vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * m_count * sizeof(float), m_data,
                 GL_STREAM_DRAW);
    glBindVertexArray(m_points.vao);

    m_shaderProgram = new ShaderProgram(
        "#version 150\nuniform vec2 center;\nuniform vec2 scale;\nin vec2 "
        "vp;\nvoid main() {\n   gl_Position = vec4((vp.x + center.x) / "
        "scale.x, (vp.y - center.y) / scale.y, 0.0, 1.0);\n}\n",
        "#version 150\nout vec4 frag_colour;\nvoid main () {\n    frag_colour "
        "= vec4(0.0, 0.0, 0.0, 1.0);\n}");

    m_shaderProgram->vertexAttribPointer("vp", 2, GL_FLOAT, false, 8, 0);

    // Hurts performance really hard:
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);

    // Smaller pointsize makes rendering faster
    glPointSize(3.0);
    glLineWidth(3.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    m_numberRenderer = new NumberRenderer(800, 600);

    m_renderLoop(window);

    return 0;
}

int GLPlot::m_renderLoop(sf::RenderWindow * window) {
    sf::Event event;
    bool      mouseDown = false, lines = false, scaleX = false, scaleY = false,
         axisNeedUpdate = true;
    int mouseX = 0, mouseY = 0, mouseDX = 0, mouseDY = 0,
        screenSizeX = window->getSize().x, screenSizeY = window->getSize().y;

    float scale[2] = {1.0, 1.0}, centerX = 0.0, centerY = 0.0,
          mouseDXScreen = 0.0, mouseDYScreen = 0.0;

    m_axisData = (float *)malloc(256 * sizeof(float));

    for(int i = 0; i < 256; i++) { m_axisData[i] = 0.0; }

    glClearColor(1, 1, 1, 1);

    m_numberRenderer->setScreenDimension(screenSizeX, screenSizeY);
    m_generateAxisData(screenSizeX, screenSizeY, centerX, centerY, scale[0],
                       scale[1]);

    glBindBuffer(GL_ARRAY_BUFFER, m_axis.vbo);
    glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(float), m_axisData,
                 GL_STREAM_DRAW);

    glBindVertexArray(m_axis.vao);
    m_shaderProgram->vertexAttribPointer("vp", 2, GL_FLOAT, false, 8, 0);

    while(window->isOpen()) {
        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window->close();
            } else if(event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                screenSizeX = event.size.width;
                screenSizeY = event.size.height;
                m_numberRenderer->setScreenDimension(screenSizeX, screenSizeY);
                axisNeedUpdate = true;
            } else if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    mouseDown = true;

                    mouseX = sf::Mouse::getPosition().x;
                    mouseY = sf::Mouse::getPosition().y;
                } else if(event.mouseButton.button == sf::Mouse::Right)
                    lines = !lines;
            } else if(event.type == sf::Event::MouseButtonReleased) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    mouseDown     = false;
                    centerX       = centerX - mouseDXScreen;
                    centerY       = centerY - mouseDYScreen;
                    mouseDXScreen = 0.0;
                    mouseDYScreen = 0.0;
                }
            } else if(event.type == sf::Event::MouseWheelScrolled) {
                if(scaleX && !scaleY)
                    scale[0] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                else if(scaleY && !scaleX)
                    scale[1] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                else {
                    scale[0] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                    scale[1] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                }

                axisNeedUpdate = true;
            } else if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::LShift)
                    scaleX = true;
                else if(event.key.code == sf::Keyboard::LControl)
                    scaleY = true;
            } else if(event.type == sf::Event::KeyReleased) {
                if(event.key.code == sf::Keyboard::LShift)
                    scaleX = false;
                else if(event.key.code == sf::Keyboard::LControl)
                    scaleY = false;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if(mouseDown) {
            mouseDX = mouseX - sf::Mouse::getPosition().x;
            mouseDY = mouseY - sf::Mouse::getPosition().y;

            mouseDXScreen =
                ((((double)mouseDX * 2) / (double)screenSizeX)) * scale[0];
            mouseDYScreen =
                ((((double)mouseDY * 2) / (double)screenSizeY)) * scale[1];

            axisNeedUpdate = true;
        }

        if(m_changed) {
            glBindBuffer(GL_ARRAY_BUFFER, m_points.vbo);
            glBufferData(GL_ARRAY_BUFFER, 2 * m_count * sizeof(float), m_data,
                         GL_STREAM_DRAW);

            m_changed = false;
        }

        if(axisNeedUpdate) {
            m_numberRenderer->setScreenDimension(screenSizeX, screenSizeY);
            m_generateAxisData(screenSizeX, screenSizeY,
                               centerX - mouseDXScreen, centerY - mouseDYScreen,
                               scale[0], scale[1]);

            glBindBuffer(GL_ARRAY_BUFFER, m_axis.vbo);
            glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(float), m_axisData,
                         GL_STREAM_DRAW);

            axisNeedUpdate = false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, m_points.vbo);
        m_shaderProgram->useProgram();
        glBindVertexArray(m_points.vao);

        m_shaderProgram->uniform2f("center", centerX - mouseDXScreen,
                                   centerY - mouseDYScreen);
        m_shaderProgram->uniform2f("scale", scale[0], scale[1]);

        if(lines) {
            glDrawArrays(GL_LINE_STRIP, 0, m_count);
            glDrawArrays(GL_POINTS, 0, m_count);
        } else
            glDrawArrays(GL_POINTS, 0, m_count);

        glBindBuffer(GL_ARRAY_BUFFER, m_axis.vbo);
        m_shaderProgram->useProgram();
        glBindVertexArray(m_axis.vao);

        m_shaderProgram->uniform2f("center", 0.0, 0.0);
        m_shaderProgram->uniform2f("scale", 1.0, 1.0);

        glDrawArrays(GL_LINES, 0, 128);

        m_numberRenderer->render();

        window->display();
    }

    return 1;
}

int GLPlot::m_generateAxisData(int screenWidth, int screenHeight, float centerX,
                               float centerY, float scaleX, float scaleY) {
    centerX = -centerX;
    centerY = centerY;

    float xlow  = -1.0 * scaleX + centerX;
    float xhigh = 1.0 * scaleX + centerX;
    float ylow  = -1.0 * scaleY + centerY;
    float yhigh = 1.0 * scaleY + centerY;

    float paddingX = 150.0 / (float)screenWidth;
    float paddingY = 30.0 / (float)screenHeight;

    int xExp = AxisScale::getOptimalExponent(xlow, xhigh);
    int yExp = AxisScale::getOptimalExponent(ylow, yhigh);

    m_numberRenderer->clearText();

    // Base Axis:
    m_axisData[0] = -1.0 + paddingX;
    m_axisData[1] = 1.0;

    m_axisData[2] = -1.0 + paddingX;
    m_axisData[3] = -1.0;

    m_axisData[4] = -1.0;
    m_axisData[5] = -1.0 + paddingY;

    m_axisData[6] = 1.0;
    m_axisData[7] = -1.0 + paddingY;

    int   i = 8, k = 1;
    float tmp = (floor(xlow / pow10(xExp)) * pow10(xExp) - centerX) / scaleX;
    char  buf[128];

    while(tmp <= 1.0 && i < 132) {
        m_axisData[i++] = tmp;
        m_axisData[i++] = -1.0 + paddingY;

        m_axisData[i++] = tmp;
        m_axisData[i++] = -1.0 + 1.5 * paddingY;

        tmp += (double)pow10(xExp) / scaleX;

        sprintf(buf, "%de%d", (int)ceil(xlow / pow10(xExp)), xExp);
        xlow += pow10(xExp);
        m_numberRenderer->addText(0.5 * (tmp + 1.0) * screenWidth, 0, 30, buf);
    }

    tmp = (floor(ylow / pow10(yExp)) * pow10(yExp) - centerY) / scaleY;

    while(tmp <= 1.0 && i < 256) {
        m_axisData[i++] = -1.0 + paddingX;
        m_axisData[i++] = tmp;

        m_axisData[i++] = -1.0 + 1.1 * paddingX;
        m_axisData[i++] = tmp;

        tmp += (double)pow10(yExp) / scaleY;

        sprintf(buf, "%de%d", (int)ceil(ylow / pow10(yExp)), yExp);
        ylow += pow10(yExp);
        m_numberRenderer->addText(0, 0.5 * (tmp + 1.0) * screenHeight, 30, buf);
    }

    while(i < 256) m_axisData[i++] = 0;

    return 0;
}

inline float GLPlot::m_pixelToScreen(int x, int screenSize) {
    return 2.0 * ((float)x / (float)screenSize) - 1.0;
}

inline float * GLPlot::m_pixelsToCoords(int x, int y, int screenWidth,
                                        int screenHeight) {
    float * screenCoords = (float *)malloc(2 * sizeof(float));

    screenCoords[0] = m_pixelToScreen(x, screenWidth);
    screenCoords[1] = m_pixelToScreen(y, screenHeight);

    return screenCoords;
}
