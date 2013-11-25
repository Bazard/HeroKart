#pragma once

namespace glimac {

	struct Vertex2DUV  {
	public:
		GLfloat x, y;
		GLfloat u, v;

		Vertex2DUV (): x(0), y(0), u(0), v(0){};
		Vertex2DUV (GLfloat x, GLfloat y, GLfloat u, GLfloat v):
			x(x), y(y), u(u), v(v) {
		}
	};

}