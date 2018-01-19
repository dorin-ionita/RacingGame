#include "Laborator6.h"

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>

#include <Core/Engine.h>
#define OFFSET_MACH_PLACE 10

using namespace std;

char my_data[1000];
int last_x_road = -30;
int last_y_road = -30;
int old_last_x_road = 0;
int old_last_y_road = 0;
int cur_idx_string = 0;

float general_angle;
float offset;

float old_y;
float old_x;

float ROTATE_ANGLE;

GLenum polygonMode;

float points_collisions;
float set = 100;

bool fps;

Laborator6::Laborator6()
{
}

Laborator6::~Laborator6()
{
}

void Laborator6::Init()
{
	//Camera

	camera = new Laborator::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
	//


	ROTATE_ANGLE = 30;
	polygonMode = GL_FILL;
	{
		FILE *input_file;
		input_file = fopen("input.txt", "r");
		if (input_file == NULL)
			cout << "Can't open the input data file.";
		else
			fgets(my_data, 1000, input_file);
		fclose(input_file);
	} //Read data from file

	//cout << "Data in file is";
	//puts(my_data);

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	//Create a simple cube
	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  0), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0)),
			VertexFormat(glm::vec3(1, -1,  0), glm::vec3(1, 0, 0), glm::vec3(0.9, 0.4, 0)),
			VertexFormat(glm::vec3(-1,  1,  0), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0)),
			VertexFormat(glm::vec3(1,  1,  0), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0)),
			VertexFormat(glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0), glm::vec3(0.3, 0.5, 0)),
			VertexFormat(glm::vec3(1, -1, 0), glm::vec3(0, 1, 0), glm::vec3(0.5, 0.2, 0)),
			VertexFormat(glm::vec3(-1,  1, 0), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0)),
			VertexFormat(glm::vec3(1,  1, 0), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.5, 0)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("cube", vertices, indices);
	}

	{
		Mesh* mesh = new Mesh("roata");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "oildrum.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		vector<VertexFormat> vertices
		{
			VertexFormat(glm::vec3(-1, -1,  0), glm::vec3(0, 1, 0), glm::vec3(0.2, 0.8, 0)),
			VertexFormat(glm::vec3(1, -1,  0), glm::vec3(1, 0, 0), glm::vec3(0.9, 0.4, 0)),
			VertexFormat(glm::vec3(-1,  1,  0), glm::vec3(1, 0, 0), glm::vec3(0.7, 0.7, 0)),
			VertexFormat(glm::vec3(1,  1,  0), glm::vec3(0, 1, 0), glm::vec3(0.7, 0.3, 0)),
			VertexFormat(glm::vec3(-1, -1, 0), glm::vec3(1, 1, 0), glm::vec3(0.3, 0.5, 0)),
			VertexFormat(glm::vec3(1, -1, 0), glm::vec3(0, 1, 0), glm::vec3(0.5, 0.2, 0)),
			VertexFormat(glm::vec3(-1,  1, 0), glm::vec3(1, 1, 0), glm::vec3(0.7, 0.0, 0)),
			VertexFormat(glm::vec3(1,  1, 0), glm::vec3(0, 0, 0), glm::vec3(0.1, 0.5, 0)),
		};

		vector<unsigned short> indices =
		{
			0, 1, 2,		1, 3, 2,
			2, 3, 7,		2, 7, 6,
			1, 7, 3,		1, 5, 7,
			6, 7, 4,		7, 5, 4,
			0, 4, 1,		1, 4, 5,
			2, 6, 4,		0, 2, 4,
		};

		CreateMesh("road", vertices, indices);
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader *shader = new Shader("ShaderLab6");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader *shader = new Shader("ShaderTerrain");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShaderTerrain.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShaderTerrain.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Shader *shader = new Shader("ShaderRoad");
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/VertexShaderRoad.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator6/Shaders/FragmentShaderRoad.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

Mesh* Laborator6::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Laborator6::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator6::Update(float deltaTimeSeconds)
{
	if (fps) {
		float cameraX = 0;
		float cameraZ = -1;
		float cameraY = 1;
		GetSceneCamera()->SetPosition(glm::vec3(cameraX, cameraY, cameraZ));
	}
	else {
		float cameraX = 0;
		float cameraZ = 3;
		float cameraY = 2;
		GetSceneCamera()->SetPosition(glm::vec3(cameraX, cameraY, cameraZ));
	}

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);
	{
		//the car
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(general_angle), glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.55f, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f + ROTATE_ANGLE), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		//modelMatirx = glm::translate(modelMatrix, glm::vec3(offset, 0, 0))
		RenderSimpleMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
		// the wheels
		modelMatrix = glm::mat4(1);
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(general_angle), glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5, 0.15, 0.25));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
		//cout << ROTATE_ANGLE;
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f + ROTATE_ANGLE), glm::vec3(0, 0, 1));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		RenderSimpleMesh(meshes["roata"], shaders["VertexNormal"], modelMatrix);

		modelMatrix = glm::mat4(1);
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(general_angle), glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5, 0.15, -0.25));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f + ROTATE_ANGLE), glm::vec3(0, 0, 1));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		RenderSimpleMesh(meshes["roata"], shaders["VertexNormal"], modelMatrix);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(+0.1, 0.15, 0.25));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
		//cout << ROTATE_ANGLE;
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f + ROTATE_ANGLE), glm::vec3(0, 0, 1));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		RenderSimpleMesh(meshes["roata"], shaders["VertexNormal"], modelMatrix);

		modelMatrix = glm::mat4(1);
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(general_angle), glm::vec3(0, 1, 0));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(+0.1, 0.15, -0.25));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f + ROTATE_ANGLE), glm::vec3(0, 0, 1));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 1, 0));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(0, 0, 1));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5));
		RenderSimpleMesh(meshes["roata"], shaders["VertexNormal"], modelMatrix);
	}
	//{
	//	glm::mat4 modelMatrix = glm::mat4(1);
	//	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	//	modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
	//	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	//	RenderSimpleMesh(meshes["oildrum"], shaders["ShaderLab6"], modelMatrix);
	//}

	//{
	//	glm::mat4 modelMatrix = glm::mat4(1);
	//	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	//	modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
	//	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f));
	//	RenderSimpleMesh(meshes["parapet"], shaders["ShaderLab6"], modelMatrix);
	//}


	//{
	//	glm::mat4 modelMatrix = glm::mat4(1);
	//	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	//	modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
	//	modelMatrix = glm::scale(modelMatrix, glm::vec3(90.0f));
	//	RenderSimpleMesh(meshes["box"], shaders["ShaderLab6"], modelMatrix);
	//}

	{
		glm::mat4 modelMatrix = glm::mat4(1);
		//modelMatrix = glm::translate(modelMatrix, glm::vec3(2, 0, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1000.0f));
		modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
		//modelMatrix = glm::rotate(modelMatrix, RADIANS(general_angle), glm::vec3(0, 0, 15));
		RenderSimpleMesh(meshes["cube"], shaders["ShaderTerrain"], modelMatrix);
	}


	{
		for (int i = cur_idx_string; i < strlen(my_data); i += 2) {
			old_last_x_road = last_x_road;
			old_last_y_road = last_y_road;
			
			if (my_data[i] == '1') {
				//cout << 1;
				last_x_road += 2;
			}
			else if (my_data[i] == '2') {
				//cout << 2;
				last_y_road -= 2;
			}
			else if (my_data[i] == '3') {
				//cout << 3;
				last_y_road += 2;
			}
			else if (my_data[i] == '4') {
				//cout << 4;
				last_x_road -= 2;
			}

			{
				//cout << "AICI";
				//cout << last_x_road << " " << last_y_road << endl;
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(general_angle), glm::vec3(0, 1, 0));
				modelMatrix = glm::translate(modelMatrix, glm::vec3(-old_x - OFFSET_MACH_PLACE, 0, old_y));
				modelMatrix = glm::translate(modelMatrix, glm::vec3(last_x_road, 0.01f, last_y_road));
				modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(3.0f));
				RenderSimpleMesh(meshes["road"], shaders["ShaderRoad"], modelMatrix);

				if (i % 12 == 2) {
					glm::mat4 modelMatrix = glm::mat4(1);
					modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.01f, 0));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(general_angle), glm::vec3(0, 1, 0));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(-old_x - OFFSET_MACH_PLACE, 0, old_y));
					modelMatrix = glm::translate(modelMatrix, glm::vec3(last_x_road, 0.01f, last_y_road));
					modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
					modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
					RenderSimpleMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);
					if (sqrt(pow((-old_x - OFFSET_MACH_PLACE + last_x_road), 2) + pow((old_y + last_y_road), 2)) < 1.5) {
						if (!set) {
							points_collisions += 10;
							cout << "POINTS: " << points_collisions << endl;
							set = 50;
						}
					}
				}
			}


		}
	}
	// Camera

	//

	last_x_road = -30;
	last_y_road = -30;
}

void Laborator6::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator6::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int location = glGetUniformLocation(shader->program, "Model");

	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// TODO : get shader location for uniform mat4 "View"
	location = glGetUniformLocation(shader->program, "View");

	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));


	// TODO : get shader location for uniform mat4 "Projection"
	location = glGetUniformLocation(shader->program, "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	float time = Engine::GetElapsedTime();
	location = glGetUniformLocation(shader->GetProgramID(), "Time");
	glUniform1f(location, abs(sin(time / 10 )));

	//glm::vec3 initial_color = { 0.5, 0.5, 0.5 };
	//location = glGetUniformLocation(shader->GetProgramID(), "InitialColor");
	//glUniform3fv(location, 1, glm::value_ptr(initial_color));


	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator6::OnInputUpdate(float deltaTime, int mods)
{
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			camera->TranslateForward(cameraSpeed*deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			camera->TranslateRight(-cameraSpeed*deltaTime);
			// TODO : translate the camera to the left
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			camera->TranslateForward(-cameraSpeed*deltaTime);
			// TODO : translate the camera backwards
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			// TODO : translate the camera to the right
			camera->TranslateRight(cameraSpeed*deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_Q)) {
			// TODO : translate the camera down
			camera->TranslateUpword(-cameraSpeed*deltaTime);
		}

		if (window->KeyHold(GLFW_KEY_E)) {
			// TODO : translate the camera up
			camera->TranslateUpword(cameraSpeed*deltaTime);
		}
	}

	if (window->KeyHold(GLFW_KEY_J)) {
		general_angle -= 0.5;
	}
	if (window->KeyHold(GLFW_KEY_L)) {
		general_angle += 0.5;
	}
	if (window->KeyHold(GLFW_KEY_I)) {
		if (set)
			set--;
		else {
			offset -= 0.05;
			old_y += 0.05 * cos(RADIANS(general_angle));
			old_x += 0.05 * sin(RADIANS(general_angle));
		}
	}
	if (window->KeyHold(GLFW_KEY_K)) {
		old_y -=0.05 * cos(RADIANS(general_angle));
		old_x -= 0.05 * sin(RADIANS(general_angle));
	}
}

void Laborator6::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_M)
		fps = !fps;
	if (key == GLFW_KEY_J) {
		ROTATE_ANGLE = 45;
	}
	if (key == GLFW_KEY_L) {
		ROTATE_ANGLE = 15;
	}

	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_LINE:
			polygonMode = GL_FILL;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator6::OnKeyRelease(int key, int mods)
{
	if (key == GLFW_KEY_J) {
		ROTATE_ANGLE = 30;
	}
	if (key == GLFW_KEY_L) {
		ROTATE_ANGLE = 30;
	}
	// add key release event
}

void Laborator6::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {

			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(-sensivityOX*deltaY);
			camera->RotateFirstPerson_OY(-sensivityOY*deltaX);

		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(-sensivityOX*deltaY);
			camera->RotateThirdPerson_OY(-sensivityOY*deltaX);
		}
	}
}

void Laborator6::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator6::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator6::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator6::OnWindowResize(int width, int height)
{
}
