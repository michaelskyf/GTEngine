#version 100
attribute vec3 vPos; // vertex
attribute vec3 nPos; // normals
attribute vec2 tPos; // texture coordinates

uniform mat4 VP_matrix;
uniform mat4 model_matrix;

varying vec3 Color;

void main()
{
	//gl_Position = projection * vec4(vPos, 1.0);
	gl_Position = model_matrix * vec4(vPos.x, vPos.y, vPos.z - 5.0, 1.0);
	Color = vPos;
}
