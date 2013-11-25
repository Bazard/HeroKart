#pragma once

namespace glimac {

	struct Vertex3DRGB {
	public:
		GLfloat x, y, z;
		GLfloat r, g, b;

		Vertex3DRGB(): x(0), y(0), z(0), r(0), g(0), b(0){};
		Vertex3DRGB(GLfloat x, GLfloat y, GLfloat z, GLfloat r, GLfloat g, GLfloat b):
			x(x), y(y), z(z), r(r), g(g), b(b) {
		}
	};

}