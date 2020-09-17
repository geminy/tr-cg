#version 300 es
// uniforms used by the vertex shader
uniform mat4 u_mvpMatrix; // matrix to convert P from model space to clip space
uniform sampler2D displacementMap;
// attribute inputs to the vertex shader
layout(location = 0) in vec4 a_position; // input position value
layout(location = 1) in vec3 a_normal; // input normal value
layout(location = 2) in vec2 a_texcoord; // input texcoord value
layout(location = 3) in vec4 a_color; // input color
// vertex shader output and input to the fragment shader
out vec4 v_color;
void main()
{
	v_color = a_color;
	float displacement = texture(displacementMap, a_texcoord).a;
	vec4 displaced_position = a_position + vec4(a_normal * diplacement, 0.0);
	gl_Position = u_mvpMatrix * displaced_position;
}