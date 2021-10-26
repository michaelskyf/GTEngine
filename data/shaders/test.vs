#version 100

attribute vec3 position;

varying vec3 Color;

void main()
{
	gl_Position = vec4(position.x, position.y, position.z, 1.0);
	Color = position;
}
