#version 100

attribute vec3 aPos;

varying vec3 Color;

void main()
{
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
	Color = aPos;
}
