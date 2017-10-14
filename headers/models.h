#pragma once
#ifndef MODELS_H
#define MODELS_H
#include "OpenGLheader.h"
namespace GLEngine {

	struct textureAtrib {
		GLuint texture;
		GLuint GL_sampler;
	};

	textureAtrib createTexture(const char *);
	struct _tex {
		bool textureIdices[32];

		bool& operator[](int _pos) {
			return textureIdices[_pos - GL_TEXTURE0];
		}
		_tex() {
			for (int i = 0; i < 32; i++)textureIdices[i] = false;
		}
	};

	inline _tex& texIndices() {
		static _tex var;
		return var;
	}

	

	
	class __shape {
	private:
		friend class __model;
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
		textureAtrib texture;
	public:		
		void SetTexture(const char *);
		glm::vec3 position;
		glm::vec3 color;
		

		__model();		
		__model(__shape*,  glm::vec3 _position = glm::vec3(), glm::vec3  _color = glm::vec3(), textureAtrib  _texture = textureAtrib());
		~__model();

		size_t VerticesNum();
		GLuint VAO();
		GLuint VBO();
		textureAtrib Texture();
		
	};



}


#endif // !MODELS_H

