#pragma once
#ifndef GLCLASSES_H
#define GLCLASSES_H
#include "OpenGLheader.h"
#include "camera.h"

namespace GLEngine {
	struct __screen {
		GLuint Width;
		GLuint Height;		
		GLfloat AspectRatio;
		__screen();
		__screen(GLuint, GLuint);
	};

	inline __screen& Screen() {
		static __screen scr;
		return scr;
	}

	struct __keys {
	private:
		struct __keys_8 {
			bool key_0 : 1;
			bool key_1 : 1;
			bool key_2 : 1;
			bool key_3 : 1;
			bool key_4 : 1;
			bool key_5 : 1;
			bool key_6 : 1;
			bool key_7 : 1;

			__keys_8();
		};
		static const size_t size = 64;
		__keys_8 keys_array[size];//512 булевых элементов, возможно нужно больше
	public:

		//не ссылочный!
		bool operator[](int);
		void set_at(int);
		void reset_at(int);

	};

	inline __keys& keys() {
		static __keys var;
		return var;
	}

	class Shader
	{
	private:			
		Shader();
	public:		
		GLuint program;
		Shader(const char* _shader_name);
		void Use();
		~Shader();
	};

	struct __mouse_pos {
		GLfloat x;
		GLfloat y;
		__mouse_pos();
	};

	inline __mouse_pos& mousePos() {
		static __mouse_pos pos;
		return pos;
	}

	struct __deltaTime {
		GLfloat delta;
		GLfloat lastFrame;

		__deltaTime();
		void setDelta();
		operator GLfloat();

	};

	inline __deltaTime& deltaTime() {
		static __deltaTime del;
		return del;
	}
	
	struct __rotation {
		GLfloat x;
		GLfloat y;
		GLfloat z;
		__rotation();
	};

	inline __rotation& rotation_around() {
		static __rotation r;
		return r;
	}
}

#endif //!GLCLASSES_H

