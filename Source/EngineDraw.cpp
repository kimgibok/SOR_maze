#include "Engine.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "loadobj.h"
#include "learnshader.h"

//for cameraRotation yaw,pitch
float yaw;
float pitch;

void Engine::Draw()
{
	//background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//Clear
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(sh.GetId());

	//camera Rotation matrix
	glm::mat4 rotationPitchMatrix = glm::mat4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, glm::cos(camerapitchRotation), -glm::sin(camerapitchRotation), 0.0f,
		0.0f, glm::sin(camerapitchRotation), glm::cos(camerapitchRotation), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	glm::mat4 rotationYawMatrix = glm::mat4(
		glm::cos(cameraRotation), 0.0f, -glm::sin(cameraRotation), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		glm::sin(cameraRotation), 0.0f, glm::cos(cameraRotation), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	//matrix
	glm::mat4 viewMatrix = glm::translate(glm::mat4(1.0f), -cameraPosition);
	glm::mat4 rotationViewMatrix = rotationPitchMatrix * rotationYawMatrix;
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(mapXSize, 1.0f, mapZSize));
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));
	glm::mat4 modelViewMatrix = rotationViewMatrix * modelMatrix * viewMatrix * scaleMatrix;
	
	//ground
	glUniformMatrix4fv(sh.GetUniformLocation("u_Perspective"), 1, false, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(sh.GetUniformLocation("u_ModelViewMatrix"), 1, false, glm::value_ptr(modelViewMatrix));
	glUniform4f(sh.GetUniformLocation("u_Color"), 0.0f, 0.0f, 0.0f, 1.0f);


	glDrawArrays(GL_TRIANGLES, 0, vb.VertexCount());

	//for item rotation
	glm::mat4 rotX = glm::mat4
	(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, glm::cos(theta), -glm::sin(theta), 0.0f,
		0.0f, glm::sin(theta), glm::cos(theta), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	glm::mat4 rotY = glm::mat4(
		glm::cos(theta), 0.0f, glm::sin(theta), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-glm::sin(theta), 0.0f, glm::cos(theta), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	glm::mat4 rotZ = glm::mat4
	(
		glm::cos(theta), -glm::sin(theta), 0.0f, 0.0f,
		glm::sin(theta), glm::cos(theta), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);

	//wall
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(wallSize, 3.0f, wallSize));
	for (auto w : maze)
	{
		modelMatrix = glm::translate(glm::mat4(1.0f), w);
		modelViewMatrix = rotationViewMatrix * modelMatrix * viewMatrix * scaleMatrix;

		glUniformMatrix4fv(sh.GetUniformLocation("u_ModelViewMatrix"), 1, false, glm::value_ptr(modelViewMatrix));
		glUniform4f(sh.GetUniformLocation("u_Color"), 1.0f, 1.0f, 1.0f, 1.0f);

		glDrawArrays(GL_TRIANGLES, 0, vb.VertexCount());
	}


	//Draw obj
	Model ourModel(objFilePath);
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	glUseProgram(ourShader.ID);
	for (auto c : coins)
	{
		modelMatrix = glm::translate(glm::mat4(1.0f), c);
		modelViewMatrix = rotationViewMatrix * modelMatrix * viewMatrix * scaleMatrix * rotY;

		// Apply uniforms 
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "u_ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniform4f(glGetUniformLocation(ourShader.ID, "u_Color"), 0.4f, 0.4f, 0.4f, 1.0f);

		ourShader.setMat4("model", modelMatrix);
		ourModel.Draw(ourShader);
	}


	//draw exit

	glUseProgram(rainbow.ID);

	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.4f, 0.4f, 0.4f));
	modelMatrix = glm::translate(glm::mat4(1.0f), exitPos);
	modelViewMatrix = rotationViewMatrix * modelMatrix * viewMatrix * scaleMatrix * rotX * rotZ; 

	glUniformMatrix4fv(glGetUniformLocation(rainbow.ID, "u_Perspective"), 1, GL_FALSE, glm::value_ptr(perspectiveMatrix));
	glUniformMatrix4fv(glGetUniformLocation(rainbow.ID, "u_ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelViewMatrix));


	glDrawArrays(GL_TRIANGLES, 0, vb.VertexCount());

	//swap buffer
	glfwSwapBuffers(glWindow);
}


