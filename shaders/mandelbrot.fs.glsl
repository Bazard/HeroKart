
#version 330 core

in vec3 vFragColor;
in vec3 vFragPosition;
out vec3 fFragColor;

vec2 multComplexe(vec2 c1, vec2 c2) {
	return vec2(c1.x*c2.x-c1.y*c2.y,c1.y*c2.x+c1.x*c2.y);
}

vec3 Mandelbrot(vec3 position){
	float N=30;
	vec2 c=vec2(position.x,position.y);
	vec2 z=c;
	
	for(int i=0;i<N;++i){
		if(length(z)>=2)
			return vec3(i/N);
		z+=multComplexe(z,z)+c;
	}
	
	return vec3(N);
}

float particule(vec3 P){
	float distance=smoothstep(0.4, 0.5, max(abs(fract(8.0 * P.x - 0.5 * mod(floor(8.0 * P.y), 2.0)) - 0.5), abs(fract(8.0 * P.y) - 0.5)));
	float alpha=5;
	float beta=4;
	float res=alpha*exp(-beta*distance*distance);
	return res;
}

void main() {
	vec3 mandel=Mandelbrot(vFragPosition);
    fFragColor = mandel;
};
                    