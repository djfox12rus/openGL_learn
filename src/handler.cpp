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
	Shader lightingShader = Shader("[ModelShader]");
	Shader lampShader = Shader("[LampShader]");

	__model crate = __model("[CratesModel]");
	__model lamp = __model("[LightModel]");
		
	glm::vec3 cubePositions = glm::vec3(0.0f, 0.0f, -4.0f);
	glm::vec3 lampPositions = glm::vec3(2.5f, 1.0f, -3.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
	glm::mat4 model;

	glm::mat4 view;

	glm::mat4 projection;
	
	GLint modelLoc;
	GLint viewLoc;
	GLint projLoc;
	GLint modelColor;
	GLint lightColorLoc;
	GLint lightPosLoc;
	GLint viewPosLoc;
	//trans = projection*view*model;
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(Window()))
	{
		deltaTime().setDelta();
		
		glfwPollEvents();
		do_movement();
		do_rotation();
		{
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//RainbowBackground();

			//model = RotatingCube();	
			//model = glm::mat4();
			
			
			view = Camera().GetViewMatrix();
			projection = glm::perspective(glm::radians(Camera().Zoom), Screen().AspectRatio, 0.1f, 100.0f);

			lightingShader.Use();

			modelLoc = glGetUniformLocation(lightingShader.program, "model");
			viewLoc = glGetUniformLocation(lightingShader.program, "view");
			projLoc = glGetUniformLocation(lightingShader.program, "projection");


			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, crate.texture);*/


			modelColor = glGetUniformLocation(lightingShader.program, "modelColor");
			glUniform3f(modelColor, crate.color.r, crate.color.g, crate.color.b);
			lightColorLoc = glGetUniformLocation(lightingShader.program, "lightColor");
			glUniform3f(lightColorLoc, lamp.color.r, lamp.color.g, lamp.color.b);
					
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			
			lightPosLoc = glGetUniformLocation(lightingShader.program, "lightPos");
			glUniform3f(lightPosLoc, lampPositions.x, lampPositions.y, lampPositions.z);

			viewPosLoc = glGetUniformLocation(lightingShader.program, "viewPos");
			glUniform3f(viewPosLoc, Camera().Position.x, Camera().Position.y, Camera().Position.z);

			glBindVertexArray(crate.VAO);			
			model = glm::mat4();
			model = glm::translate(model, cubePositions);
			model = model* RotatingCube();					
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			lampShader.Use();
			modelLoc = glGetUniformLocation(lampShader.program, "model");
			viewLoc = glGetUniformLocation(lampShader.program, "view");
			projLoc = glGetUniformLocation(lampShader.program, "projection");
			modelColor = glGetUniformLocation(lampShader.program, "modelColor");
			glUniform3f(modelColor, lamp.color.r, lamp.color.g, lamp.color.b);
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(lamp.VAO);

			model = glm::mat4();
			model = glm::translate(model, lampPositions);			
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			glDrawArrays(GL_TRIANGLES, 0, 36);

			
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);


		}
		glfwSwapBuffers(Window());
	}
	return 0;
}
