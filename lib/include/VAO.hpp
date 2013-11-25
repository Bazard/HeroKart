#pragma once
#include <GL/glew.h>

namespace glimac {

	struct VAO {
		private:
			GLuint m_nGLId;

		public:
			VAO(){				
				glGenVertexArrays(1,&m_nGLId);
			}
			VAO(VAO&& rvalue): m_nGLId(rvalue.m_nGLId) {
				rvalue.m_nGLId = 0;
			}

			VAO& operator =(VAO&& rvalue) {
				m_nGLId = rvalue.m_nGLId;
				rvalue.m_nGLId = 0;
			}
			
			~VAO(){
				glDeleteVertexArrays(1,&m_nGLId);
			}
			GLuint getGLId() const{
				return m_nGLId;
			}

			void bind() const{
				glBindVertexArray(m_nGLId);
			}

			static void debind() {
				glBindVertexArray(0);	
			}
	};
}