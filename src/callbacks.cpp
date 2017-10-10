#include "../headers/callbacks.h"

void GLEngine::key_callback(GLFWwindow * _window, int _key, int _scancode, int _action, int _mode)
{
	if (_key == GLFW_KEY_ESCAPE && _action == GLFW_PRESS)
		glfwSetWindowShouldClose(_window, GL_TRUE);

	if (_action == GLFW_PRESS)
		keys().set_at(_key);
	else if (_action == GLFW_RELEASE)
		keys().reset_at(_key);
}

void GLEngine::mouse_callback(GLFWwindow * _window, double _xpos, double _ypos)
{
	static bool firstMouse = true;
	if (firstMouse) // эта переменная была проинициализирована значением true
		if (firstMouse)
		{
			mousePos().x = _xpos;
			mousePos().y = _ypos;
			firstMouse = false;
		}

	GLfloat xoffset = _xpos - mousePos().x;
	GLfloat yoffset = mousePos().y - _ypos;  // Reversed since y-coordinates go from bottom to left

	mousePos().x = _xpos;
	mousePos().y = _ypos;

	Camera().ProcessMouseMovement(xoffset, yoffset);
}

void GLEngine::scroll_callback(GLFWwindow * _window, double _xoffset, double _yoffset)
{
	Camera().ProcessMouseScroll(_yoffset);
}

void GLEngine::do_movement()
{
	if (keys()[GLFW_KEY_W])
		Camera().ProcessKeyboard(FORWARD, deltaTime());
	if (keys()[GLFW_KEY_S])
		Camera().ProcessKeyboard(BACKWARD, deltaTime());
	if (keys()[GLFW_KEY_A])
		Camera().ProcessKeyboard(LEFT, deltaTime());
	if (keys()[GLFW_KEY_D])
		Camera().ProcessKeyboard(RIGHT, deltaTime());
}

void GLEngine::do_rotation()
{
	static const int step = 5000;
	if (keys()[GLFW_KEY_RIGHT])
	{
		rotation_around().y += M_PI / step;
		if (rotation_around().y >= M_PI * 2)
			rotation_around().y = M_PI / step;
	}
	if (keys()[GLFW_KEY_LEFT])
	{
		rotation_around().y -= M_PI / step;
		if (rotation_around().y < 0)
			rotation_around().y = M_PI * 2 - M_PI / step;
	}
	if (keys()[GLFW_KEY_PAGE_DOWN])
	{
		rotation_around().x += M_PI / step;
		if (rotation_around().x >= M_PI * 2)
			rotation_around().x = M_PI / step;
	}
	if (keys()[GLFW_KEY_PAGE_UP])
	{
		rotation_around().x -= M_PI / step;
		if (rotation_around().x < 0)
			rotation_around().x = M_PI * 2 - M_PI / step;
	}
	if (keys()[GLFW_KEY_DOWN])
	{
		rotation_around().z += M_PI / step;
		if (rotation_around().z >= M_PI * 2)
			rotation_around().z = M_PI / step;
	}
	if (keys()[GLFW_KEY_UP])
	{
		rotation_around().z -= M_PI / step;
		if (rotation_around().z < 0)
			rotation_around().z = M_PI * 2 - M_PI / step;
	}
	if (keys()[GLFW_KEY_RIGHT_CONTROL]) {
		rotation_around().z = 0;
		rotation_around().z = 0;
		rotation_around().z = 0;
	}
	if (keys()[GLFW_KEY_RIGHT_SHIFT]) {
		default_pos() = true;
	}
}

glm::mat4 GLEngine::RotatingCube()
{
	static glm::mat4 rotation_matr;
	if (default_pos()) {
		rotation_matr = glm::mat4();
		default_pos() = false;
	}
	else {
		rotation_matr = glm::rotate(rotation_matr, rotation_around().x, glm::vec3(0.0, 0.0, 1.0));
		rotation_matr = glm::rotate(rotation_matr, rotation_around().y, glm::vec3(0.0, 1.0, 0.0));
		rotation_matr = glm::rotate(rotation_matr, rotation_around().z, glm::vec3(1.0, 0.0, 0.0));		
	}

	return rotation_matr;
}
