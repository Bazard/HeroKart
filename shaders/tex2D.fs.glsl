#version 330 core

in vec2 vFragTexture;

out vec3 fFragColor;

uniform sampler2D uTexture;

void main() {
	vec4 truc=texture(uTexture, vFragTexture);
	fFragColor=truc.xyz;
}