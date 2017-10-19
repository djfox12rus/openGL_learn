#pragma once
#ifndef HANDLER_H
#define HANDLER_H
#include "OpenGLheader.h"
#include "callbacks.h"
#include "mesh.h"

namespace GLEngine {

	GLFWwindow* InitWindow();
	inline GLFWwindow* Window() {
		static GLFWwindow* win(InitWindow());
		return win;
	}

	int Handler();
	

	int Main_loop();
}

#endif // !HANDLER_H
