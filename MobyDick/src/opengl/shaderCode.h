#pragma once

namespace basicShader
{
    const char* vertextShader = R"(
#version 330 core

layout(location=0) in vec3 i_position;
layout(location=1) in vec4 i_color;
layout(location=2) in vec2 i_texCoord;

uniform mat4 u_projection_matrix;
out vec4 v_color;
out vec2 v_texCoord;

void main() {
    v_color = i_color;
    v_texCoord = i_texCoord;
    gl_Position = u_projection_matrix * vec4( i_position, 1.0 );
};

)";

    const char* fragmentShader = R"(
#version 330 core

layout(location=0) out vec4 o_color;

in vec4 v_color;
in vec2 v_texCoord;
uniform sampler2D u_Texture;
uniform sampler2D u_dynamicTexture;
uniform int u_isDynamicTexture;
uniform int u_isNoTexture;


void main() {
    vec4 texColor;

    if(u_isNoTexture == 1)
    {
        texColor = v_color;
    }
    else if(u_isDynamicTexture == 0)
    {
        texColor = texture(u_Texture, v_texCoord );

    }
    else
    {
        texColor = texture(u_dynamicTexture, v_texCoord );

    }

    o_color = texColor * v_color ;

};

)";

}
