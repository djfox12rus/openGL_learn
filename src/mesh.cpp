#include "..\headers\mesh.h"

void GLEngine::Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

GLEngine::Mesh::Mesh() :
	VAO()
	, VBO()
	, EBO()
	, vertices()
	, indices()
	, material()
{
}

GLEngine::Mesh::~Mesh()
{
	/*glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
	glDeleteBuffers(1, &this->EBO);*/
}

GLEngine::Mesh::Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, Material& _material):
	VAO()
	, VBO()
	, EBO()
	, vertices(_vertices)
	, indices(_indices)
	, material(_material)
{
	setupMesh();
}

void GLEngine::Mesh::Draw(const Shader& _shader)
{
	using namespace std;
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	string temp;
	string number;	
	for (unsigned int i = 0; i < this->material.texture_diffuse.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);							
		number.assign(to_string(diffuseNr++));		
		temp.assign("material.texture_diffuse");		
		temp.append(number);
		_shader.setInt(temp.c_str(), i);		
		glBindTexture(GL_TEXTURE_2D, material.texture_diffuse[i].id);		
	}
	for (unsigned int i = 0; i < this->material.texture_specular.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i + this->material.texture_diffuse.size());
		number.assign(to_string(specularNr++));
		temp.assign("material.texture_specular");
		temp.append(number);
		_shader.setInt(temp.c_str(), i + this->material.texture_diffuse.size());
		glBindTexture(GL_TEXTURE_2D, material.texture_specular[i].id);
	}
	_shader.setFloat("material.shininess", material.shininess);
	GLEngine::logErrors(std::string("GLEngine::Mesh::Draw"));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void GLEngine::Model::loadModel(std::string & _path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(_path, aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		if (LOGS::CAN_LOG()) {
			LOGS::LOG_STREAM() << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		}
		return;
	}
	this->directory = _path.substr(0, _path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void GLEngine::Model::processNode(aiNode * _node, const aiScene * _scene)
{
	for (uint32_t i = 0; i < _node->mNumMeshes; i++)
	{
		aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
		this->meshes.push_back(processMesh(mesh, _scene));
	}

	for (uint32_t i = 0; i < _node->mNumChildren; i++)
	{
		processNode(_node->mChildren[i], _scene);
	}

}

GLEngine::Mesh GLEngine::Model::processMesh(aiMesh * _mesh, const aiScene * _scene)
{
	using namespace std;
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Vertex vertex;
	for (uint32_t i = 0; i < _mesh->mNumVertices; i++)
	{
		vertex = Vertex();		
		vertex.Position = glm::vec3(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(_mesh->mNormals[i].x, _mesh->mNormals[i].y, _mesh->mNormals[i].z);
		if (_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{				
			vertex.TexCoords = glm::vec2(_mesh->mTextureCoords[0][i].x, _mesh->mTextureCoords[0][i].y);
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	for (uint32_t i = 0; i < _mesh->mNumFaces; i++)
	{
		aiFace face = _mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Material material;
	if (_mesh->mMaterialIndex >= 0)
	{		
		material = loadMaterial(_scene->mMaterials[_mesh->mMaterialIndex]);
		if (LOGS::CAN_LOG()) {
			LOGS::LOG_STREAM() << "Material loaded: " << std::endl;
			for (int i = 0; i < material.texture_diffuse.size(); i++) {
				LOGS::LOG_STREAM() << "DIff: "<< material.texture_diffuse[i].path.C_Str() << std::endl;
			}
			for (int i = 0; i < material.texture_specular.size(); i++) {
				LOGS::LOG_STREAM() << "Spec: "<< material.texture_specular[i].path.C_Str() << std::endl;
			}
			LOGS::LOG_STREAM() <<"Shiny: "<< material.shininess << std::endl;
		}
	}
	
	return Mesh(vertices, indices, material);
}

GLEngine::Material GLEngine::Model::loadMaterial(aiMaterial * _mat)
{
	using namespace std;
	Material material;	
	vector<Texture> diffuseMaps = loadMaterialTextures(_mat, aiTextureType_DIFFUSE);
	material.texture_diffuse.insert(material.texture_diffuse.end(), diffuseMaps.begin(), diffuseMaps.end());
	vector<Texture> specularMaps = loadMaterialTextures(_mat, aiTextureType_SPECULAR);
	material.texture_specular.insert(material.texture_specular.end(), specularMaps.begin(), specularMaps.end());
	_mat->Get(AI_MATKEY_SHININESS, material.shininess);
	return material;
}

std::vector<GLEngine::Texture> GLEngine::Model::loadMaterialTextures(aiMaterial * _mat, aiTextureType _type)
{
	using namespace std;
	vector<Texture> textures;
	aiString str;
	Texture texture;
	bool skip = false;
	for (uint32_t i = 0; i < _mat->GetTextureCount(_type); i++)
	{
		str = aiString();
		_mat->GetTexture(_type, i, &str);
		skip = false;
		for (uint32_t j = 0; j < textures_loaded.size(); j++)
		{
			if (!std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()))
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			texture = Texture();
			texture.id = TextureFromFile(string(str.C_Str()), directory);
			texture.path = str;
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}	
	return textures;
}

GLEngine::Model::Model(const char * _name)
{
	Settings::SettingsFileHandler fs;	
	loadModel(fs.ReadPathTo(_name));
}

void GLEngine::Model::Draw(const Shader& _shader)
{
	for (uint32_t i = 0; i < meshes.size(); i++)
		meshes[i].Draw(_shader);
}

uint32_t GLEngine::TextureFromFile(std::string & path, const std::string & directory, bool gamma)
{
	using namespace std;
	string filename = directory + '/' + path;
	

	uint32_t textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = SOIL_load_image(filename.c_str(), &width, &height, &nrComponents, SOIL_LOAD_AUTO);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		SOIL_free_image_data(data);
	}
	else
	{
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "Texture failed to load at path: " << path << std::endl;		
		SOIL_free_image_data(data);
	}

	return textureID;
}

