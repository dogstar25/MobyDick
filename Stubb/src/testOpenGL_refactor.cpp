#pragma once

#define GL_GLEXT_PROTOTYPES


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include <SDL2/SDL.h>




#include <glad/glad.h>

#include <SDL2/SDL_opengl.h>

#include <memory>
#include "Game.h"
#include "TextureManager.h"
#include "Renderer.h"
#include <stdio.h>

#include "opengl/Shader.h"
#include "opengl/VertexBuffer.h"
#include "opengl/IndexBuffer.h"

std::unique_ptr<Game> game;


int main(int argc, char* argv[])
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        return -1;
    }

    
    static const int width = 800;
    static const int height = 600;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // vertex attrubute indexes
    const int attrib_position = 0;
    const int attrib_color = 1;
    const int attrib_texture = 2;

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

    //Initialize the Renderer
    Renderer::instance().init(window);

    //Initialize the texture manager
    TextureManager::instance().init();
    TextureManager::instance().load("textureAssets");

    //Shader
    Shader basicShader("BASIC");

    //Create the shader program
    program = glCreateProgram();

    //Attach my built and ready shader program
    glAttachShader(program, basicShader.vertexshaderId());
    glAttachShader(program, basicShader.fragmentshaderId());

    //shader uniforms
    glBindAttribLocation(program, attrib_position, "i_position");
    glBindAttribLocation(program, attrib_color, "i_color");
    glBindAttribLocation(program, attrib_texture, "i_texCoord");

    //Link and use the program
    glLinkProgram(program);
    glUseProgram(program);

    //Projection matrix
    glm::mat4 projection_matrix;
    projection_matrix = glm::ortho( 0.0f, (float)width, (float)height, 0.0f);
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
    //glm::mat4 mvp = projection_matrix * view;

    GLuint matrixId = glGetUniformLocation(program, "u_projection_matrix");
    glUniformMatrix4fv(matrixId, 1, false, (float*)&projection_matrix[0][0]);

    //Clean up
    basicShader.cleanup();

    //END create shaders


    //glDisable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    //glViewport(0, 0, width, height);

   

    ///
    //Vertex Buffer Input
    // 
    //Create a vertext Buffer Array
    //Example of a 25 pixel object placed at location 50,50
    const GLfloat g_vertex_buffer_data[] = {
        /*  R, G, B, A, X,  Y   texx texy */
            1, 0, 0, 1, 50, 50, 0.0, 0.0, // vertex 0
            1, 0, 0, 1, 75, 50, 1.0, 0.0, // vertex 1
            1, 0, 0, 1, 75, 75, 1.0, 1.0, // vertex 2
            1, 0, 0, 1, 50, 75, 0.0, 1.0 // vertex 3
    };

    VertexBuffer vertexBuffer(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(attrib_position, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(4 * sizeof(float)));
    glVertexAttribPointer(attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
    glVertexAttribPointer(attrib_texture, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));

    //index buffer
    const GLuint g_index_buffer[] = {
        0,1,2,
        2,3,0
    };

    IndexBuffer indexBuffer(g_index_buffer, sizeof(g_index_buffer));
    

    //////////////////////////////////////////////////////////////////////////////////////
    //Texture stuff
    //
    static GLuint texture_id;
    SDL_Surface* surf = TextureManager::instance().getTexture("TX_STUBB")->surface;
    //Generate an array of textures.  We only want one texture (one element array), so trick
    //it by treating "texture" as array of length one.
    glGenTextures(1, &texture_id);
    //Select (bind) the texture we just generated as the current 2D texture OpenGL is using/modifying.
    //All subsequent changes to OpenGL's texturing state for 2D textures will affect this texture.
    //glActiveTexture(GL_TEXTURE0, texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    
    //Set the minification and magnification filters.  In this case, when the texture is minified (i.e., the texture's pixels (texels) are
    //*smaller* than the screen pixels you're seeing them on, linearly filter them (i.e. blend them together).  This blends four texels for
    //each sample--which is not very much.  Mipmapping can give better results.  Find a texturing tutorial that discusses these issues
    //further.  Conversely, when the texture is magnified (i.e., the texture's texels are *larger* than the screen pixels you're seeing
    //them on), linearly filter them.  Qualitatively, this causes "blown up" (overmagnified) textures to look blurry instead of blocky.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    //load in the image data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surf->pixels);
    //Unbind
    //glBindTexture(GL_TEXTURE_2D, 0);
    //Unload SDL's copy of the data; we don't need it anymore because OpenGL now stores it in the texture.
    //SDL_FreeSurface(surf);
    GLuint textUniformId = glGetUniformLocation(program, "u_Texture");
    glUniform1i(textUniformId, 0);
    //glBindTexture(GL_TEXTURE_2D, 0);

    //Texture uniform


    //////////////////////////////////
  
    //unbind all - to replicate later what things need binding for each draw
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    vertexBuffer.unbind();
    indexBuffer.unbind();
    
    

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

        //Set the shader
        glUseProgram(program);

        //Set the projection matrix
        GLuint matrixId = glGetUniformLocation(program, "u_projection_matrix");
        glUniformMatrix4fv(matrixId, 1, false, (float*)&projection_matrix);

        //Bind the vertex array
        glBindVertexArray(vao);

        //Bind the texture
        glBindTexture(GL_TEXTURE_2D, texture_id);

        //Bind the indexBuffer
        indexBuffer.bind();

        //Draw
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
        SDL_Delay(1);
    }
    glDeleteTextures(1, &texture_id);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}