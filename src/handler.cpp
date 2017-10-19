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
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
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
	//Shader lightingShader = Shader("PhongLightingShader");
	Shader lampShader = Shader("LampShader");

	PhongLightingShader lightingShader = PhongLightingShader(std::string("#define NR_POINT_LIGHTS 4 \n #define NR_SPOT_LIGHTS 1 \n"));

	//Model nanosuit = Model("[nanosuitMod]");
	Model spherelamp = Model("[Sphere]");
	//Model plane = Model("[ShapeFloor]");
	Model crates = Model("[TwoCrates]");

	DirectLight DirLight = DirectLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.1f), glm::vec3(0.3f), glm::vec3(1.0f));
	SpotLight flashLight = SpotLight(
		Camera().Position
		, Camera().Front
		, glm::vec3(0.05f)
		, glm::vec3(0.8f)
		, glm::vec3(1.0f)
		, 1.0
		, 0.045f
		, 0.0075f
		, glm::cos(glm::radians(12.5f))
		, glm::cos(glm::radians(17.5f)));

	PointLight pointLights[] = {
		PointLight(glm::vec3(0.7f,  0.2f,  2.0f), glm::vec3(0.05f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f)
		, PointLight(glm::vec3(2.3f, -3.3f, -4.0f),glm::vec3(0.05f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.0225f, 0.0019f)
		, PointLight(glm::vec3(-4.0f,  2.0f, -8.0f),glm::vec3(0.05f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f)
		, PointLight(glm::vec3(1.0f,  2.0f, 0.0f),glm::vec3(0.05f), glm::vec3(0.5f), glm::vec3(1.0f), 1.0f, 0.09f, 0.032f)

	};

	CoordsTransMatrices mats;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_DEPTH_TEST);
	glfwShowWindow(Window());
	while (!glfwWindowShouldClose(Window()))
	{
		GLEngine::logErrors(std::string("GLEngine::Main_loop"));
		deltaTime().setDelta();
		glfwPollEvents();
		do_movement();
		do_rotation();
		glClearColor(0.05f, 0.05f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			/*mats = CoordsTransMatrices(
				glm::mat4()
				, Camera().GetViewMatrix()
				, glm::perspective(glm::radians(Camera().Zoom), Screen().AspectRatio, 0.1f, 1000.0f));*/
			mats.model = glm::mat4();
			mats.view = Camera().GetViewMatrix();
			mats.projection = glm::perspective(glm::radians(Camera().Zoom), Screen().AspectRatio, 0.1f, 1000.0f);

			flashLight.position = Camera().Position;
			flashLight.direction = Camera().Front;

			pointLights[0].position = glm::vec3(-10.0 + 12.0f*glm::cos(0.5*deltaTime().lastFrame), 0.0f, -4.0f - 4.0f*glm::sin(0.5*deltaTime().lastFrame));

			pointLights[1].position = glm::vec3(10.0f*glm::sin(deltaTime().lastFrame), 4.0f+ 3.0*glm::sin(0.25*deltaTime().lastFrame), 10.0f*glm::cos(deltaTime().lastFrame));
			pointLights[2].position = glm::vec3(-4.0f + 12.0f*glm::sin(deltaTime().lastFrame), 2.0f, -6.0f);
			//pointLights[3].position = glm::vec3(0.0f + 12.0f*glm::cos(deltaTime().lastFrame), 0.0f, 2.0f);


			lightingShader.Use();
			lightingShader.setCameraPosition(Camera().Position);

			//общий свет			
			if (keys()[GLFW_KEY_H])
				lightingShader.setDirLight(DirLight);
			else
				lightingShader.setDirLight(DirectLight());

			//фонарик
			if (keys()[GLFW_KEY_F]) {
				lightingShader.setSpotLight(flashLight);
			}
			else {
				lightingShader.setSpotLight(SpotLight());
			}


			for (int i = 0; i < 4; i++) {
				if (!keys()[GLFW_KEY_G]) {
					lightingShader.setPointLight(pointLights[i], i);
				}
				else {
					lightingShader.setPointLight(PointLight(), i);
				}
			}

			mats.modelTranslate(glm::vec3(0.0f, -2.0f, 0.0f));
			mats.updateMatrixNormals();
			lightingShader.setCoordsTransMatrices(mats);

			crates.Draw(lightingShader);
			
						
			/*mats.model = glm::mat4();
			mats.modelTranslate(glm::vec3(0.0f, -2.0f, -4.0f));
			mats.modelScale(glm::vec3(0.2f, 0.2f, 0.2f));
			mats.updateMatrixNormals();
			lightingShader.setCoordsTransMatrices(mats);

			
			nanosuit.Draw(lightingShader);*/




			lampShader.Use();

			lampShader.setMat4("matrix_view", mats.view);
			lampShader.setMat4("matrix_projection", mats.projection);

			for (unsigned int i = 0; i < 4; i++)
			{
				lampShader.setVec3("modelColor", pointLights[i].specular);
				mats.model = glm::mat4();
				mats.modelTranslate(pointLights[i].position);
				mats.modelScale(glm::vec3(0.1f));
				//mats.updateMatrixNormals();
				lampShader.setMat4("matrix_model", mats.model);
				if (!keys()[GLFW_KEY_G])
					spherelamp.Draw(lampShader);
			}

		}


		glfwSwapBuffers(Window());

		

	}
	return 0;
}
