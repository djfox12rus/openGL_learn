#include "../headers/GLClasses.h"

GLEngine::__keys::__keys_8::__keys_8() :key_0(false), key_1(false), key_2(false), key_3(false), key_4(false), key_5(false), key_6(false), key_7(false)
{
}

bool GLEngine::__keys::operator[](int _place)
{
	if (_place<0 || _place >= this->size * 8)return false;
	int byte_pos = _place / 8;
	int bit_pos = _place % 8;
	switch (bit_pos)
	{
	case 0:return this->keys_array[byte_pos].key_0;
	case 1:return this->keys_array[byte_pos].key_1;
	case 2:return this->keys_array[byte_pos].key_2;
	case 3:return this->keys_array[byte_pos].key_3;
	case 4:return this->keys_array[byte_pos].key_4;
	case 5:return this->keys_array[byte_pos].key_5;
	case 6:return this->keys_array[byte_pos].key_6;
	case 7:return this->keys_array[byte_pos].key_7;
	default:return false;
		break;
	}
	return false;
}

void GLEngine::__keys::set_at(int _place)
{
	if (_place<0 || _place >= this->size * 8)return;
	int byte_pos = _place / 8;
	int bit_pos = _place % 8;
	switch (bit_pos)
	{
	case 0:this->keys_array[byte_pos].key_0 = true;
		break;
	case 1:this->keys_array[byte_pos].key_1 = true;
		break;
	case 2:this->keys_array[byte_pos].key_2 = true;
		break;
	case 3:this->keys_array[byte_pos].key_3 = true;
		break;
	case 4:this->keys_array[byte_pos].key_4 = true;
		break;
	case 5:this->keys_array[byte_pos].key_5 = true;
		break;
	case 6:this->keys_array[byte_pos].key_6 = true;
		break;
	case 7:this->keys_array[byte_pos].key_7 = true;
		break;
	default:return;
		break;
	}
}

void GLEngine::__keys::reset_at(int _place)
{
	if (_place<0 || _place >= this->size * 8)return;
	int byte_pos = _place / 8;
	int bit_pos = _place % 8;
	switch (bit_pos)
	{
	case 0:this->keys_array[byte_pos].key_0 = false;
		break;
	case 1:this->keys_array[byte_pos].key_1 = false;
		break;
	case 2:this->keys_array[byte_pos].key_2 = false;
		break;
	case 3:this->keys_array[byte_pos].key_3 = false;
		break;
	case 4:this->keys_array[byte_pos].key_4 = false;
		break;
	case 5:this->keys_array[byte_pos].key_5 = false;
		break;
	case 6:this->keys_array[byte_pos].key_6 = false;
		break;
	case 7:this->keys_array[byte_pos].key_7 = false;
		break;
	default:return;
		break;
	}
}


GLEngine::Shader::Shader()
{
}

GLEngine::Shader::Shader(const char * _shader_name)
{
	std::string vertexCode = std::string("#version ");
	vertexCode.append(std::to_string(GLVersion()));
	vertexCode.append("\n ");
	std::string fragmentCode = std::string("#version ");
	fragmentCode.append(std::to_string(GLVersion()));
	fragmentCode.append("\n ");
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	std::string vertexPath;
	std::string fragmentPath;
	
	Settings::SettingsFileHandler fs;
	if (fs.ReadShadersPaths(_shader_name, vertexPath, fragmentPath)) {
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::SHADER::Could not read path to shaders" << std::endl;
		
		vertexPath = std::string("VertexShader.vrt");
		fragmentPath = std::string("FragmentShader.frg");
	}


	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vertexCode.append(vShaderStream.str());
		fragmentCode.append(fShaderStream.str());
	}
	catch (std::ifstream::failure e) {
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}	
	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar* fShaderCode = fragmentCode.c_str();

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	success;
	infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	this->program = glCreateProgram();
	glAttachShader(this->program, vertexShader);
	glAttachShader(this->program, fragmentShader);
	glLinkProgram(this->program);

	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}	
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

void GLEngine::Shader::Use()
{
	glUseProgram(this->program);
}

void GLEngine::Shader::setBool(const std::string & name, bool value) const
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), (int)value);
}

void GLEngine::Shader::setInt(const std::string & name, int value) const
{
	glUniform1i(glGetUniformLocation(program, name.c_str()), value);
}

void GLEngine::Shader::setFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void GLEngine::Shader::setVec3(const std::string & name, glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y,value.z);
}

void GLEngine::Shader::setMat4(const std::string & name, glm::mat4 & value, GLsizei count, GLboolean transpose) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), count, transpose, glm::value_ptr(value));
}

GLEngine::Shader::~Shader()
{
}

GLEngine::__mouse_pos::__mouse_pos()
{
	int width, height;
	Settings::SettingsFileHandler fs;
	fs.ReadWidthHeightTitle(width, height);
	this->x = width / 2;
	this->y = height / 2;
}

GLEngine::__deltaTime::__deltaTime():delta(), lastFrame()
{
}

void GLEngine::__deltaTime::setDelta()
{
	GLfloat currentFrame = glfwGetTime();
	delta = currentFrame - lastFrame;	
	lastFrame = currentFrame;
}

GLEngine::__deltaTime::operator GLfloat()
{
	return delta;
}

GLEngine::__rotation::__rotation():x(),y(),z()
{
}

GLEngine::__screen::__screen() : Width(), Height(), AspectRatio()
{
}

GLEngine::__screen::__screen(GLuint _w, GLuint _h): Width(_w), Height(_h), AspectRatio((GLfloat)_w/(GLfloat)_h)
{	
}

GLEngine::Material::Material(glm::vec3 &_amb, glm::vec3 &_diff, glm::vec3 &_spec, float _shiny): ambient(_amb), diffuse(_diff), specular(_spec), shininess(_shiny)
{
}

GLEngine::Light::Light(glm::vec3 &_amb, glm::vec3 &_diff, glm::vec3 &_spec): ambient(_amb), diffuse(_diff), specular(_spec)
{
}
