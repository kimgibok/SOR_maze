#include "Engine.h"

using namespace ShaderNamespace1;


void Engine::Run()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	while (!glfwWindowShouldClose(glWindow))
	{
		std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
		Draw(); //draw frame
		Update(); //frame update
		std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();

		std::chrono::duration<float> duration = end - start;

		timeToDrawFrame = duration.count(); //frame update duration
	}
	glfwTerminate();
}

Engine::Engine()
{
	//Set a random seed
	srand(time(NULL));

	//GLFW
	if (!glfwInit()) exit(-1);
	std::cout << "Loading obj file: " << objFileName << std::endl;

	//window
	glWindow = glfwCreateWindow(1920, 1080, "maze", NULL, NULL);
	if (!glWindow)
	{
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(glWindow);
	glfwSwapInterval(1);

	//glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) exit(-1);

	//Set up the matrices
	//Set up the perspective matrix
	//FOV
	const float fov = 75.0f;
	//Screen dimensions
	int width, height;
	glfwGetWindowSize(glWindow, &width, &height);
	//Calculate the aspect ratio
	float aspect = (float)width / (float)height;
	perspectiveMatrix = glm::perspective(3.1415926535f / 180.0f * fov, aspect, 0.01f, 1000.0f);

	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Cube
	float cube[] =
	{
		-1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
	};
	//Set up vertex buffer, shader
	vb = VertexBuffer(&cube[0], sizeof(cube), 3, true);
	sh = ShaderNamespace2::Shader("build/shaders/basic-3d-vert.glsl", "build/shaders/basic-frag.glsl", false);
	rainbowSh = ShaderNamespace2::Shader("build/shaders/rainbow-vert.glsl", "build/shaders/rainbow-frag.glsl", false);
	ShaderNamespace1::Shader ourShader("build/shaders/basic-3d-vert.glsl", "build/shaders/basic-frag.glsl");
	ShaderNamespace1::Shader rainbow("build/shaders/rainbow-vert.glsl", "build/shaders/rainbow-frag.glsl");
	
	//load objfile
	Model ourModel(objFilePath);
	
	

	//Set up the camera
	cameraPosition = glm::vec3(0.0f, 0.0f, 1.0f);
	cameraMovement = glm::vec3(0.0f, 0.0f, 0.0f);

	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraDirection= glm::normalize(cameraPosition - cameraTarget);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

	//Set up key input
	glfwSetWindowUserPointer(glWindow, this);
	auto HandleKeyInputFunc = [](GLFWwindow* win, int key, int scancode, int action, int mods)
		{
			static_cast<Engine*>(glfwGetWindowUserPointer(win))->HandleKeyInput(win, key, scancode, action, mods);
		};
	glfwSetKeyCallback(glWindow, HandleKeyInputFunc);


	//set up mouse
	auto HandleMouseInputFunc = [](GLFWwindow* win, double xpos, double ypos)
		{
			static_cast<Engine*>(glfwGetWindowUserPointer(win))->mouse_callback(win, xpos, ypos);
		};
	glfwSetCursorPosCallback(glWindow, HandleMouseInputFunc);

	//Handle window resizing
	auto HandleResizeFunc = [](GLFWwindow* win, int newWidth, int newHeight)
		{
			static_cast<Engine*>(glfwGetWindowUserPointer(win))->HandleWindowResize(win, newWidth, newHeight);
		};
	glfwSetWindowSizeCallback(glWindow, HandleResizeFunc);

	GenerateMaze();
}

//Input objFilename and return
std::string Engine::GetObjFileNameFromUser() {
	std::string objFileName;
	std::cout << "Enter the obj file name: ";
	std::cin >> objFileName;
	return objFileName;
}


