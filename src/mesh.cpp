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

GLEngine::Mesh::Mesh() :VAO(), VBO(), EBO(), vertices(), indices(), textures()
{
}

GLEngine::Mesh::Mesh(std::vector<Vertex>& _vertices, std::vector<unsigned int>& _indices, std::vector<Texture>& _textures):VAO(), VBO(), EBO(), vertices(_vertices), indices(_indices), textures(_textures)
{
	setupMesh();
}

void GLEngine::Mesh::Draw(const Shader& _shader)
{
	using namespace std;
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	string temp;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);		
		string number;
		string name = textures[i].type;
		if (!name.compare("texture_diffuse")) {
			number.assign(to_string(diffuseNr++));
		}
		else if (!name.compare("texture_specular")) {
			number.assign(to_string(specularNr++));
		}		
		//"material.texture_diffuse#" или "material.texture_specular#", # - нумерация текстур.
		temp.assign("material.");
		temp.append(name);
		temp.append(number);
		/*if (LOGS::CAN_LOG()) {
			LOGS::LOG_STREAM() << "Texture used: " << temp<<" for VAO = "<< VAO << std::endl;
		}*/
		_shader.setInt(temp.c_str(), i);

		_shader.setFloat("material.shininess", 32.0f);

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
				
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

	}
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
	directory = _path.substr(0, _path.find_last_of('/'));

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

	if (_mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = _scene->mMaterials[_mesh->mMaterialIndex];
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());


	}

	return Mesh(vertices, indices, textures);
}

std::vector<GLEngine::Texture> GLEngine::Model::loadMaterialTextures(aiMaterial * _mat, aiTextureType _type, std::string _typeName)
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

		texture = Texture();
		texture.id = TextureFromFile(string(str.C_Str()), directory);
		texture.type = _typeName;
		texture.path = str;
		textures.push_back(texture);
		textures_loaded.push_back(texture);
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
