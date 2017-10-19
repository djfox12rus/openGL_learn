#pragma once
#ifndef LIGHTS_H
#define LIGHTS_H
#include "OpenGLheader.h"
namespace GLEngine {

	struct DirectLight {
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		DirectLight();
		DirectLight(
			glm::vec3& _direction
			, glm::vec3& _ambient
			, glm::vec3& _diffuse
			, glm::vec3& _specular
			);

	};

	struct PointLight {
		glm::vec3 position;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;

		PointLight();
		PointLight(
			glm::vec3& _position
			, glm::vec3& _ambient
			, glm::vec3& _diffuse
			, glm::vec3& _specular
			, float _constant
			, float _linear
			, float _quadratic
		);
	};

	struct SpotLight {
		glm::vec3 position;
		glm::vec3 direction;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;
		float cutOff;
		float outerCutOff;

		SpotLight();
		SpotLight(
			glm::vec3& _position
			, glm::vec3& _direction
			, glm::vec3& _ambient
			, glm::vec3& _diffuse
			, glm::vec3& _specular
			, float _constant
			, float _linear
			, float _quadratic
			, float _cutOff
			, float _outerCutOff
		);
	};

}

#endif // !LIGHTS_H
