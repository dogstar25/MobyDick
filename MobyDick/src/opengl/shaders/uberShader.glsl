#shader vertex
#version 330 core

layout(location=0) in vec3 i_position;
layout(location=1) in vec4 i_color;
layout(location=2) in vec2 i_texCoord;

uniform mat4 u_projection_matrix;
out vec4 v_color;
out vec2 v_texCoord;


void vertexShader_basicTexture( vec3 i_position)
{

    //calculate the angle
    //vec3 angle = (90,0,0);
    //vec3 rAngle = radians(angle);
    float rAngle = 90;
    rAngle = radians(rAngle);
    float cosAngle	=	cos(rAngle);	
	float sinAngle	=	sin(rAngle);
//    mat4 rotateX = mat4( 0.0, c.x, s.x, 0.0,
//                         1.0, 0.0, 0.0, 0.0,
//                         0.0, -s.x, c.x, 0.0,
//                         0.0, 0.0, 0.0, 1.0);
//
//    mat4 rotateY = mat4( c.y, 0.0, -s.y, 0.0,
//                         0.0, 1.0, 0.0, 0.0,
//                         s.y, 0.0, c.y, 0.0,
//                         0.0, 0.0, 0.0, 1.0);
//
    
//    vec2 rotated = vec2(cosAngle * (i_position.x) + sinAngle * (i_position.y),
//                        cosAngle * (i_position.y) - sinAngle * (i_position.x));

//    vec2 rotated = vec2(cosAngle * (i_position.x + mid) + sinAngle * (i_position.y + mid) + mid,
//                        cosAngle * (i_position.y + mid) - sinAngle * (i_position.x + mid) + mid);
//
    //vec4 center_position = u_projection_matrix * vec4( i_position.x + (i_position.x/2),i_position.y + (i_position.y/2) , 0, 1.0 );
    //float rAngle = 45;
    //center_position.x = (center_position.x * cos(rAngle)) - (center_position.y * sin(rAngle));
    //center_position.y = (center_position.x * sin(rAngle)) + (center_position.y * cos(rAngle));


    //gl_Position = u_projection_matrix * vec4(rotated, 0.0, 1.0) *  vec4( i_position, 1.0);
    gl_Position = u_projection_matrix * vec4( i_position, 1.0);
};


void main() {
    
    vertexShader_basicTexture(i_position);

    //Pass through to frag shader variables
    v_color = i_color;
    v_texCoord = i_texCoord;


};



#shader fragment
#version 330 core

layout(location=0) out vec4 o_color;

in vec4 v_color;
in vec2 v_texCoord;
uniform sampler2D u_Texture;

vec4 fragShader_basicTexture( sampler2D u_Texture, vec4 v_color, vec2 v_texCoord)
{
    vec4 texColor = texture(u_Texture, v_texCoord );
    vec4 color = texColor * v_color ;
    return color;
    
}

void main() {

    o_color = fragShader_basicTexture(u_Texture, v_color, v_texCoord);

};

