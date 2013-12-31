#include "Object3D.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <SOIL/SOIL.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> 
#include "Kart.h"

using namespace glimac;

bool Object3D::LoadObjFromFile(std::string sFilePath)
{  
   Assimp::Importer importer; 
   const aiScene* scene = importer.ReadFile( sFilePath,  
      aiProcess_CalcTangentSpace       |  
      aiProcess_Triangulate            | 
      aiProcess_JoinIdenticalVertices  | 
      aiProcess_SortByPType); 

   if(!scene) 
   { 
      std::cerr << "Erreur d'importer" << std::endl;
      return false; 
   } 

   bool t=false;
   std::cout << scene->mNumMeshes << " meshes" << std::endl;
   for(int i=0; i<scene->mNumMeshes;++i) 
   { 
      aiMesh* mesh = scene->mMeshes[i]; 
      int iMeshFaces = mesh->mNumFaces; 
      for(int j=0;j<iMeshFaces;++j) 
      { 
         const aiFace& face = mesh->mFaces[j]; 
         for(int k=0;k<3;++k) 
         { 
			// std::cout << "Coucou " << i << " " << j << " " << k << std::endl;
			glimac::ShapeVertex vertex;
			vertex.position.x=mesh->mVertices[face.mIndices[k]].x;
			vertex.position.y=mesh->mVertices[face.mIndices[k]].y;
			vertex.position.z=mesh->mVertices[face.mIndices[k]].z;
			// std::cout << "Position done" << std::endl;
			// if (mesh->HasTextureCoords(0)) {
				// vertex.texCoords.x=mesh->mTextureCoords[0][face.mIndices[k]].x;
				// vertex.texCoords.y=mesh->mTextureCoords[0][face.mIndices[k]].y;
				// if(!t){
				// std::cout << "Textures reperees" << std::endl;
				// t=true;
				// }
			// }else {
				vertex.texCoords.x=mesh->mVertices[face.mIndices[k]].x;
				vertex.texCoords.y=mesh->mVertices[face.mIndices[k]].y;
			// }
			
			vertex.normal.x= mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]].x : vertex.normal.x=1.0f;
			vertex.normal.y= mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]].y : vertex.normal.y=1.0f;
			vertex.normal.z= mesh->HasNormals() ? mesh->mNormals[face.mIndices[k]].z : vertex.normal.z=1.0f;
			// std::cout << "Normal done" << std::endl;
			
			m_Vertices.push_back(vertex);
        } 
      }  
    } 

	m_nVertexCount=m_Vertices.size();
	
   return true; 
   
}

//Textures
int Object3D::LoadTexture(){
	int img_width=0, img_height=0;
	
	unsigned char* img = SOIL_load_image("../textures/default.png", &img_width, &img_height, NULL, 0);
	
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	
	
	return idTexture;

}

int Object3D::LoadTexture(const char* sFilePath){
	int img_width=0, img_height=0;
	
	unsigned char* img = SOIL_load_image(sFilePath, &img_width, &img_height, NULL, 0);
	
	glGenTextures(1, &idTexture);
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glBindTexture(GL_TEXTURE_2D,0);
	
	
	return idTexture;
	}

void Object3D::buildVAO(){
	vao.bind();
	vbo.bind();
	
	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex, position)));

	//Normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex, normal)));
	
	//TextureCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE, sizeof(ShapeVertex),(const GLvoid*)(offsetof(ShapeVertex, texCoords)));
	
	glimac::VBO::debind();
	glimac::VAO::debind();
}

void Object3D::build(){
	buildVBO();
	buildVAO();
}

void Object3D::buildVBO(){
	vbo.bind();	
	glBufferData(GL_ARRAY_BUFFER, sizeof(ShapeVertex)*m_nVertexCount, &m_Vertices[0], GL_STATIC_DRAW);	
	VBO::debind(); // on débinde
}
	
VAO& Object3D::getVAO(){
	return vao;
}

VBO& Object3D::getVBO(){
	return vbo;
}

void Object3D::Draw(GLuint uTex){
	glBindTexture(GL_TEXTURE_2D, idTexture);
	glUniform1i(uTex,0);
	glDrawArrays(GL_TRIANGLES,0,m_nVertexCount);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Object3D::sphere(GLfloat r, GLsizei discLat, GLsizei discLong) {
    // Equation paramétrique en (r, phi, theta) de la sphère 
    // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= phi <= 2PI
    //
    // x(r, phi, theta) = r sin(phi) cos(theta)
    // y(r, phi, theta) = r sin(theta)
    // z(r, phi, theta) = r cos(phi) cos(theta)
    //
    // Discrétisation:
    // dPhi = 2PI / discLat, dTheta = PI / discLong
    //
    // x(r, i, j) = r * sin(i * dPhi) * cos(-PI / 2 + j * dTheta)
    // y(r, i, j) = r * sin(-PI / 2 + j * dTheta)
    // z(r, i, j) = r * cos(i * dPhi) * cos(-PI / 2 + j * dTheta)

    GLfloat rcpLat = 1.f / discLat, rcpLong = 1.f / discLong;
    GLfloat dPhi = 2 * PI * rcpLat, dTheta = PI * rcpLong;
    
    std::vector<glimac::ShapeVertex> data;
    
    // Construit l'ensemble des vertex
    for(GLsizei j = 0; j <= discLong; ++j) {
        GLfloat cosTheta = cos(-PI / 2 + j * dTheta);
        GLfloat sinTheta = sin(-PI / 2 + j * dTheta);
        
        for(GLsizei i = 0; i <= discLat; ++i) {
            glimac::ShapeVertex vertex;
            
            vertex.texCoords.x = i * rcpLat;
            vertex.texCoords.y = 1.f - j * rcpLong;

            vertex.normal.x = sin(i * dPhi) * cosTheta;
            vertex.normal.y = sinTheta;
            vertex.normal.z = cos(i * dPhi) * cosTheta;
            
            vertex.position = r * vertex.normal;
            
            data.push_back(vertex);
        }
    }

    m_nVertexCount = discLat * discLong * 6;
    
    GLuint idx = 0;
    // Construit les vertex finaux en regroupant les données en triangles:
    // Pour une longitude donnée, les deux triangles formant une face sont de la forme:
    // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for(GLsizei j = 0; j < discLong; ++j) {
        GLsizei offset = j * (discLat + 1);
        for(GLsizei i = 0; i < discLat; ++i) {
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + (i + 1)]);
            m_Vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + discLat + 1 + (i + 1)]);
            m_Vertices.push_back(data[offset + i + discLat + 1]);
        }
    }
    
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs
}

void Object3D::cone(GLfloat height, GLfloat r, GLsizei discLat, GLsizei discHeight) {
    // Equation paramétrique en (r, phi, h) du cone
    // avec r >= 0, -PI / 2 <= theta <= PI / 2, 0 <= h <= height
    //
    // x(r, phi, h) = r (height - h) sin(phi) / height
    // y(r, phi, h) = h
    // z(r, phi, h) = r (height - h) cos(phi) / height
    //
    // Discrétisation:
    // dPhi = 2PI / discLat, dH = height / discHeight
    //
    // x(r, i, j) = r * (height - j * dH) * sin(i * dPhi) / height
    // y(r, i, j) = j * dH
    // z(r, i, j) = r * (height - j * dH) * cos(i * dPhi) / height

    GLfloat rcpLat = 1.f / discLat, rcpH = 1.f / discHeight;
    GLfloat dPhi = 2 * PI * rcpLat, dH = height * rcpH;
    
    std::vector<glimac::ShapeVertex> data;
    
    // Construit l'ensemble des vertex
    for(GLsizei j = 0; j <= discHeight; ++j) {
        for(GLsizei i = 0; i < discLat; ++i) {
            glimac::ShapeVertex vertex;
            
            vertex.texCoords.x = i * rcpLat;
            vertex.texCoords.y = j * rcpH;
            
            vertex.position.x = r * (height - j * dH) * sin(i * dPhi) / height;
            vertex.position.y = j * dH;
            vertex.position.z = r * (height - j * dH) * cos(i * dPhi) / height;
            
            /* avec cette formule la normale est mal définie au sommet (= (0, 0, 0))
            vertex.normal.x = vertex.position.x;
            vertex.normal.y = r * r * (1 - vertex.position.y / height) / height;
            vertex.normal.z = vertex.position.z;
            vertex.normal = glm::normalize(vertex.normal);
            */
            
            vertex.normal.x = sin(i * dPhi);
            vertex.normal.y = r / height;
            vertex.normal.z = cos(i * dPhi);
            vertex.normal = glm::normalize(vertex.normal);
            
            data.push_back(vertex);
        }
    }

    m_nVertexCount = discLat * discHeight * 6;
    
    GLuint idx = 0;
    // Construit les vertex finaux en regroupant les données en triangles:
    // Pour une longitude donnée, les deux triangles formant une face sont de la forme:
    // (i, i + 1, i + discLat + 1), (i, i + discLat + 1, i + discLat)
    // avec i sur la bande correspondant à la longitude
    for(GLsizei j = 0; j < discHeight; ++j) {
        GLsizei offset = j * discLat;
        for(GLsizei i = 0; i < discLat; ++i) {
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + (i + 1) % discLat]);
            m_Vertices.push_back(data[offset + discLat + (i + 1) % discLat]);
            m_Vertices.push_back(data[offset + i]);
            m_Vertices.push_back(data[offset + discLat + (i + 1) % discLat]);
            m_Vertices.push_back(data[offset + i + discLat]);
        }
    }
    
    // Attention ! dans cette implantation on duplique beaucoup de sommets. Une meilleur stratégie est de passer
    // par un Index Buffer Object, que nous verrons dans les prochains TDs
}

void Object3D::TransfoMatrix(glm::mat4 ViewMatrix, glm::vec3 tra){
		MVMatrix = ViewMatrix;
		MVMatrix = glm::translate(MVMatrix,tra);	
		MVMatrix = glm::rotate(MVMatrix, angle,glm::vec3(0,1,0));	
		MVMatrix = glm::scale(MVMatrix, sca);
}

void Object3D::MatrixToShader(GLuint uMVMatrix,GLuint uMVPMatrix,GLuint uNormalMatrix,int WIDTH, int HEIGHT){
		glm::mat4 ProjMatrix=glm::perspective(70.f,(float)WIDTH/(float)HEIGHT,0.1f,100.f); //angle vertical, ratio largeur/hauteur, near, far
		glUniformMatrix4fv(uMVMatrix, 1, false, glm::value_ptr(MVMatrix));
		glUniformMatrix4fv(uMVPMatrix, 1, false, glm::value_ptr(ProjMatrix*MVMatrix));			
		glUniformMatrix4fv(uNormalMatrix, 1, false, glm::value_ptr(glm::transpose(glm::inverse(MVMatrix))));
}

Object3D::Object3D(const Object3D& obj){
	idTexture=obj.idTexture;
	pos=obj.pos;
	dir=obj.dir;
	angle=obj.angle;
	m_Vertices=obj.m_Vertices;
	m_nVertexCount=obj.m_nVertexCount;
	hitboxSize=obj.hitboxSize;
	build();
}







// Teste si l'objet entre en collision avec un autre objet
bool Object3D::isInCollision(Object3D &other){
	if(!other.isVisible())
		return false;
    // Variables de collision selon les différents axes
    bool collision_x = false;
    bool collision_y = false;
    bool collision_z = false;

    // Position de l'objet sur lequel est appelé la fonction
    float x1 = pos.x;
    float y1 = pos.y;
    float z1 = pos.z;
    float h1 = hitboxSize;
//std::cout << h1 << std::endl;
    // Position de l'autre objet 
    float x2 = other.pos.x;
    float y2 = other.pos.y;
    float z2 = other.pos.z;
    float h2 = other.hitboxSize;
//std::cout << x2 << std::endl;
//std::cout << h2 << std::endl;

    // Teste si les 2 objets sont en collision sur l'axe x
    if( ((x2-h2 <= x1+h1) && (x1+h1 <= x2+h2)) || ((x2-h2 <= x1-h1) && (x1-h1 <= x2+h2)) ){
        collision_x = true;
    }

    // Teste si les 2 objets sont en collision sur l'axe y
    if( ((y2-h2 <= y1+h1) && (y1+h1 <= y2+h2)) || ((y2-h2 <= y1-h1) && (y1-h1 <= y2+h2)) ){
        collision_y = true;
    }

    // Teste si les 2 objets sont en collision sur l'axe z
    if( ((z2-h2 <= z1+h1) && (z1+h1 <= z2+h2)) || ((z2-h2 <= z1-h1) && (z1-h1 <= z2+h2)) ){
        collision_z = true;
    }


    if(collision_x==true && collision_y==true && collision_z==true){
        // std::cout << "collision !" << std::endl;
        return true;
    }else{
        // std::cout << "pas collision..." << std::endl;
        return false;
    }
}


// Empêche les collisions entre objets
void Object3D::avoidCollision(Object3D &other){
    // Position de l'objet sur lequel est appelé la fonction
	float x1 = pos.x;
    float y1 = pos.y;
    float z1 = pos.z;
    float h1 = hitboxSize;

    // Position de l'autre objet 
    float x2 = other.pos.x;
    float y2 = other.pos.y;
    float z2 = other.pos.z;
    float h2 = other.hitboxSize;

    // Si les 2 objets sont en collision sur l'axe x à gauche
    if( (x2-h2 <= x1+h1) && (x1+h1 <= x2) ){
        pos=glm::vec3(x2-h2-h1, y1, z1);        
    }

    // Si les 2 objets sont en collision sur l'axe x à droite
    if( (x2 <= x1-h1) && (x1-h1 <= x2+h2) ){
        pos=glm::vec3(x2+h2+h1, y1, z1);        
    }

    // Si les 2 objets sont en collision sur l'axe y à gauche
    if( (y2-h2 <= y1+h1) && (y1+h1 <= y2) ){
       pos=glm::vec3(x1, y2-h2-h1, z1);        
    }

    // Si les 2 objets sont en collision sur l'axe y à droite
    if( (y2 <= y1-h1) && (y1-h1 <= y2+h2) ){
        pos=glm::vec3(x1, y2-h2-h1, z1);        
    }

    // Si les 2 objets sont en collision sur l'axe z à gauche
    if( (z2-h2 <= z1+h1) && (z1+h1 <= z2) ){
        pos=glm::vec3(x1, y1, z2-h2-h1);        
    }

    // Si les 2 objets sont en collision sur l'axe z à droite
    if( (z2 <= z1-h1) && (z1-h1 <= z2+h2) ){
        pos=glm::vec3(x1, y1, z2+h2+h1);        
    }
}