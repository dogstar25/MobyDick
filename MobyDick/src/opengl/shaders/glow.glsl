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

const float blurSize = 1.0/1024.0;
const float intensity = 0.35;

layout(location=0) out vec4 o_color;
in vec4 v_color;
in vec2 v_texCoord;
uniform sampler2D u_Texture;

void main()
{
   vec4 sum = vec4(0);
   int j;
   int i;

   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture(u_Texture, vec2(v_texCoord.x - 4.0*blurSize, v_texCoord.y)) * 0.05;
   sum += texture(u_Texture, vec2(v_texCoord.x - 3.0*blurSize, v_texCoord.y)) * 0.09;
   sum += texture(u_Texture, vec2(v_texCoord.x - 2.0*blurSize, v_texCoord.y)) * 0.12;
   sum += texture(u_Texture, vec2(v_texCoord.x - blurSize, v_texCoord.y)) * 0.15;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y)) * 0.16;
   sum += texture(u_Texture, vec2(v_texCoord.x + blurSize, v_texCoord.y)) * 0.15;
   sum += texture(u_Texture, vec2(v_texCoord.x + 2.0*blurSize, v_texCoord.y)) * 0.12;
   sum += texture(u_Texture, vec2(v_texCoord.x + 3.0*blurSize, v_texCoord.y)) * 0.09;
   sum += texture(u_Texture, vec2(v_texCoord.x + 4.0*blurSize, v_texCoord.y)) * 0.05;
	
    // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y - 4.0*blurSize)) * 0.05;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y - 3.0*blurSize)) * 0.09;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y - 2.0*blurSize)) * 0.12;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y - blurSize)) * 0.15;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y)) * 0.16;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y + blurSize)) * 0.15;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y + 2.0*blurSize)) * 0.12;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y + 3.0*blurSize)) * 0.09;
   sum += texture(u_Texture, vec2(v_texCoord.x, v_texCoord.y + 4.0*blurSize)) * 0.05;

   //increase blur with intensity!
   //fragColor = sum*intensity + texture(iChannel0, texcoord); 
   o_color = sum * v_color * texture(u_Texture, v_texCoord);
 
 }