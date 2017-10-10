#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <vector>
#include "OpenGLheader.h"

namespace GLEngine {

	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};


	const GLfloat YAW = -90.0f;
	const GLfloat PITCH = 0.0f;
	const GLfloat ROLL = 0.0f;
	const GLfloat SPEED = 3.0f;
	const GLfloat SENSITIVTY = 0.05f;
	const GLfloat ZOOM = 45.0f;


	class __camera
	{
	private:
		__camera();
		void updateCameraVectors();
	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		GLfloat Yaw;
		GLfloat Pitch;
		GLfloat Roll;

		GLfloat MovementSpeed;
		GLfloat MouseSensitivity;
		GLfloat Zoom;

		__camera(glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat _yaw = YAW, GLfloat _pitch = PITCH, GLfloat _roll = PITCH);
		__camera(GLfloat _posX, GLfloat _posY, GLfloat _posZ, GLfloat _upX, GLfloat _upY, GLfloat _upZ, GLfloat _yaw, GLfloat _pitch, GLfloat _roll);

		~__camera();

		glm::mat4 GetViewMatrix();
		void ProcessKeyboard(Camera_Movement _direction, GLfloat _deltaTime);
		void ProcessMouseMovement(GLfloat _xoffset, GLfloat _yoffset, GLboolean _constrainPitch = true);
		void ProcessMouseScroll(GLfloat _yoffset);
	};

	inline __camera& Camera() {
		static __camera cam = __camera(glm::vec3(0.0f, 0.0f, 3.0f));
		return cam;
	 }

}


#endif // !CAMERA_H


