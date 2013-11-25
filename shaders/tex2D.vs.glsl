#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexCoord;

out vec2 vFragTexCoord;

uniform mat3 uModelMatrix;

void main() {
	vFragTexCoord = aVertexTexCoord;
   gl_Position = vec4(uModelMatrix*vec3(aVertexPosition,1), 1);
  
};