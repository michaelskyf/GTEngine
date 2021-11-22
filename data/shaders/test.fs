#version 100
precision mediump float;

varying vec2 TexCoords;
uniform sampler2D diffuse;

void main()
{
	gl_FragColor = texture2D(diffuse, TexCoords);
}
