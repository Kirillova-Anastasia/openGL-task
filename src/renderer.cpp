#include "renderer.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

int rect_number = 16;

int add_shader(std::string fileName, GLuint shaderProgram, GLenum shaderType)
{
    char* src; int32_t size;

    //Читаем файл с кодом шейдера
    std::ifstream t;
    std::string fileContent;

    t.open(fileName);
    if(t.is_open())
    {
        std::stringstream buffer;
        buffer << t.rdbuf();
        fileContent = buffer.str();
    }
    else std::cout<<"File "<<fileName<<" opening failed"<<std::endl;
    t.close();
    size = fileContent.length();  //Set the Size

    std::string result = fileContent;
    src = const_cast<char*>(result.c_str());

    int shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &src, &size);
    
    //компилируем шейдер
    glCompileShader(shaderID);
    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if(success) 
    {
        glAttachShader(shaderProgram, shaderID);
    }
    else 
    {
        //печатаем информацию об ошибках компиляции
        int m;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &m);
        char* l = new char[m];
        if(t) glGetShaderInfoLog(shaderID, m, &m, l);
        std::cout<<"Compiler Error: "<<l<<std::endl;
        delete[] l;
    }
    return shaderID;
}
void add_shader_program(GLuint &shaderProgram, const std::string vs, const std::string fs)
{
    //компилируем шейдеры и связываем их с программой
    shaderProgram = glCreateProgram(); 
    int vs_code = add_shader(vs,shaderProgram, GL_VERTEX_SHADER);
    int fs_code = add_shader(fs,shaderProgram, GL_FRAGMENT_SHADER);
    glLinkProgram(shaderProgram);

    //печатаем ошибки линковки
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        int m;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &m);
        char* l = new char[m];
        glGetProgramInfoLog(shaderProgram, m, &m, l);
        std::cout<<"Linker Error: "<<l<<std::endl;
        delete[] l;
    }
    glDeleteShader(vs_code);
    glDeleteShader(fs_code);
}
void Renderer::Init(SDL_Window *_window, int w, int h)
{
    window = _window;
    width = w;
    height = h;

    GLfloat scale = 0.1f;
    GLfloat flag_width = 4*scale,flag_height = 3*scale;
    GLfloat shift = flag_width / rect_number, tex_shift = 1.0f / rect_number;
    GLfloat vertices[(rect_number + 1) * 10];
    for (int i = 0; i < rect_number + 1; ++i) {
        //верхняя точка
        vertices[i*10] = -flag_width/2 + i*shift;
        vertices[i*10+1] = flag_height/2;
        vertices[i*10+2] = 0.0f;
        vertices[i*10+3] = 1.0f - i*tex_shift;
        vertices[i*10+4] = 0.0f;

        //нижняя точка
        vertices[i*10+5] = -flag_width/2 + i*shift;
        vertices[i*10+6] = -flag_height/2;
        vertices[i*10+7] = 0.0f;
        vertices[i*10+8] = 1.0f - i*tex_shift;
        vertices[i*10+9] = 1.0f;
    }

    GLuint indices[rect_number * 6];
    for (int i = 0; i < rect_number; ++i) {
        indices[i*6] = i*2;
        indices[i*6+1] = i*2+2;
        indices[i*6+2] = i*2+3;
        indices[i*6+3] = i*2+3;
        indices[i*6+4] = i*2+1;
        indices[i*6+5] = i*2;
    }

// 1. Создаем буферы
    glGenBuffers(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
// 2. Копируем наши вершины в буфер для OpenGL
glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 3. Копируем наши индексы в в буфер для OpenGL
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 3. Устанавливаем указатели на вершинные атрибуты
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
glEnableVertexAttribArray(0);   
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
glEnableVertexAttribArray(1);  
// 4. Отвязываем VAO (НЕ EBO)
glBindVertexArray(0);

add_shader_program(shaderProgram,"shaders/simple.vert","shaders/simple.frag");
}
void Renderer::Render(unsigned char *data, int width, int height)
{
    unsigned int texture;
    // Загрузка и создание текстуры
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // все последующие GL_TEXTURE_2D-операции теперь будут влиять на данный текстурный объект
    
    // Установка параметров наложения текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // установка метода наложения текстуры GL_REPEAT (стандартный метод наложения)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Установка параметров фильтрации текстуры
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Загрузка изображения, создание текстуры и генерирование мипмап-уровней
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glUseProgram(shaderProgram);
    //glUniform3f(glGetUniformLocation(shaderProgram, "color"),0,1,0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, rect_number*6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
void Renderer::Close()
{
    glDeleteBuffers(1,&VAO);
    glDeleteBuffers(1,&VBO);
    glDeleteBuffers(1,&EBO);
    glDeleteProgram(shaderProgram);
}