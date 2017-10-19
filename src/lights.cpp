#include "..\headers\lights.h"

GLEngine::DirectLight::DirectLight():
	direction()
	, ambient()
	, diffuse()
	, specular()
{
}

GLEngine::DirectLight::DirectLight(glm::vec3 & _direction, glm::vec3 & _ambient, glm::vec3 & _diffuse, glm::vec3 & _specular):
	direction(_direction)
	, ambient(_ambient)
	, diffuse(_diffuse)
	, specular(_specular)
{
}

GLEngine::PointLight::PointLight():
	position()
	, ambient()
	, diffuse()
	, specular()
	, constant(1.0f)
	, linear()
	, quadratic()
{
}

GLEngine::PointLight::PointLight(glm::vec3 & _position, glm::vec3 & _ambient, glm::vec3 & _diffuse, glm::vec3 & _specular, float _constant, float _linear, float _quadratic):
	position (_position)
	, ambient (_ambient)
	, diffuse (_diffuse)
	, specular(_specular)
	, constant (_constant)
	, linear (_linear)
	, quadratic (_quadratic)
{
}

GLEngine::SpotLight::SpotLight():
	position()
	, direction()
	, ambient()
	, diffuse()
	, specular()
	, constant(1.0f)
	, linear()
	, quadratic()
	, cutOff()
	, outerCutOff()
{
}

GLEngine::SpotLight::SpotLight(glm::vec3 & _position, glm::vec3 & _direction, glm::vec3 & _ambient, glm::vec3 & _diffuse, glm::vec3 & _specular, float _constant, float _linear, float _quadratic, float _cutOff, float _outerCutOff):
	position(_position)
	, direction(_direction)
	, ambient(_ambient)
	, diffuse(_diffuse)
	, specular(_specular)
	, constant(_constant)
	, linear(_linear)
	, quadratic(_quadratic)
	, cutOff(_cutOff)
	, outerCutOff(_outerCutOff)
{
}
