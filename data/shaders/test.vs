#version 100
attribute vec3 vPos;

varying vec3 Color;

void main()
{
	gl_Position = vec4(vPos, 1.0);
	Color = vPos;
}
