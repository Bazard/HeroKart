#pragma once
#include <GL/glew.h>

struct Vertex2DUV {
	GLfloat x, y;
	GLfloat u, v;

	Vertex2DUV(GLfloat x, GLfloat y, GLfloat u, GLfloat v) :
		x(x), y(y), u(u), v(v) {
	}
};