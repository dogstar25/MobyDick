#pragma once

namespace basicShader
{
    const char* vertextShader = R"(
#version 450 core

layout(location=0) in vec3 i_position;
layout(location=1) in vec4 i_color;
layout(location=2) in vec2 i_texCoord;
layout(location=3) in float i_texIndex;

uniform mat4 u_projection_matrix;
out vec4 v_color;
out vec2 v_texCoord;
out float v_texIndex;

void main() {
    v_color = i_color;
    v_texCoord = i_texCoord;
    v_texIndex = i_texIndex;
    gl_Position = u_projection_matrix * vec4( i_position, 1.0 );
};

)";

    const char* fragmentShader = R"(
#version 450 core

layout(location=0) out vec4 o_color;

in vec4 v_color;
in vec2 v_texCoord;
in float v_texIndex;
uniform sampler2D u_Texture;


void main() {
    vec4 texColor;

    texColor = texture(u_Texture, v_texCoord );

    o_color = texColor * v_color ;

};

)";

}
