#version 100
precision mediump float;

varying vec2 TexCoords;
uniform sampler2D diffuse;

void main()
{
	gl_FragColor = vec4(0.1,0.1,0.1,0.1) + texture2D(diffuse, TexCoords);
}
