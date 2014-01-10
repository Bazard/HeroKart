
#version 330 core

in vec3 vFragPosition;
in vec3 vFragNormal;
in vec2 vFragTexCoords;
out vec3 fFragColor;

uniform vec3 uKd;
uniform vec3 uKs;
uniform float uShininess;

uniform vec3 uLightDir_vs;
uniform vec3 uLightIntensity;

vec3 blinnPhong(){
	vec3 halfVector=vec3((normalize(-vFragPosition)+uLightDir_vs)/2);
	return vec3(uLightIntensity*(uKd*(dot(uLightDir_vs,vFragNormal))+uKs*pow(dot(halfVector,vFragNormal),uShininess)));
}

uniform sampler2D uTexture;

void main() {
	vec4 colortex=texture(uTexture, vFragTexCoords);
    fFragColor = colortex.xyz;//blinnPhong()*colortex.xyz;
}
                    