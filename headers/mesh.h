#pragma once
#ifndef MESH_H
#define MESH_H

#include "GLClasses.h"

namespace GLEngine {

	uint32_t TextureFromFile(std::string &path, const std::string &directory, bool gamma = false);

	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		Vertex() :Position(), Normal(), TexCoords() {}
	};

	struct Texture {
		uint32_t id;
		std::string type;
		aiString path;
	};

	class Mesh {
	private:		
		uint32_t VAO, VBO, EBO;		
		void setupMesh();
	public:		
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		std::vector<Texture> textures;
		
		Mesh();
		Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, std::vector<Texture>& _textures);
		void Draw(const Shader & _shader);
	
	};
	
	class Model
	{
	private:
		/*  Model Data  */
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string directory;
		/*  Functions   */
		void loadModel(std::string & _path);
		void processNode(aiNode *_node, const aiScene *_scene);
		Mesh processMesh(aiMesh *_mesh, const aiScene *_scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *_mat, aiTextureType _type, std::string _typeName);
	public:
		/*  Functions   */
		Model(const char *_name);
		void Draw(const Shader& _shader);
	};

}


#endif // !MESH_H
