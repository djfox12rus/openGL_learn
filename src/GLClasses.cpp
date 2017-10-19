#include "../headers/GLClasses.h"

GLEngine::__keys::__keys_8::__keys_8() :key_0(false), key_1(false), key_2(false), key_3(false), key_4(false), key_5(false), key_6(false), key_7(false)
{
}

bool GLEngine::__keys::operator[](int _place)
{
	if (_place<0 || _place >= this->size * 8)return false;
	int byte_pos = _place / 8;
	int bit_pos = _place % 8;
	switch (bit_pos)
	{
	case 0:return this->keys_array[byte_pos].key_0;
	case 1:return this->keys_array[byte_pos].key_1;
	case 2:return this->keys_array[byte_pos].key_2;
	case 3:return this->keys_array[byte_pos].key_3;
	case 4:return this->keys_array[byte_pos].key_4;
	case 5:return this->keys_array[byte_pos].key_5;
	case 6:return this->keys_array[byte_pos].key_6;
	case 7:return this->keys_array[byte_pos].key_7;
	default:return false;
		break;
	}
	return false;
}

void GLEngine::__keys::set_at(int _place)
{
	if (_place<0 || _place >= this->size * 8)return;
	int byte_pos = _place / 8;
	int bit_pos = _place % 8;
	switch (bit_pos)
	{
	case 0:this->keys_array[byte_pos].key_0 = true;
		break;
	case 1:this->keys_array[byte_pos].key_1 = true;
		break;
	case 2:this->keys_array[byte_pos].key_2 = true;
		break;
	case 3:this->keys_array[byte_pos].key_3 = true;
		break;
	case 4:this->keys_array[byte_pos].key_4 = true;
		break;
	case 5:this->keys_array[byte_pos].key_5 = true;
		break;
	case 6:this->keys_array[byte_pos].key_6 = true;
		break;
	case 7:this->keys_array[byte_pos].key_7 = true;
		break;
	default:return;
		break;
	}
}

void GLEngine::__keys::reset_at(int _place)
{
	if (_place<0 || _place >= this->size * 8)return;
	int byte_pos = _place / 8;
	int bit_pos = _place % 8;
	switch (bit_pos)
	{
	case 0:this->keys_array[byte_pos].key_0 = false;
		break;
	case 1:this->keys_array[byte_pos].key_1 = false;
		break;
	case 2:this->keys_array[byte_pos].key_2 = false;
		break;
	case 3:this->keys_array[byte_pos].key_3 = false;
		break;
	case 4:this->keys_array[byte_pos].key_4 = false;
		break;
	case 5:this->keys_array[byte_pos].key_5 = false;
		break;
	case 6:this->keys_array[byte_pos].key_6 = false;
		break;
	case 7:this->keys_array[byte_pos].key_7 = false;
		break;
	default:return;
		break;
	}
}




GLEngine::__mouse_pos::__mouse_pos()
{
	int width, height;
	Settings::SettingsFileHandler fs;
	fs.ReadWidthHeightTitle(width, height);
	this->x = width / 2;
	this->y = height / 2;
}

GLEngine::__deltaTime::__deltaTime():delta(), lastFrame()
{
}

void GLEngine::__deltaTime::setDelta()
{
	GLfloat currentFrame = glfwGetTime();
	delta = currentFrame - lastFrame;	
	lastFrame = currentFrame;
}

GLEngine::__deltaTime::operator GLfloat()
{
	return delta;
}

GLEngine::__rotation::__rotation():x(),y(),z()
{
}

GLEngine::__screen::__screen() : Width(), Height(), AspectRatio()
{
}

GLEngine::__screen::__screen(GLuint _w, GLuint _h): Width(_w), Height(_h), AspectRatio((GLfloat)_w/(GLfloat)_h)
{	
}

GLEngine::Light::Light(glm::vec3 &_amb, glm::vec3 &_diff, glm::vec3 &_spec): ambient(_amb), diffuse(_diff), specular(_spec), constant(), linear(), quadratic(), cutOff()
{
}

GLEngine::Light::Light(glm::vec3 & _amb, glm::vec3 & _diff, glm::vec3 & _spec, float _linear, float _quadratic, float _cutOff, float _outerCutOff, float _constant):ambient(_amb), diffuse(_diff), specular(_spec), constant(_constant), linear(_linear), quadratic(_quadratic), cutOff(_cutOff),outerCutOff(_outerCutOff)
{
}
