#pragma once
#ifndef SHADERS_H
#define SHADERS_H

#include "OpenGLheader.h"
#include "lights.h"

namespace GLEngine {

	struct CoordsTransMatrices {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat3 normals;

		CoordsTransMatrices();
		CoordsTransMatrices(
			glm::mat4& _model
			, glm::mat4& _view
			, glm::mat4& _projection
		);
		void updateMatrixNormals();
		void modelTranslate(glm::vec3& _where);
		void modelScale(glm::vec3& _to);

	};

	class Shader
	{
	protected:
		GLuint program;
	public:

		Shader();
		Shader(const char* _shader_name, std::string& _defines = std::string());
		~Shader();

		void Use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string &name, int value) const;
		void setUInt(const std::string &name, uint32_t value) const;
		void setFloat(const std::string &name, float value) const;
		void setVec3(const std::string &name, glm::vec3& value) const;
		void setVec4(const std::string &name, glm::vec4& value) const;
		void setMat3(const std::string &name, glm::mat3& value, GLsizei count = 1, GLboolean transpose = GL_FALSE) const;
		void setMat4(const std::string &name, glm::mat4& value, GLsizei count = 1, GLboolean transpose = GL_FALSE) const;
	};


	class PhongLightingShader 
		:public Shader {
	private:		
	public:		
		PhongLightingShader();
		PhongLightingShader(std::string& _defines);
		~PhongLightingShader();

		//«адать uniform переменные дл€ направленного источника освещени€
		void setDirLight(DirectLight& _light);

		//«адать uniform переменные дл€ i-того точечного источника освещени€
		void setPointLight(PointLight& _light, size_t _i = 0);
			
		//«адать uniform переменные дл€ i-того "прожектора"
		void setSpotLight(SpotLight& _light, size_t _i = 0);

		void setCameraPosition(glm::vec3& _pos);

		void setCoordsTransMatrices(CoordsTransMatrices& _matrices);

	};
}

#endif // !SHADERS_H
