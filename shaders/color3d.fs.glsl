
#version 330 core

in vec3 vFragColor;
in vec3 vFragPosition;
out vec3 fFragColor;

float particule(vec3 P){
	float distance=smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5)));
	float alpha=5;
	float beta=4;
	float res=alpha*exp(-beta*distance*distance);
	return res;
}

void main() {
	float att=1;//particule(vFragPosition);
    fFragColor = att*vFragColor;
};
                    