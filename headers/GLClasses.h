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
		~Shader();
		void Use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setFloat(const std::string &name, float value) const;
		void setVec3(const std::string &name, glm::vec3& value) const;
		void setVec4(const std::string &name, glm::vec4& value) const;
		void setMat3(const std::string &name, glm::mat3& value, GLsizei count = 1, GLboolean transpose = GL_FALSE) const;
		void setMat4(const std::string &name, glm::mat4& value, GLsizei count = 1, GLboolean transpose = GL_FALSE) const;

		
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

	struct Material {
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;
		Material(glm::vec3&, glm::vec3&, glm::vec3&,float);
	};

	struct Light {		
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		float constant;
		float linear;
		float quadratic;

		float cutOff;
		float outerCutOff;
		Light(glm::vec3&_amb, glm::vec3&_diff, glm::vec3&_spec);
		Light(glm::vec3&_amb, glm::vec3&_diff, glm::vec3&_spec, float _linear, float _quadratic, float _cutOff, float _outerCutOff, float _constant = 1.0);
	};
}

#endif //!GLCLASSES_H

