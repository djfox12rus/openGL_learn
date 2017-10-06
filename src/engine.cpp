
#include "../headers/engine.h"




GLEngine::GLwindow::GLwindow()
{
	Settings::SettingsFileHandler fs;
	int w, h;
	char*t = new char[100];
	if (fs.ReadWidthHeightTitle(w, h, &t)) {
		w = 1280; h = 720;	
		strcpy_s(t,100, "Window");
	}

	if (InitWindow(w,h,t) == nullptr) {
		throw std::runtime_error("Could not create window");
	}	
	delete[]t;
}


GLEngine::GLwindow::~GLwindow()
{

}

GLFWwindow* GLEngine::GLwindow::InitWindow(int _width, int _height, const char * _title, GLFWmonitor * _monitor, GLFWwindow * _share)
{
	this->win = glfwCreateWindow(_width, _height, _title, _monitor, _share);
	if (this->win == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(this->win);
	InitGLEW();
	glfwSetKeyCallback(this->win, key_callback);	
	InitViewport();
	return this->win;
}

int GLEngine::GLwindow::InitViewport()
{
	int width, height;
	glfwGetFramebufferSize(this->win, &width, &height);
	glViewport(0, 0, width, height);
	return 0;
}

void GLEngine::GLwindow::PollEvents()
{
	glfwPollEvents();
}

void GLEngine::GLwindow::SwapBuffers()
{
	glfwSwapBuffers(win);
}

void GLEngine::key_callback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode)
{
	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, GL_TRUE);
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
	char*vertexPath = new char[100];
	char*fragmentPath = new char[100];
	Settings::SettingsFileHandler fs;
	if (fs.ReadShadersPaths(_shader_name, &vertexPath, &fragmentPath)) {
		std::cout << "ERROR::SHADER::Could not read path to shaders" << std::endl;
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
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	delete[]vertexPath;
	delete[]fragmentPath;
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
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	this->Program = glCreateProgram();
	glAttachShader(this->Program, vertexShader);
	glAttachShader(this->Program, fragmentShader);
	glLinkProgram(this->Program);
	
	glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}



void GLEngine::Shader::Use()
{
	glUseProgram(this->Program);
}

void GLEngine::RainbowBackground()
{
	static uint32_t count = 0;
	static uint16_t rainbow_count = 0;
	static float color_red = 1;
	static float color_green = 0;
	static float color_blue = 0;
	if (rainbow_count == 0) {
		color_red = float(UINT32_MAX - count) / UINT32_MAX;
		color_green = float(count) / UINT32_MAX;
	}
	else if (rainbow_count == 1) {
		color_green = float(UINT32_MAX - count) / UINT32_MAX;
		color_blue = float(count) / (UINT32_MAX);
	}
	else {
		color_blue = float(UINT32_MAX - count) / (UINT32_MAX);
		color_red = float(count) / UINT32_MAX;
	}


	if (count != 0) {
		glClearColor(color_red, color_green, color_blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	count += 16777216;

	//std::cout << count << ", " << rainbow_count << std::endl;

	if (count == 0) {
		rainbow_count = (rainbow_count == 2) ? 0 : rainbow_count + 1;
	}
}

GLfloat * GLEngine::ProcessCoordsForRotatingTriangle(uint32_t _lenght, float _x_shift, float _y_shift, bool _direction)
{
	const float sqrt3_2 = sqrt(3) / 2;
	const float half = 0.5;
	const float _2_pi_3 = M_PI * 2 / 3;
	const uint32_t len = 18 * _lenght;
	float sin_t = 0;
	float cos_t = 0;
	float step_t = _2_pi_3 / (len);
	GLfloat* vertices = new GLfloat[len];
	for (int count = 0; count < len; count += 18) {
		sin_t = sin(step_t*count);
		cos_t = cos(step_t*count);
		if (_direction) {
			vertices[count] = half*cos_t + _x_shift;
			vertices[count + 1] = half*sin_t + _y_shift;
			vertices[count + 2] = 0.0f;

			vertices[count + 3] = 1.0f;
			vertices[count + 4] = 0.0f;
			vertices[count + 5] = 0.0f;

			vertices[count + 6] = half*(-half*cos_t - sqrt3_2*sin_t) + _x_shift;
			vertices[count + 7] = half*(sqrt3_2*cos_t - half*sin_t) + _y_shift;
			vertices[count + 8] = 0.0f;

			vertices[count + 9] = 0.0f;
			vertices[count + 10] = 1.0f;
			vertices[count + 11] = 0.0f;

			vertices[count + 12] = half*(-half*cos_t + sqrt3_2*sin_t) + _x_shift;
			vertices[count + 13] = half*(-sqrt3_2*cos_t - half*sin_t) + _y_shift;
			vertices[count + 14] = 0.0f;

			vertices[count + 15] = 0.0f;
			vertices[count + 16] = 0.0f;
			vertices[count + 17] = 1.0f;
		}
		else {
			vertices[count + 1] = half*cos_t + _y_shift;
			vertices[count] = half*sin_t + _x_shift;
			vertices[count + 2] = 0;

			vertices[count + 3] = 1.0f;
			vertices[count + 4] = 0.0f;
			vertices[count + 5] = 0.0f;

			vertices[count + 7] = half*(-half*cos_t - sqrt3_2*sin_t) + _y_shift;
			vertices[count + 6] = half*(sqrt3_2*cos_t - half*sin_t) + _x_shift;
			vertices[count + 8] = 0;

			vertices[count + 9] = 0.0f;
			vertices[count + 10] = 1.0f;
			vertices[count + 11] = 0.0f;

			vertices[count + 13] = half*(-half*cos_t + sqrt3_2*sin_t) + _y_shift;
			vertices[count + 12] = half*(-sqrt3_2*cos_t - half*sin_t) + _x_shift;
			vertices[count + 14] = 0;

			vertices[count + 15] = 0.0f;
			vertices[count + 16] = 0.0f;
			vertices[count + 17] = 1.0f;
		}
	}
	return vertices;
}

int GLEngine::Engine()
{
	GLwindow window;
	Shader my_shader = Shader("[MyShader]");

	const uint32_t len = 100;
	GLfloat* vertices_1 = ProcessCoordsForRotatingTriangle(len, 0.26, 0.26, true);
	GLfloat* vertices_2 = ProcessCoordsForRotatingTriangle(len, -0.26, -0.26, false);

	GLuint VBO[2],VAO[2];
	glGenVertexArrays(2, VAO); 
	glGenBuffers(2, VBO);


	glBindVertexArray(VAO[0]);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, 18* len* sizeof(GLfloat), vertices_1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, len * 18 * sizeof(GLfloat), vertices_2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	uint32_t counter = 0;
	while (!glfwWindowShouldClose(window.WINDOW()))
	{
		window.PollEvents();


		RainbowBackground();
		my_shader.Use();
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 3 * counter, 3);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 3 * counter, 3);
		glBindVertexArray(0);
		counter = (counter == len - 1) ? 0 : counter + 1;


		window.SwapBuffers();
	}
	delete[]vertices_2;
	delete[]vertices_1;
	return 0;
}


