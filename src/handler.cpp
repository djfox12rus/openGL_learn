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

	__shape cube = __shape("[ShapeCube]");

	__model crate = __model(&cube,glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.0f, 0.0f, -4.0f));
	__model lamp = __model(&cube, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.5f, 0.0f, -3.0f));
			
	crate.SetTexture("[CrateTexture]");
	
	glm::mat4 model;

	glm::mat4 view;

	glm::mat4 projection;

	textureAtrib specMap = createTexture("[SpecMap]");
	
	Material mater = Material(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);
	Light light = Light(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

	glm::vec3 lightColor;
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
			
			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, crate.texture);*/

					
			lightingShader.setMat4("view", view);
			lightingShader.setMat4("projection", projection);			
			
			lightingShader.setVec3("viewPos", Camera().Position);

			lightingShader.setVec3("material.ambient", mater.ambient);
			lightingShader.setInt("material.diffuse", 0);
			lightingShader.setInt("material.specular", 1);
			lightingShader.setFloat("material.shininess", mater.shininess);

			lightingShader.setVec3("light.position", lamp.position);
			lightingShader.setVec3("light.ambient", light.ambient);
			lightingShader.setVec3("light.diffuse", light.diffuse);
			lightingShader.setVec3("light.specular", light.specular);

			glActiveTexture(crate.Texture().GL_sampler);
			glBindTexture(GL_TEXTURE_2D, crate.Texture().texture);


			glActiveTexture(specMap.GL_sampler);
			glBindTexture(GL_TEXTURE_2D, specMap.texture);

			glBindVertexArray(crate.VAO());			
			model = glm::mat4();
			model = glm::translate(model, crate.position);
			model = model* RotatingCube();			
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			lampShader.Use();			

			lampShader.setVec3("modelColor", lamp.color);
			lampShader.setMat4("view", view);
			lampShader.setMat4("projection", projection);		

			glBindVertexArray(lamp.VAO());

			model = glm::mat4();
			model = glm::translate(model, lamp.position);			
			model = glm::scale(model, glm::vec3(0.2f));
			lampShader.setMat4("model", model);
			

			glDrawArrays(GL_TRIANGLES, 0, 36);

			lamp.position = glm::vec3(2.0f*glm::sin(deltaTime().lastFrame), 0.0f, -4.0f -2.0f*glm::cos(deltaTime().lastFrame));
			//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
			//glDrawArrays(GL_TRIANGLES, 0, 36);

			glBindVertexArray(0);


		}
		glfwSwapBuffers(Window());
	}
	return 0;
}
