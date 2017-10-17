#include "../headers/models.h"

int GLEngine::__shape::ParseModelFile(std::string &_path)
{
	std::ifstream model_file_stream;
	model_file_stream.open(_path, std::fstream::in);
	if (!model_file_stream.is_open())
		return -1;
	std::string in_str;
	char* ptr;
	while (!model_file_stream.eof()) {
		model_file_stream >> in_str;
		if (!in_str.compare("[VerticesNum]")) {
			model_file_stream >> in_str;
			this->size = strtoul(in_str.c_str(), nullptr, 0);
		}
		else if (!in_str.compare("[Texture]")) {
			model_file_stream >> in_str;
			ptr = &in_str[in_str.find("=")];
			ptr++;
			this->textureEnable = strtoul(ptr, &ptr, 0);
			break;
		}
		else if (!in_str.compare("[Normals]")) {
			model_file_stream >> in_str;
			ptr = &in_str[in_str.find("=")];
			ptr++;
			this->normalsEnable = strtoul(ptr, &ptr, 0);
		}
	}
	size_t layout = (3 + this->textureEnable * 2 + this->normalsEnable * 3);
	size_t length = this->size*layout;

	std::vector<GLfloat> vertices;
	vertices.resize(length);

	GLfloat *t = vertices.data();
	//std::string texPath;
	//bool flag = false;
	size_t count = 0;
	uint32_t triangle_flag = 0;
	uint32_t vertex_flag = 0;

	glm::vec3 vector1;
	glm::vec3 vector2;
	glm::vec3 normal;

	while (in_str.compare("[Vertices]")) {
		model_file_stream >> in_str;
	}

	while (!model_file_stream.eof() && count < length) {
		if (vertex_flag < 3)
		{
			model_file_stream >> in_str;
			ptr = &in_str[0];
			vertices[count] = strtod(ptr, &ptr);
		}
		count++;
		vertex_flag++;
		if (vertex_flag == layout) vertex_flag = 0;
	}
	if (this->normalsEnable)
		for (count = 0; count < length; count += 3 * layout) {
			vector1 = glm::vec3(vertices[count + layout] - vertices[count], vertices[count + layout + 1] - vertices[count + 1], vertices[count + layout + 2] - vertices[count + 2]);
			vector2 = glm::vec3(vertices[count + 2 * layout] - vertices[count], vertices[count + 2 * layout + 1] - vertices[count + 1], vertices[count + 2 * layout + 2] - vertices[count + 2]);
			normal = glm::normalize(glm::cross(vector1, vector2));

			for (int i = 0; i < 3; i++) {
				vertices[count + i*layout  + 3] = normal.x;
				vertices[count + i*layout + 4] = normal.y;
				vertices[count + i*layout + 5] = normal.z;
			}
		}

	
	if (this->textureEnable)
	{
		while (in_str.compare("[TexCoords]")) {
			model_file_stream >> in_str;
		}
		count = 1;
		while (!model_file_stream.eof() && count <= this->size) {
			for (int j = count*layout - 2; j < count*layout; j++) {
				model_file_stream >> in_str;
				ptr = &in_str[0];
				vertices[j] = strtod(ptr, &ptr);
			}
			count++;
		}
	}
	
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices
	, GL_STATIC_DRAW);*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, layout * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	if (this->normalsEnable) {
		glVertexAttribPointer((int)this->normalsEnable, 3, GL_FLOAT, GL_FALSE, layout * sizeof(GLfloat), (GLvoid*)(3* this->normalsEnable * sizeof(GLfloat)));
		glEnableVertexAttribArray((int)this->normalsEnable);
	}

	if (this->textureEnable) {
		glVertexAttribPointer((int)this->normalsEnable+(int)this->textureEnable, 2, GL_FLOAT, GL_FALSE, layout * sizeof(GLfloat), (GLvoid*)(3*((int)this->normalsEnable + (int)this->textureEnable) * sizeof(GLfloat)));
		glEnableVertexAttribArray((int)this->normalsEnable + (int)this->textureEnable);
	}
	
	
	glBindVertexArray(0);


	return 0;
}

GLEngine::__shape::__shape() : size(), textureEnable(), normalsEnable(), VAO(), VBO()
{
}

GLEngine::__shape::__shape(const char *_shapename) : size(), textureEnable(), normalsEnable(), VAO(), VBO()
{
	Settings::SettingsFileHandler fs;
	std::string path = fs.ReadPathTo(_shapename);
	if (!path.empty()) {
		if (ParseModelFile(path)) {
			if (LOGS::CAN_LOG())
				LOGS::LOG_STREAM() << "ERROR::MODEL::Could not parse shape file." << std::endl;
		}
	}
	else {
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::MODEL::Could not read path to shape." << std::endl;
	}
}

GLEngine::__shape::__shape(size_t _s, bool _textureEnable, bool _normalsEnable, GLuint _VAO, GLuint _VBO) : size(_s), textureEnable(_textureEnable), normalsEnable(_normalsEnable), VAO(_VAO), VBO(_VBO)
{
}

GLEngine::__shape::__shape(__shape &)
{
}

GLEngine::__shape::~__shape()
{
	if (this->VAO)
		glDeleteVertexArrays(1, &this->VAO);
	if (this->VBO)
		glDeleteBuffers(1, &this->VBO);
}

void GLEngine::__model::SetTexture(const char * _texName)
{
	if (shapes->textureEnable)
		this->texture = createTexture(_texName);
}



GLEngine::__model::__model() :shapes(), texture(), color()
{
}


GLEngine::__model::__model(__shape* _sh,  glm::vec3 _position, glm::vec3 _color, textureAtrib _texture) : shapes(_sh), texture(_texture), color(_color), position(_position)
{
}


GLEngine::__model::~__model()
{
}

size_t GLEngine::__model::VerticesNum()
{
	return shapes->size;
}

GLuint GLEngine::__model::VAO()
{
	return shapes->VAO;
}

GLuint GLEngine::__model::VBO()
{
	return shapes->VBO;
}

GLEngine::textureAtrib GLEngine::__model::Texture()
{
	return texture;
}

GLEngine::textureAtrib GLEngine::createTexture(const char *_texName)
{
	Settings::SettingsFileHandler fs;
	std::string path = fs.ReadPathTo(_texName);
	if (path.empty()) {
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::MODEL::Could not attach texture." << std::endl;
		return textureAtrib();
	}
	textureAtrib out;
	out.GL_sampler = GL_TEXTURE0;
	while (texIndices()[out.GL_sampler] && out.GL_sampler <= GL_TEXTURE31) {
		out.GL_sampler++;
	}

	texIndices()[out.GL_sampler] = true;
	glGenTextures(1, &out.texture);
	glActiveTexture(out.GL_sampler);
	glBindTexture(GL_TEXTURE_2D, out.texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* texture_image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(texture_image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return out;
}
