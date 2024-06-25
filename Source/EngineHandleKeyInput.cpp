#include "Engine.h"

//Handlekeyinput to cameramovement
void Engine::HandleKeyInput(GLFWwindow* win, int key, int scancode, int action, int mods) {
	float forward = 0.0f;
	float right = 0.0f;

	//movement via WASD
	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) forward += 1.0f;
	if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) forward -= 1.0f;
	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) right -= 1.0f;
	if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) right += 1.0f;

	cameraMovement.x = forward * playerSpeed * glm::sin(cameraRotation) + right * playerSpeed * glm::cos(cameraRotation);
	cameraMovement.z = forward * playerSpeed * -glm::cos(cameraRotation) + right * playerSpeed * glm::sin(cameraRotation);
	
	//jump
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_SPACE:
			if (cameraPosition.y <= 2.5f)
				cameraMovement.y = 10.0f;
			break;
		}
	}

	//Closewindow
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(glWindow, GLFW_TRUE);
	}
}

	//mousecallback to camerarotation
	void Engine::mouse_callback(GLFWwindow* win, double xpos, double ypos) {

		static double lastX = 400, lastY = 300; //Initialize
		static bool firstMouse = true;

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX; //set up xoffset, yoffset
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.005; //sensitivity
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		cameraRotation += xoffset;
		camerapitchRotation += yoffset;
		//std::cout << camerapitchRotation << std::endl;
		
		//set up cameraRoatiation range
		if (camerapitchRotation > 0.55f) {
			camerapitchRotation = 0.55f;
		}

		if (camerapitchRotation < -0.8f) {
			camerapitchRotation = -0.8f;
		}

		
	}




