#version 100
attribute vec3 vPos; // vertex
attribute vec3 nPos; // normals
attribute vec2 tPos; // texture coordinates

uniform mat4 VP_matrix;
uniform mat4 model_matrix;

varying vec3 Color;

void main()
{
	gl_Position = VP_matrix * model_matrix * vec4(vPos, 1.0);
	Color = vPos;
}
