#include "../headers/models.h"

int GLEngine::__model::ParseModelFile(std::string &_path)
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
		else if (!in_str.compare("[Color]")) {
			model_file_stream >> in_str;
			ptr = &in_str[0];
			color.r = strtod(ptr, &ptr);
			model_file_stream >> in_str;
			ptr = &in_str[0];
			color.g = strtod(ptr, &ptr);
			model_file_stream >> in_str;
			ptr = &in_str[0];
			color.b = strtod(ptr, &ptr);
		}
		else if (!in_str.compare("[Normals]")) {
			model_file_stream >> in_str;
			ptr = &in_str[in_str.find("=")];
			ptr++;
			this->normalsEnable = strtoul(ptr, &ptr, 0);
		}
	}
	size_t length = this->size*(3 + this->textureEnable * 2 + this->normalsEnable * 3);
	GLfloat *vertices = new GLfloat[length];

	std::string texPath;
	bool flag = false;
	size_t count = 0;
	uint32_t fives = 0;

	model_file_stream >> in_str;
	if (this->textureEnable) {
		ptr = &in_str[in_str.find("=")];
		ptr++;
		texPath.assign(ptr);
		this->AttachTexture(texPath);
	}

	while (!model_file_stream.eof()) {
		model_file_stream >> in_str;

		if (flag) {
			if (fives < (3 + this->textureEnable * 2 + this->normalsEnable * 3))
			{
				ptr = &in_str[0];
				vertices[count] = strtod(ptr, &ptr);
				count++;
			}
			fives++;
			if (fives >= 8) fives = 0;
		}
		if (!in_str.compare("[Vertices]")) {
			flag = true;
		}
	}
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, length * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	if (this->textureEnable && !this->normalsEnable) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);		
	}
	else if (!this->textureEnable && this->normalsEnable) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
	}
	else if (this->textureEnable && this->normalsEnable) {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	}
	else {
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
	}
	glBindVertexArray(0);

	delete[]vertices;
	return 0;
}

void GLEngine::__model::AttachTexture(std::string& _texPath)
{
	glGenTextures(1, &this->texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height;
	unsigned char* texture_image = SOIL_load_image(_texPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(texture_image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLEngine::__model::__model() :size(), textureEnable(), normalsEnable(), VAO(), VBO(), EBO(), texture(), color()
{
}

GLEngine::__model::__model(const char *_modelname) : size(), textureEnable(), normalsEnable(), VAO(), VBO(), EBO(), texture(), color()
{
	Settings::SettingsFileHandler fs;
	std::string path = fs.ReadPathToModelFile(_modelname);
	if (!path.empty()) {
		if (ParseModelFile(path)) {
			if (LOGS::CAN_LOG())
				LOGS::LOG_STREAM() << "ERROR::MODEL::Could not parse model file." << std::endl;
		}
	}
	else {
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::MODEL::Could not read path to models." << std::endl;
	}
}

GLEngine::__model::__model(size_t _s, bool _textureEnable, bool _normalsEnable, GLuint _VAO, GLuint _VBO, GLuint _EBO, GLuint _texture, glm::vec3 _color) : size(_s), textureEnable(_textureEnable), normalsEnable(_normalsEnable), VAO(_VAO), VBO(_VBO), EBO(_EBO), texture(_texture), color(_color)
{
}


GLEngine::__model::~__model()
{
	if (this->VAO)
		glDeleteVertexArrays(1, &this->VAO);
	if (this->VBO)
		glDeleteBuffers(1, &this->VBO);
	if (this->EBO)
		glDeleteBuffers(1, &this->EBO);
}
