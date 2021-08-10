/*
    Minimal SDL2 + OpenGL3 example.
    Author: https://github.com/koute
    This file is in the public domain; you can do whatever you want with it.
    In case the concept of public domain doesn't exist in your jurisdiction
    you can also use this code under the terms of Creative Commons CC0 license,
    either version 1.0 or (at your option) any later version; for details see:
        http://creativecommons.org/publicdomain/zero/1.0/
    This software is distributed without any warranty whatsoever.
    Compile and run with: gcc opengl3_hello.c `sdl2-config --libs --cflags` -lGL -Wall && ./a.out
*/

#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>


#include <glad/glad.h>
#include <SDL2/SDL_opengl.h>

#include <memory>
#include "Game.h"
#include <stdio.h>

std::unique_ptr<Game> game;

typedef float t_mat4x4[16];

static inline void mat4x4_ortho(t_mat4x4 out, float left, float right, float bottom, float top, float znear, float zfar)
{
#define T(a, b) (a * 4 + b)

    out[T(0, 0)] = 2.0f / (right - left);
    out[T(0, 1)] = 0.0f;
    out[T(0, 2)] = 0.0f;
    out[T(0, 3)] = 0.0f;

    out[T(1, 1)] = 2.0f / (top - bottom);
    out[T(1, 0)] = 0.0f;
    out[T(1, 2)] = 0.0f;
    out[T(1, 3)] = 0.0f;

    out[T(2, 2)] = -2.0f / (zfar - znear);
    out[T(2, 0)] = 0.0f;
    out[T(2, 1)] = 0.0f;
    out[T(2, 3)] = 0.0f;

    out[T(3, 0)] = -(right + left) / (right - left);
    out[T(3, 1)] = -(top + bottom) / (top - bottom);
    out[T(3, 2)] = -(zfar + znear) / (zfar - znear);
    out[T(3, 3)] = 1.0f;

#undef T
}

static const char* vertex_shader =
"#version 130\n"
"in vec2 i_position;\n"
"in vec4 i_color;\n"
"out vec4 v_color;\n"
"uniform mat4 u_projection_matrix;\n"
"void main() {\n"
"    v_color = i_color;\n"
"    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );\n"
"}\n";

static const char* fragment_shader =
"#version 130\n"
"in vec4 v_color;\n"
"out vec4 o_color;\n"
"void main() {\n"
"    o_color = v_color;\n"
"}\n";

typedef enum t_attrib_id
{
    attrib_position,
    attrib_color
} t_attrib_id;

int main(int argc, char* argv[])
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return -1;
    }

    //if (SDL_GL_LoadLibrary(NULL) != 0)
    //{
    //    std::cout << "Failed to initialize _GL_LoadLibrary" << std::endl;
    //    return -1;

    //}


    //SDL_Init(SDL_INIT_VIDEO);
    /*SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);*/

    static const int width = 800;
    static const int height = 600;


    SDL_Window* window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext context = SDL_GL_CreateContext(window);

    GLuint vs, fs, program;

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    //
    //Begin creating and compiling shaders
    //
    vs = glCreateShader(GL_VERTEX_SHADER);
    fs = glCreateShader(GL_FRAGMENT_SHADER);

    int length = strlen(vertex_shader);
    glShaderSource(vs, 1, (const GLchar**)&vertex_shader, &length);
    glCompileShader(vs);

    GLint status;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        fprintf(stderr, "vertex shader compilation failed\n");
        return 1;
    }

    length = strlen(fragment_shader);
    glShaderSource(fs, 1, (const GLchar**)&fragment_shader, &length);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        fprintf(stderr, "fragment shader compilation failed\n");
        return 1;
    }

    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glBindAttribLocation(program, attrib_position, "i_position");
    glBindAttribLocation(program, attrib_color, "i_color");
    glLinkProgram(program);

    glUseProgram(program);

    //Clean up
    glDeleteShader(vs);
    glDeleteShader(fs);

    //
    //END create shaders
    //


    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glViewport(0, 0, width, height);

   

    ///
    //Vertex Buffer Input
    // 
    //Create a vertext Buffer Array
    const GLfloat g_vertex_buffer_data[] = {
        /*  R, G, B, A, X, Y  */
            1, 0, 0, 1, 0, 0,
            0, 1, 0, 1, width, 0,
            0, 0, 1, 1, width, height,

            1, 0, 0, 1, 0, 0,
            0, 0, 1, 1, width, height,
            1, 1, 1, 1, 0, height
    };

    // Generate  a vertext buffer object with a unique id, i.e. 1
    GLuint vbo;
    glGenBuffers(1, &vbo);
    //Bind the vertext buffer array to the GL_ARRAY_BUFFER
    //we can only bind ONE vertex buffer objewct (vbo) to the GL_ARRAY_BUFFER buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo); //state setting
    //We are now bound to the GL_ARRAY_BUFFER buffer
    //Copy the vertext data into the buffer bound to the GL_ARRAY_BUFFER, which is vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // state using
    //
    //Vertex Buffer complete
    //

    ///
    //Vertex Array Input
    // 

    GLuint vao;

    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);


    glEnableVertexAttribArray(attrib_position);
    glEnableVertexAttribArray(attrib_color);

    glVertexAttribPointer(attrib_position, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(4 * sizeof(float)));
    glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
    

    //
    // Projection Matrix
    //
    t_mat4x4 projection_matrix;
    mat4x4_ortho(projection_matrix, 0.0f, (float)width, (float)height, 0.0f, 0.0f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(program, "u_projection_matrix"), 1, GL_FALSE, projection_matrix);

    for (;; )
    {
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYUP:
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        return 0;
                    break;
            }
        }

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}