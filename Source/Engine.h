#include "Shader.h"
#include "VertexBuffer.h"
#include <unordered_map>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <vector>
#include <stack>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "loadobj.h"
#include "learnshader.h"


class Engine
{

	GLFWwindow* glWindow;

	//vertexbuffer, shader, objModel
	VertexBuffer vb;
	ShaderNamespace2::Shader sh;
	ShaderNamespace2::Shader rainbowSh;
	ShaderNamespace1::Shader ourShader;
	ShaderNamespace1::Shader rainbow;
	Model ourModel;

	//input obj filename
	std::string GetObjFileNameFromUser();
	std::string objFileName = GetObjFileNameFromUser();
	std::string objFilePath = "build/obj/" + objFileName;

	//matrix
	glm::mat4 perspectiveMatrix;

	//camera
	glm::vec3 cameraPosition;
	glm::vec3 cameraDirection;
	glm::vec3 cameraTarget;
	glm::vec3 up;
	glm::vec3 cameraRight;
	glm::vec3 cameraUp;
	glm::vec3 cameraFront;
	glm::mat4 view;

	//cameraRotation, Movement
	float yaw;
	float pitch;
	glm::vec3 cameraMovement;
	float cameraRotation = 0.0f, rotationSpeed = 0.0f, camerapitchRotation = 0.0f, pitchrotationSpeed = 0.0f;

	//frame
	float timeToDrawFrame = 1.0f;

	//maze,coin=obj,exit
	std::vector<glm::vec3> maze; 
	std::vector<glm::vec3> coins; 
	glm::vec3 exitPos; 

	//mapsize
	const float wallSize = 2.5f;
	const float mapXSize = 32.0f * wallSize, mapZSize = 32.0f * wallSize;
	const float playerSpeed = 10.0f, playerRotationSpeed = 1.6f;


	float theta = 0.0f;

	void Draw();
	void Update();

	//input key,mouse
	void HandleKeyInput(GLFWwindow* win, int key, int scancode, int action, int mods);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	//windowsize
	void HandleWindowResize(GLFWwindow* win, int newWidth, int newHeight);

	bool ReadLevelFile(std::string filename);

	//generateMaze
	void GenerateMaze();



public:
	void Run();

	Engine();
};
