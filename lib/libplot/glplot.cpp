// Robin Heinemann Copyright (C) 2015

#include "glplot.h"

glplot::glplot() {
    std::thread([=]() { create_window(); }).detach();
}

glplot::~glplot() { glFinish(); }

int glplot::plot(float * data, unsigned int count) {
    this->data    = data;
    this->count   = count;
    this->changed = true;

    return 0;
}

int glplot::create_window() {
    sf::RenderWindow * window = new sf::RenderWindow(
        sf::VideoMode{800, 600, sf::VideoMode::getDesktopMode().bitsPerPixel},
        APPLICATION_NAME, sf::Style::Default,
        sf::ContextSettings{24, 8, 0, 4, 3, sf::ContextSettings::Core});

    glewExperimental    = 1u;
    unsigned int status = glewInit();

    if(status != GLEW_OK) {
        std::cerr << "glewInit error: " << glewGetErrorString(status)
                  << std::endl;
        exit(-1);
    }

    std::cout << "GL_VERSION : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL_VENDOR : " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "GL_RENDERER : " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLEW_VERSION : " << glewGetString(GLEW_VERSION) << std::endl;
    std::cout << "GLSL_VERSION : " << glGetString(GL_SHADING_LANGUAGE_VERSION)
              << std::endl;

    if(!GLEW_VERSION_3_3) {
        std::cerr << "Atleast version 3.3 of OpenGL is needed for this program "
                     "to work";
        exit(-1);
    }

    sf::ContextSettings settings = window->getSettings();

    std::cout << "DEPTH BITS: " << settings.depthBits << std::endl;
    std::cout << "STENCIL BITS: " << settings.stencilBits << std::endl;
    std::cout << "ANTIALIASING LEVEL: " << settings.antialiasingLevel
              << std::endl;

    window->setVerticalSyncEnabled(true);

    glGenBuffers(1, &points.vbo);
    glGenBuffers(1, &axis.vbo);
    glGenBuffers(1, &axis_labels.vbo);

    glGenVertexArrays(1, &points.vao);
    glGenVertexArrays(1, &axis.vao);
    glGenVertexArrays(1, &axis_labels.vao);

    glBindBuffer(GL_ARRAY_BUFFER, points.vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * count * sizeof(float), data,
                 GL_STREAM_DRAW);
    glBindVertexArray(points.vao);

    shader = new class shader_program(
        "#version 150\nuniform vec2 center;\nuniform vec2 scale;\nin vec2 "
        "vp;\nvoid main() {\n   gl_Position = vec4((vp.x + center.x) / "
        "scale.x, (vp.y - center.y) / scale.y, 0.0, 1.0);\n}\n",
        "#version 150\nout vec4 frag_colour;\nvoid main () {\n    frag_colour "
        "= vec4(0.0, 0.0, 0.0, 1.0);\n}");

    shader->vertex_attrib_pointer("vp", 2, GL_FLOAT, false, 8, nullptr);

    // Hurts performance really hard:
    glDisable(GL_POINT_SMOOTH);

    glEnable(GL_LINE_SMOOTH);

    // Smaller pointsize makes rendering faster
    glPointSize(3.0);
    glLineWidth(3.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    nrenderer = new class number_renderer(800, 600);

    render_loop(window);

    return 0;
}

int glplot::render_loop(sf::RenderWindow * window) {
    sf::Event event{};
    bool      mouseDown = false, lines = false, scaleX = false, scaleY = false,
         axisNeedUpdate = true;
    int mouseX = 0, mouseY = 0, mouseDX = 0, mouseDY = 0,
        screenSizeX = window->getSize().x, screenSizeY = window->getSize().y;

    float scale[2] = {1.0, 1.0}, centerX = 0.0, centerY = 0.0,
          mouseDXScreen = 0.0, mouseDYScreen = 0.0;

    axis_data = (float *)malloc(256 * sizeof(float));

    for(int i = 0; i < 256; i++) { axis_data[i] = 0.0; }

    glClearColor(1, 1, 1, 1);

    nrenderer->set_screen_dimension(screenSizeX, screenSizeY);
    generate_axis_data(screenSizeX, screenSizeY, centerX, centerY, scale[0],
                       scale[1]);

    glBindBuffer(GL_ARRAY_BUFFER, axis.vbo);
    glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(float), axis_data,
                 GL_STREAM_DRAW);

    glBindVertexArray(axis.vao);
    shader->vertex_attrib_pointer("vp", 2, GL_FLOAT, false, 8, nullptr);

    while(window->isOpen()) {
        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window->close();
            } else if(event.type == sf::Event::Resized) {
                glViewport(0, 0, event.size.width, event.size.height);
                screenSizeX = event.size.width;
                screenSizeY = event.size.height;
                nrenderer->set_screen_dimension(screenSizeX, screenSizeY);
                axisNeedUpdate = true;
            } else if(event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    mouseDown = true;

                    mouseX = sf::Mouse::getPosition().x;
                    mouseY = sf::Mouse::getPosition().y;
                } else if(event.mouseButton.button == sf::Mouse::Right) {
                    lines = !lines;
                }
            } else if(event.type == sf::Event::MouseButtonReleased) {
                if(event.mouseButton.button == sf::Mouse::Left) {
                    mouseDown     = false;
                    centerX       = centerX - mouseDXScreen;
                    centerY       = centerY - mouseDYScreen;
                    mouseDXScreen = 0.0;
                    mouseDYScreen = 0.0;
                }
            } else if(event.type == sf::Event::MouseWheelScrolled) {
                if(scaleX && !scaleY) {
                    scale[0] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                } else if(scaleY && !scaleX) {
                    scale[1] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                } else {
                    scale[0] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                    scale[1] *=
                        (event.mouseWheelScroll.delta > 0 ? 0.9 : 1.11111111);
                }

                axisNeedUpdate = true;
            } else if(event.type == sf::Event::KeyPressed) {
                if(event.key.code == sf::Keyboard::LShift) {
                    scaleX = true;
                } else if(event.key.code == sf::Keyboard::LControl) {
                    scaleY = true;
                }
            } else if(event.type == sf::Event::KeyReleased) {
                if(event.key.code == sf::Keyboard::LShift) {
                    scaleX = false;
                } else if(event.key.code == sf::Keyboard::LControl) {
                    scaleY = false;
                }
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

        if(changed) {
            glBindBuffer(GL_ARRAY_BUFFER, points.vbo);
            glBufferData(GL_ARRAY_BUFFER, 2 * count * sizeof(float), data,
                         GL_STREAM_DRAW);

            changed = false;
        }

        if(axisNeedUpdate) {
            nrenderer->set_screen_dimension(screenSizeX, screenSizeY);
            generate_axis_data(screenSizeX, screenSizeY,
                               centerX - mouseDXScreen, centerY - mouseDYScreen,
                               scale[0], scale[1]);

            glBindBuffer(GL_ARRAY_BUFFER, axis.vbo);
            glBufferData(GL_ARRAY_BUFFER, 256 * sizeof(float), axis_data,
                         GL_STREAM_DRAW);

            axisNeedUpdate = false;
        }

        glBindBuffer(GL_ARRAY_BUFFER, points.vbo);
        shader->use_program();
        glBindVertexArray(points.vao);

        shader->uniform2f("center", centerX - mouseDXScreen,
                                  centerY - mouseDYScreen);
        shader->uniform2f("scale", scale[0], scale[1]);

        if(lines) {
            glDrawArrays(GL_LINE_STRIP, 0, count);
            glDrawArrays(GL_POINTS, 0, count);
        } else {
            glDrawArrays(GL_POINTS, 0, count);
        }

        glBindBuffer(GL_ARRAY_BUFFER, axis.vbo);
        shader->use_program();
        glBindVertexArray(axis.vao);

        shader->uniform2f("center", 0.0, 0.0);
        shader->uniform2f("scale", 1.0, 1.0);

        glDrawArrays(GL_LINES, 0, 128);

        nrenderer->render();

        window->display();
    }

    std::exit(EXIT_SUCCESS);

    return 1;
}

int glplot::generate_axis_data(int screenWidth, int screenHeight, float centerX,
                               float centerY, float scaleX, float scaleY) {
    centerX = -centerX;
    //    centerY = centerY;

    float xlow  = -1.0 * scaleX + centerX;
    float xhigh = 1.0 * scaleX + centerX;
    float ylow  = -1.0 * scaleY + centerY;
    float yhigh = 1.0 * scaleY + centerY;

    float paddingX = 150.0 / (float)screenWidth;
    float paddingY = 30.0 / (float)screenHeight;

    int xExp = axis_scale::get_optimal_exponent(xlow, xhigh);
    int yExp = axis_scale::get_optimal_exponent(ylow, yhigh);

    nrenderer->clear_text();

    // Base Axis:
    axis_data[0] = -1.0 + paddingX;
    axis_data[1] = 1.0;

    axis_data[2] = -1.0 + paddingX;
    axis_data[3] = -1.0;

    axis_data[4] = -1.0;
    axis_data[5] = -1.0 + paddingY;

    axis_data[6] = 1.0;
    axis_data[7] = -1.0 + paddingY;

    int   i = 8, k = 1;
    float tmp = (floor(xlow / pow10(xExp)) * pow10(xExp) - centerX) / scaleX;
    char  buf[128];

    while(tmp <= 1.0 && i < 132) {
        axis_data[i++] = tmp;
        axis_data[i++] = -1.0 + paddingY;

        axis_data[i++] = tmp;
        axis_data[i++] = -1.0 + 1.5 * paddingY;

        tmp += (double)pow10(xExp) / scaleX;

        sprintf(buf, "%de%d", (int)ceil(xlow / pow10(xExp)), xExp);
        xlow += pow10(xExp);
        nrenderer->add_text(0.5 * (tmp + 1.0) * screenWidth, 0, 30, buf);
    }

    tmp = (floor(ylow / pow10(yExp)) * pow10(yExp) - centerY) / scaleY;

    while(tmp <= 1.0 && i < 256) {
        axis_data[i++] = -1.0 + paddingX;
        axis_data[i++] = tmp;

        axis_data[i++] = -1.0 + 1.1 * paddingX;
        axis_data[i++] = tmp;

        tmp += (double)pow10(yExp) / scaleY;

        sprintf(buf, "%de%d", (int)ceil(ylow / pow10(yExp)), yExp);
        ylow += pow10(yExp);
        nrenderer->add_text(0, 0.5 * (tmp + 1.0) * screenHeight, 30, buf);
    }

    while(i < 256) { axis_data[i++] = 0; }

    return 0;
}

inline float glplot::pixel_to_screen(int x, int screenSize) {
    return 2.0 * ((float)x / (float)screenSize) - 1.0;
}

inline float * glplot::pixels_to_coords(int x, int y, int screenWidth,
                                        int screenHeight) {
    float * screenCoords = (float *)malloc(2 * sizeof(float));

    screenCoords[0] = pixel_to_screen(x, screenWidth);
    screenCoords[1] = pixel_to_screen(y, screenHeight);

    return screenCoords;
}
