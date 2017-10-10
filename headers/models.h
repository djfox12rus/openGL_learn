#pragma once
#ifndef MODELS_H
#define MODELS_H
#include "OpenGLheader.h"
namespace GLEngine {

	class __model
	{	
	private:
		int ParseModelFile(std::string&);
		void AttachTexture(std::string&);
		size_t size;
		bool textureEnable;
		bool normalsEnable;
	public:		
		GLuint VAO;
		GLuint VBO;
		GLuint EBO;
		GLuint texture;
		glm::vec3 color;
		

		__model();
		__model(const char*);
		__model(size_t _s, bool _textureEnable, bool _normalsEnable, GLuint _VAO, GLuint _VBO, GLuint _EBO, GLuint _texture, glm::vec3 _color);
		~__model();
	};



}


#endif // !MODELS_H

