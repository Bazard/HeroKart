#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragPosition;
out vec3 vFragColor;

mat3 translate(float tx, float ty){
	mat3 mat=mat3(vec3(1,0,0),vec3(0,1,0),vec3(tx,ty,1));
	return mat;
}

mat3 scale(float sx, float sy){
	mat3 mat=mat3(vec3(sx,0,0),vec3(0,sy,0),vec3(0,0,1));
	return mat;
}

mat3 rotate(float a){
	mat3 mat=mat3(vec3(cos(a),sin(a),0),vec3(-sin(a),cos(a),0),vec3(0,0,1));
	return mat;	
}

void main() {
   vFragColor = aVertexColor;
   gl_Position = vec4(aVertexPosition, 1);
   vFragPosition = translate(1,0)*aVertexPosition;
};