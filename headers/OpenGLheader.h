#pragma once
#ifndef GLHEADER_H
#define GLHEADER_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "logs.h"

namespace GLEngine {

	inline uint16_t& GLVersion() {
		static uint16_t ver;
		return ver;
	}

	inline int InitOpenGL(int _glfw_profile = GLFW_OPENGL_CORE_PROFILE, int _glfw_resizable = GL_FALSE) {
		using namespace Settings;
		if (!glfwInit()) {
			if (LOGS::CAN_LOG())
				LOGS::LOG_STREAM() << "Failed to initialize GLFW!" << std::endl;
			else std::cout << "Failed to initialize GLFW!" << std::endl;
			return -1;
		}
		SettingsFileHandler fs;
		uint16_t version = fs.ReadGLVersion();
		int version_major = version / 100;
		int version_minor = (version % 100) / 10;
		GLVersion() = version;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, version_major);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, version_minor);
		glfwWindowHint(GLFW_OPENGL_PROFILE, _glfw_profile);
		glfwWindowHint(GLFW_RESIZABLE, _glfw_resizable);
		return 0;
	}

	inline int InitGLEW()
	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			if (LOGS::CAN_LOG())
				LOGS::LOG_STREAM() << "Failed to initialize GLEW!" << std::endl;
			else std::cout << "Failed to initialize GLEW!" << std::endl;
			return -1;
		}
		return 0;
	}

	inline void logErrors(std::string& _func_name)
	{
		if (!LOGS::CAN_LOG())return;
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			LOGS::LOG_STREAM() << _func_name<< std::endl;
			switch (err) {
			case GL_INVALID_ENUM:
				LOGS::LOG_STREAM() << "ERROR::GL_INVALID_ENUM" << std::endl;
				break;
			case GL_INVALID_VALUE:
				LOGS::LOG_STREAM() << "ERROR::GL_INVALID_VALUE" << std::endl;
				break;
			case GL_INVALID_OPERATION:
				LOGS::LOG_STREAM() << "ERROR::GL_INVALID_OPERATION" << std::endl;
				break;
			case GL_STACK_OVERFLOW:
				LOGS::LOG_STREAM() << "ERROR::GL_STACK_OVERFLOW" << std::endl;
				break;
			case GL_STACK_UNDERFLOW:
				LOGS::LOG_STREAM() << "ERROR::GL_STACK_UNDERFLOW" << std::endl;
				break;
			case GL_OUT_OF_MEMORY:
				LOGS::LOG_STREAM() << "ERROR::GL_OUT_OF_MEMORY" << std::endl;
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				LOGS::LOG_STREAM() << "ERROR::GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
				break;
			case GL_CONTEXT_LOST:
				LOGS::LOG_STREAM() << "ERROR::GL_CONTEXT_LOST" << std::endl;
				break;
			default:
				LOGS::LOG_STREAM() << "ERROR::UNKNOWN_ERROR" << std::endl;
				break;
			}
		}
	}
}
#endif // !GLHEADER_H
