#version 100
attribute vec3 position;

uniform mat4 view_matrix;
uniform mat4 model_matrix;

varying vec3 Color;

void main()
{
	gl_Position = view_matrix * model_matrix * vec4(position, 1.0);
	Color = position;
}
