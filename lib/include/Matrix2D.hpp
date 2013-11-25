#pragma once

namespace glimac {

	struct Matrix2D  {
	public:
		GLfloat values[9];

		Matrix2D (){
			for(int i=0;i<9;++i) values[i]=0;
		};
		
		Matrix2D (GLfloat a){
			for(int i=0;i<9;++i) values[i]=a;
		};
		
	void translate(float tx, float ty){
			values[0]=1;
			values[4]=1;
			values[6]=tx;
			values[7]=ty;
			values[8]=1;

	}

	void scale(float sx, float sy){
			values[0]=sx;
			values[4]=sy;
			values[8]=1;
	}

	void rotate(float a){
			values[0]=cos(a);
			values[1]=sin(a);
			values[3]=-sin(a);
			values[4]=cos(a);
			values[8]=1;
	}

	};
	
	Matrix2D operator*(const Matrix2D& lhs, const Matrix2D& rhs) {
		Matrix2D result;
		for(int j = 0; j < 3; ++j) {
			for(int i = 0; i < 3; ++i) {
				int idx = i + j * 3;
				result.values[idx] = 0.f;
				for(int k = 0; k < 3; ++k) {
					result.values[idx] += lhs.values[i + k * 3] * rhs.values[k + j * 3];
				}
			}
		}
		return result;
	}

}