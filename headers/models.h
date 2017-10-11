#pragma once
#ifndef MODELS_H
#define MODELS_H
#include "OpenGLheader.h"
namespace GLEngine {
	
	class __shape {
	private:
		int ParseModelFile(std::string&);		
		size_t size;
		bool textureEnable;
		bool normalsEnable;
	public:
		GLuint VAO;
		GLuint VBO;

		__shape();
		__shape(const char*);
		__shape(size_t _s, bool _textureEnable, bool _normalsEnable, GLuint _VAO, GLuint _VBO);
		__shape(__shape&);
		~__shape();

	};


	class __model
	{	
	private:		
		__shape* shapes;
		
	public:		
		void SetTexture(const char *);
		GLuint texture;
		glm::vec3 color;
		

		__model();		
		__model(__shape*, GLuint  _texture = 0, glm::vec3  _color = glm::vec3());
		~__model();
	};



}


#endif // !MODELS_H

