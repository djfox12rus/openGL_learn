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
		aiString path;
	};

	struct Material {
		std::vector<Texture>texture_diffuse;
		std::vector<Texture>texture_specular;
		float shininess;
		Material() :
			texture_diffuse()
			, texture_specular()
			, shininess()
		{}
		Material(Material&_m) :
			texture_diffuse(_m.texture_diffuse)
			, texture_specular(_m.texture_specular)
			, shininess(_m.shininess)
		{}

	};

	class Mesh {
	private:
		uint32_t VAO, VBO, EBO;
		void setupMesh();
	public:
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		Material material;

		Mesh();
		~Mesh();
		Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, Material& _material);
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
		GLEngine::Material loadMaterial(aiMaterial *_mat);
		std::vector<Texture> loadMaterialTextures(aiMaterial *_mat, aiTextureType _type);
	public:
		/*  Functions   */
		Model(const char *_name);
		void Draw(const Shader& _shader);
	};

}


#endif // !MESH_H
