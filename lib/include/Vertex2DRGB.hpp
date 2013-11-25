#pragma once

namespace glimac {

	struct Vertex2DRGB {
	public:
		GLfloat x, y;
		GLfloat r, g, b;

		Vertex2DRGB(): x(0), y(0), r(0), g(0), b(0){};
		Vertex2DRGB(GLfloat x, GLfloat y, GLfloat r, GLfloat g, GLfloat b):
			x(x), y(y), r(r), g(g), b(b) {
		}
	};

}