#include "../headers/handler.h"

GLFWwindow * GLEngine::InitWindow()
{
	Settings::SettingsFileHandler fs;
	int width, height;
	std::string title;
	if (fs.ReadWidthHeightTitle(width, height, &title)) {
		width = 1280; height = 720;
		title = std::string("Window");
	}
	Screen() = __screen(width, height);

	GLFWwindow *win = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (win == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(win);
	InitGLEW();
	glfwSetKeyCallback(win, key_callback);
	glfwSetScrollCallback(win, scroll_callback);
	glfwSetCursorPosCallback(win, mouse_callback);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);
	return win;
}

int GLEngine::Handler()
{
	if (LOGS::InitLogFile()) {
		std::cout << "Couldn't open log file. Program will continue without logging." << std::endl;
		uint32_t delay = 0;
		while (delay < 1000000000) delay++;
	}

	if (InitOpenGL()) {
		std::cout << "Program failure. Check logs." << std::endl;
		uint32_t delay = 0;
		while (delay < 1000000000) delay++;
		LOGS::SaveLogFile();
		return -1;
	}

	Main_loop();

	LOGS::SaveLogFile();
	glfwTerminate();
	return 0;
}

int GLEngine::Main_loop()
{
	Window();
	Shader lightingShader = Shader("[LightingShader]");
	Shader lampShader = Shader("[LampShader]");

	__shape cube = __shape("[ShapeCube]");

	textureAtrib crateTex = createTexture("[CrateTexture]");;

	//__model crate = __model(&cube, glm::vec3(0.0f, 0.0f, -4.0f), glm::vec3(1.0f, 0.5f, 0.31f));
	__model lamp = __model(&cube, glm::vec3(2.5f, 0.0f, -3.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	__model crates[] = {
		__model(&cube, glm::vec3(0.0f, 0.0f, -4.0f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(2.0f,  5.0f, -15.0f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-1.5f, -2.2f, -2.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-3.8f, -2.0f, -12.3f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(3.4f, -0.4f, -3.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-1.7f,  3.0f, -7.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(1.3f, -2.0f, -2.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(1.5f,  2.0f, -2.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(1.5f,  0.2f, -1.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-1.3f,  1.0f, -1.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(0.0f, 1.0f, 3.0f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(2.0f,  5.0f, -10.0f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-1.5f, -7.2f, -1.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-4.8f, -2.0f, 5.3f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(8.4f, -7.4f, -3.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-1.7f,  3.0f, 7.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(1.3f, 2.0f, 2.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(2.5f,  5.0f, -3.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(2.5f,  -4.2f, -2.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
		__model(&cube, glm::vec3(-3.3f,  2.0f, 1.5f),glm::vec3(1.0f, 0.5f, 0.31f), crateTex),
	};
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	textureAtrib specMap = createTexture("[SpecMap]");
	Material mater = Material(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);

	/*Light movinglight = Light(glm::vec3(0.05f), glm::vec3(0.7f), glm::vec3(1.0f), 0.09f, 0.032f, 0.0f, 0.0f);

	Light light = Light(glm::vec3(0.2f), glm::vec3(0.5f), glm::vec3(1.0f));

	Light flashlight = Light(glm::vec3(0.05f), glm::vec3(0.8f), glm::vec3(1.0f), 0.045f, 0.0075f, glm::cos(glm::radians(12.5f)), glm::cos(glm::radians(17.5f)));*/

	glm::vec3 lightColor;

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -8.0f),
		glm::vec3(0.0f,  0.0f, 7.0f)
	};

	std::string uniformName, tempName;
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(Window()))
	{
		deltaTime().setDelta();

		glfwPollEvents();
		do_movement();
		do_rotation();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		{
			view = Camera().GetViewMatrix();
			projection = glm::perspective(glm::radians(Camera().Zoom), Screen().AspectRatio, 0.1f, 1000.0f);

			pointLightPositions[0] = glm::vec3(-10.0 + 12.0f*glm::cos(0.5*deltaTime().lastFrame), 0.0f, -4.0f - 4.0f*glm::sin(0.5*deltaTime().lastFrame));
			pointLightPositions[1] = glm::vec3(0.0f, -10.0 + 12.0f*glm::sin(0.5*deltaTime().lastFrame), -4.0f + 4.0f*glm::cos(0.5*deltaTime().lastFrame));
			pointLightPositions[2] = glm::vec3(-4.0f + 12.0f*glm::sin(deltaTime().lastFrame), 2.0f, -8.0f);
			pointLightPositions[3] = glm::vec3(0.0f + 12.0f*glm::cos(deltaTime().lastFrame), 0.0f, 7.0f);

			lightingShader.Use();
			lightingShader.setMat4("view", view);
			lightingShader.setMat4("projection", projection);
			lightingShader.setVec3("viewPos", Camera().Position);

			lightingShader.setInt("material.diffuse", 0);
			lightingShader.setInt("material.specular", 1);
			lightingShader.setFloat("material.shininess", 32.0f);

			glActiveTexture(crateTex.GL_sampler);
			glBindTexture(GL_TEXTURE_2D, crateTex.texture);
			glActiveTexture(specMap.GL_sampler);
			glBindTexture(GL_TEXTURE_2D, specMap.texture);
			//общий свет
			lightingShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
			if (!keys()[GLFW_KEY_H]) {
				lightingShader.setVec3("dirLight.ambient", glm::vec3());
				lightingShader.setVec3("dirLight.diffuse", glm::vec3());
				lightingShader.setVec3("dirLight.specular", glm::vec3());
			}
			else {
				lightingShader.setVec3("dirLight.ambient", glm::vec3(0.2f));
				lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.5f));
				lightingShader.setVec3("dirLight.specular", glm::vec3(1.0f));
			}
			//фонарик
			if (keys()[GLFW_KEY_F]) {
				lightingShader.setVec3("spotLight.ambient", glm::vec3(0.05f));
				lightingShader.setVec3("spotLight.diffuse", glm::vec3(0.8f));
				lightingShader.setVec3("spotLight.specular", glm::vec3(1.0f));
				lightingShader.setFloat("spotLight.constant", 1.0);
				lightingShader.setFloat("spotLight.linear", 0.045f);
				lightingShader.setFloat("spotLight.quadratic", 0.0075f);
				lightingShader.setVec3("spotLight.position", Camera().Position);
				lightingShader.setVec3("spotLight.direction", Camera().Front);
				lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
				lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
			}
			else {
				lightingShader.setVec3("spotLight.ambient", glm::vec3());
				lightingShader.setVec3("spotLight.diffuse", glm::vec3());
				lightingShader.setVec3("spotLight.specular", glm::vec3());
				lightingShader.setFloat("spotLight.constant", 1.0f);
				lightingShader.setFloat("spotLight.linear", 0.0f);
				lightingShader.setFloat("spotLight.quadratic", 0.0f);
				lightingShader.setVec3("spotLight.position", Camera().Position);
				lightingShader.setVec3("spotLight.direction", Camera().Front);
				lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
				lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
			}

			for (int i = 0; i < 4; i++) {
				uniformName = "pointLights[";
				uniformName.append(std::to_string(i));
				uniformName.append("].");
				tempName = uniformName;
				tempName.append("position");
				lightingShader.setVec3(tempName.c_str(), pointLightPositions[i]);
				if (!keys()[GLFW_KEY_G]) {
					tempName = uniformName;
					tempName.append("ambient");
					lightingShader.setVec3(tempName.c_str(), glm::vec3(0.05f));
					tempName = uniformName;
					tempName.append("diffuse");
					lightingShader.setVec3(tempName.c_str(), glm::vec3(0.7f));
					tempName = uniformName;
					tempName.append("specular");
					lightingShader.setVec3(tempName.c_str(), glm::vec3(1.0f));
					tempName = uniformName;
					tempName.append("constant");
					lightingShader.setFloat(tempName, 1.0f);
					tempName = uniformName;
					tempName.append("linear");
					lightingShader.setFloat(tempName, 0.09f);
					tempName = uniformName;
					tempName.append("quadratic");
					lightingShader.setFloat(tempName, 0.032f);
				}
				else {
					tempName = uniformName;
					tempName.append("ambient");
					lightingShader.setVec3(tempName.c_str(), glm::vec3());
					tempName = uniformName;
					tempName.append("diffuse");
					lightingShader.setVec3(tempName.c_str(), glm::vec3());
					tempName = uniformName;
					tempName.append("specular");
					lightingShader.setVec3(tempName.c_str(), glm::vec3());
					tempName = uniformName;
					tempName.append("constant");
					lightingShader.setFloat(tempName, 1.0f);
					tempName = uniformName;
					tempName.append("linear");
					lightingShader.setFloat(tempName, 0.0f);
					tempName = uniformName;
					tempName.append("quadratic");
					lightingShader.setFloat(tempName, 0.0f);
				}
			}

			glBindVertexArray(crates[0].VAO());
			for (unsigned int i = 0; i < 20; i++)
			{

				model = glm::mat4();
				model = glm::translate(model, crates[i].position);
				model = model* RotatingCube();
				float angle = 20.0f * i;
				model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
				lightingShader.setMat4("model", model);
				lightingShader.setMat3("matrix_normals", glm::inverse(glm::mat3(model)), 1, GL_TRUE);
				glDrawArrays(GL_TRIANGLES, 0, crates[i].VerticesNum());
			}
			glBindVertexArray(0);
			
			lampShader.Use();
			lampShader.setVec3("modelColor", lamp.color);
			lampShader.setMat4("view", view);
			lampShader.setMat4("projection", projection);

			glBindVertexArray(lamp.VAO());
			for (unsigned int i = 0; i < 4; i++)
			{
				model = glm::mat4();
				model = glm::translate(model, pointLightPositions[i]);	
				model = glm::scale(model, glm::vec3(0.1f));
				lampShader.setMat4("model", model);
				if (!keys()[GLFW_KEY_G])
					glDrawArrays(GL_TRIANGLES, 0, lamp.VerticesNum());
			}			
			glBindVertexArray(0);		

			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glfwSwapBuffers(Window());
	}
	return 0;
}
