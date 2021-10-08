#shader vertex
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

#shader fragment
#version 330 core

layout(location=0) out vec4 o_color;

in vec4 v_color;
in vec2 v_texCoord;
uniform sampler2D u_Texture;

void main() {

    vec4 texColor = texture(u_Texture, v_texCoord );
    o_color = texColor * v_color ;
};
