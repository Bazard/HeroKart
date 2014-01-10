
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

vec3 blinnPhong() {
	vec3 wi = normalize(uLightDir_vs);
	vec3 N = normalize(vFragNormal);
	vec3 wo = normalize(-vFragPosition);

	vec3 halfVector = 0.5f * (wi + wo);

	return uLightIntensity * (uKd * dot(wi, N) + uKs * pow(dot(halfVector, N), uShininess));
}

uniform sampler2D uTexture;

void main() {
	vec4 colortex=texture(uTexture, vFragTexCoords);
	vec3 blinn=blinnPhong();
	if(blinn.x<0.4) blinn.x=1;
	if(blinn.y<0.4) blinn.y=1;
	if(blinn.z<0.4) blinn.z=1;
		
    fFragColor = colortex.xyz*blinn;
}
                    