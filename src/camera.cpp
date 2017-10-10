#include "../headers/camera.h"

/*glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		GLfloat Yaw;
		GLfloat Pitch;
		GLfloat Roll;

		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom;*/


GLEngine::__camera::__camera()
{
}

void GLEngine::__camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}


GLEngine::__camera::__camera(glm::vec3 _position, glm::vec3 _up, GLfloat _yaw, GLfloat _pitch, GLfloat _roll):Position(_position),Front (0.0f, 0.0f, -1.0f),Up(), Right(), WorldUp(_up),Yaw(_yaw),Pitch(_pitch),Roll(_roll), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	this->updateCameraVectors();
}

GLEngine::__camera::__camera(GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _upX, GLfloat _upY, GLfloat _upZ, GLfloat _yaw, GLfloat _pitch, GLfloat _roll) : Position(_posX, _posY, _posZ), Front(0.0f, 0.0f, -1.0f), Up(), Right(), WorldUp(_upX, _upY, _upZ), Yaw(_yaw), Pitch(_pitch), Roll(_roll), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	this->updateCameraVectors();
}

GLEngine::__camera::~__camera()
{
}

glm::mat4 GLEngine::__camera::GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

void GLEngine::__camera::ProcessKeyboard(Camera_Movement _direction, GLfloat _deltaTime)
{
	GLfloat velocity = this->MovementSpeed * _deltaTime;
	if (_direction == FORWARD)
		this->Position += this->Front * velocity;
	if (_direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (_direction == LEFT)
		this->Position -= this->Right * velocity;
	if (_direction == RIGHT)
		this->Position += this->Right * velocity;
}

void GLEngine::__camera::ProcessMouseMovement(GLfloat _xoffset, GLfloat _yoffset, GLboolean _constrainPitch)
{
	_xoffset *= this->MouseSensitivity;
	_yoffset *= this->MouseSensitivity;

	this->Yaw += _xoffset;
	this->Pitch += _yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

void GLEngine::__camera::ProcessMouseScroll(GLfloat _yoffset)
{
	if (this->Zoom >= 1.0f && this->Zoom <= ZOOM)
		this->Zoom -= _yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= ZOOM)
		this->Zoom = ZOOM;
	std::cout << this->Zoom << std::endl;
}


