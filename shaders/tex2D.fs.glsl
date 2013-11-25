
#version 330 core

in vec2 vFragTexCoord;
out vec3 fFragColor;

uniform sampler2D uTexture;

vec2 multComplexe(vec2 c1, vec2 c2) {
	return vec2(c1.x*c2.x-c1.y*c2.y,c1.y*c2.x+c1.x*c2.y);
}

void main() {
	vec4 colortex=texture(uTexture, vFragTexCoord);
    fFragColor = colortex.xyz;
};
                    