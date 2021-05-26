#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <SDL2/SDL.h>

class Renderer
{
public:
    void Init(SDL_Window *window, int w, int h);
    void Render(unsigned char *data, int width, int height, GLfloat time);
    void Close();
private:
    GLuint VAO, VBO, EBO;
    GLuint shaderProgram;
    SDL_Window *window;
    int width, height;
    float flag_width, flag_height;
    GLfloat translations[200];
};