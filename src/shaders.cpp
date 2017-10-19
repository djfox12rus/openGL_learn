#include "..\headers\shaders.h"

GLEngine::CoordsTransMatrices::CoordsTransMatrices() :
	model()
	, view()
	, projection()
	, normals()
{
}

GLEngine::CoordsTransMatrices::CoordsTransMatrices(glm::mat4 & _model, glm::mat4 & _view, glm::mat4 & _projection):
	model()
	, view()
	, projection()
	, normals(glm::inverse(glm::mat3(this->model)))
{
}

void GLEngine::CoordsTransMatrices::updateMatrixNormals()
{
	this->normals = glm::inverse(glm::mat3(this->model));
}

void GLEngine::CoordsTransMatrices::modelTranslate(glm::vec3 & _where)
{
	this->model = glm::translate(this->model, _where);
}

void GLEngine::CoordsTransMatrices::modelScale(glm::vec3 & _to)
{
	this->model = glm::scale(this->model, _to);
}


GLEngine::Shader::Shader()
{
}

GLEngine::Shader::Shader(const char * _shader_name, std::string& _defines)
{
	std::string vertexCode = std::string("#version ");
	vertexCode.append(std::to_string(GLVersion()));
	vertexCode.append("\n ");
	std::string fragmentCode = std::string("#version ");
	fragmentCode.append(std::to_string(GLVersion()));
	fragmentCode.append("\n ");
	fragmentCode.append(_defines);
	
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	vShaderFile.exceptions(std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::badbit);
	std::string vertexPath;
	std::string fragmentPath;
	std::string shadersDirectory;

	Settings::SettingsFileHandler fs;

	if (!fs.ReadShadersPaths(shadersDirectory)) {
		if (LOGS::CAN_LOG())
			LOGS::LOG_STREAM() << "ERROR::SHADER::Could not read path to shaders" << std::endl;
	}
	vertexPath = shadersDirectory;
	vertexPath.append(_shader_name);
	vertexPath.append(".vrt");
	fragmentPath = shadersDirectory;
	fragmentPath.append(_shader_name);
	fragmentPath.append(".frg");
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

void GLEngine::Shader::setUInt(const std::string & name, uint32_t value) const
{
	glUniform1ui(glGetUniformLocation(program, name.c_str()), value);
}

void GLEngine::Shader::setFloat(const std::string & name, float value) const
{
	glUniform1f(glGetUniformLocation(program, name.c_str()), value);
}

void GLEngine::Shader::setVec3(const std::string & name, glm::vec3& value) const
{
	glUniform3f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z);
}

void GLEngine::Shader::setVec4(const std::string & name, glm::vec4 & value) const
{
	glUniform4f(glGetUniformLocation(program, name.c_str()), value.x, value.y, value.z, value.w);
}

void GLEngine::Shader::setMat3(const std::string & name, glm::mat3 & value, GLsizei count, GLboolean transpose) const
{
	glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), count, transpose, glm::value_ptr(value));
}

void GLEngine::Shader::setMat4(const std::string & name, glm::mat4 & value, GLsizei count, GLboolean transpose) const
{
	glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), count, transpose, glm::value_ptr(value));
}

GLEngine::Shader::~Shader()
{
}

GLEngine::PhongLightingShader::PhongLightingShader():Shader("PhongLightingShader")
{
}

GLEngine::PhongLightingShader::PhongLightingShader(std::string& _defines):Shader("PhongLightingShader", _defines)
{	
}

GLEngine::PhongLightingShader::~PhongLightingShader()
{
}

void GLEngine::PhongLightingShader::setDirLight(DirectLight& _light)
{
	this->setVec3("dirLight.direction", _light.direction);	
	this->setVec3("dirLight.ambient", _light.ambient);
	this->setVec3("dirLight.diffuse", _light.diffuse);
	this->setVec3("dirLight.specular", _light.specular);	
}

void GLEngine::PhongLightingShader::setPointLight(PointLight& _light, size_t _i)
{
	std::string uniformStructName = "pointLights[";
	uniformStructName.append(std::to_string(_i));
	uniformStructName.append("].");

	std::string uniformName = uniformStructName;
	uniformName.append("position");
	this->setVec3(uniformName.c_str(), _light.position);

	uniformName = uniformStructName;
	uniformName.append("ambient");
	this->setVec3(uniformName.c_str(), _light.ambient);

	uniformName = uniformStructName;
	uniformName.append("diffuse");
	this->setVec3(uniformName.c_str(), _light.diffuse);

	uniformName = uniformStructName;
	uniformName.append("specular");
	this->setVec3(uniformName.c_str(), _light.specular);

	uniformName = uniformStructName;
	uniformName.append("constant");
	this->setFloat(uniformName.c_str(), _light.constant);
	
	uniformName = uniformStructName;
	uniformName.append("linear");
	this->setFloat(uniformName.c_str(), _light.linear);

	uniformName = uniformStructName;
	uniformName.append("quadratic");
	this->setFloat(uniformName.c_str(), _light.quadratic);	
}

void GLEngine::PhongLightingShader::setSpotLight(SpotLight& _light, size_t _i)
{	
	std::string uniformStructName = "spotLights[";
	uniformStructName.append(std::to_string(_i));
	uniformStructName.append("].");

	std::string uniformName = uniformStructName;
	uniformName.append("position");
	this->setVec3(uniformName.c_str(), _light.position);

	uniformName = uniformStructName;
	uniformName.append("direction");
	this->setVec3(uniformName.c_str(), _light.direction);

	uniformName = uniformStructName;
	uniformName.append("ambient");
	this->setVec3(uniformName.c_str(), _light.ambient);

	uniformName = uniformStructName;
	uniformName.append("diffuse");
	this->setVec3(uniformName.c_str(), _light.diffuse);

	uniformName = uniformStructName;
	uniformName.append("specular");
	this->setVec3(uniformName.c_str(), _light.specular);

	uniformName = uniformStructName;
	uniformName.append("constant");
	this->setFloat(uniformName.c_str(), _light.constant);

	uniformName = uniformStructName;
	uniformName.append("linear");
	this->setFloat(uniformName.c_str(), _light.linear);

	uniformName = uniformStructName;
	uniformName.append("quadratic");
	this->setFloat(uniformName.c_str(), _light.quadratic);

	uniformName = uniformStructName;
	uniformName.append("cutOff");
	this->setFloat(uniformName.c_str(), _light.cutOff);

	uniformName = uniformStructName;
	uniformName.append("outerCutOff");
	this->setFloat(uniformName.c_str(), _light.outerCutOff);	
}

void GLEngine::PhongLightingShader::setCameraPosition(glm::vec3 & _pos)
{
	this->setVec3("viewPos", _pos);	
}

void GLEngine::PhongLightingShader::setCoordsTransMatrices(CoordsTransMatrices & _matrices)
{
	this->setMat4("matrix_model", _matrices.model);
	this->setMat4("matrix_view", _matrices.view);
	this->setMat4("matrix_projection", _matrices.projection);	
	this->setMat3("matrix_normals", _matrices.normals, 1, GL_TRUE);	
}


