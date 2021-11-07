#version 100
attribute vec3 vPos;

uniform mat4 projection;

varying vec3 Color;

void main()
{
	gl_Position = projection * vec4(vPos, 1.0);
	Color = vPos;
}
