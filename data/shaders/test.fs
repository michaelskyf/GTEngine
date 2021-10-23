#version 100
precision mediump float;

varying vec3 Color;

void main()
{
	gl_FragColor = vec4(Color.x, Color.y, Color.z, 1.0);
}
