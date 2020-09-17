#version 300 es
// matrix to convert a_position from model space to normalized device space
uniform mat4 u_mvpMatrix;
// attributes input to the vertex shader
in vec4 a_position; // position value
in vec4 a_color; // input vertex color
// output of the vertex shader and input to fragment shader
out vec4 v_color; // output vertex color
void main()
{
	v_color = a_color;
	gl_position = u_mvpMatrix * a_position;
}