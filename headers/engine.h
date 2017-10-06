#pragma once
#ifndef ENGINE_H
#define ENGINE_H
#include "GLheader.h"
#include "STD_commons.h"

namespace GLEngine {

	void key_callback(GLFWwindow* _window, int _key, int _scancode, int _action, int _mode);
	
	class GLwindow
	{
	private:
		GLFWwindow * win;
	public:
		GLwindow()throw(...);
		~GLwindow();

		GLFWwindow * InitWindow(int _width, int _height, const char*_title = "Window", GLFWmonitor* _monitor = nullptr, GLFWwindow* _share = nullptr);
		int InitViewport();
		void PollEvents();
		void SwapBuffers();

		inline GLFWwindow *WINDOW() {			
			return this->win==nullptr? InitWindow(1280,720): this->win;
		}
	};

	class Shader
	{
	public:		
		GLuint Program;		
		Shader(const char* _shader_name);		
		void Use();
	};


	void RainbowBackground();
	GLfloat* ProcessCoordsForRotatingTriangle(uint32_t, float, float, bool);
	int Engine();


}


#endif // !ENGINE_H


