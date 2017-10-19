#pragma once
#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "OpenGLheader.h"
#include "GLClasses.h"
namespace GLEngine {



	void key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mode);

	void mouse_callback(GLFWwindow* _window, double _xpos, double _ypos);

	void scroll_callback(GLFWwindow* _window, double _xoffset, double _yoffset);

	inline float& acceleration() {
		static float var = 1.0;		 
		return var;
	}

	void do_movement();

	void do_rotation();

	inline bool& default_pos() {
		static bool var = false;
		return var;
	}

	glm::mat4 RotatingCube();
}
#endif // !CALLBACKS_H
