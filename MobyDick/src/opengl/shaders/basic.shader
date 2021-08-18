#vertex Shader
#version 130
layout(location = 0) in vec2 i_position;
layout(location = 1) in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
void main() {
    v_color = i_color;
    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );
};

#fragment shader
#version 130
in vec4 v_color;
out vec4 o_color;
void main() {
    o_color = v_color;
};
