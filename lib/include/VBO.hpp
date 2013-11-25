#pragma once
#include <GL/glew.h>

namespace glimac {

	struct VBO {
		private:
			GLuint m_nGLId;

		public:

			VBO(){				
				glGenBuffers(1,&m_nGLId);
			}
			VBO(VBO&& rvalue): m_nGLId(rvalue.m_nGLId) {
				rvalue.m_nGLId = 0;
			}

			VBO& operator =(VBO&& rvalue) {
				m_nGLId = rvalue.m_nGLId;
				rvalue.m_nGLId = 0;
			}
			~VBO(){
				glDeleteBuffers(1,&m_nGLId);
			}
			GLuint getGLId() const{
				return m_nGLId;
			}

			void bind(GLenum target = GL_ARRAY_BUFFER) const{
				glBindBuffer(target,m_nGLId);
			}

			static void debind(GLenum target = GL_ARRAY_BUFFER){
				glBindBuffer(target,0);
			}
	};
}