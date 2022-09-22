/* Main.cpp file description:
* The core of the program and the game, main() is where the program starts when it begins running.
* (Will eventually contain the game's main function calls and gameplay loops.)
*/

// Very necessary for 3D rendering and general glad/glfw window functions:
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// These are for linear algebra (for rendering and other.)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// (Visual Studio automatically adds this but not every IDE does.)
#include <cmath>
#include <vector>

// (Used for texture and miscellaneous image-related things.)
#include <stb/stb_image.h>

// (Our manually coded header files.)
#include "Texture.h"
#include "shaderClass.h"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"

// Testing vertices for a 3D block.
std::vector<Vertex> vertices =
{ //   COORDINATES        /      TexCoord   //
	{{0.0f, 0.0f, 0.0f},	 {1.0f, 0.0f}}, //Back face.
	{{0.0f, 1.0f, 0.0f},	 {1.0f, 1.0f}},
	{{1.0f, 1.0f, 0.0f},	 {0.0f, 1.0f}},
	{{1.0f, 0.0f, 0.0f},	 {0.0f, 0.0f}},

	{{0.0f, 0.0f, 1.0f},	 {0.0f, 0.0f}}, //Front face.
	{{0.0f, 1.0f, 1.0f},	 {0.0f, 1.0f}},
	{{1.0f, 1.0f, 1.0f},	 {1.0f, 1.0f}},
	{{1.0f, 0.0f, 1.0f},	 {1.0f, 0.0f}},

	{{0.0f, 0.0f, 0.0f},	 {0.0f, 1.0f}}, //Left face.
	{{0.0f, 1.0f, 0.0f},	 {0.0f, 0.0f}},
	{{0.0f, 1.0f, 1.0f},	 {1.0f, 0.0f}},
	{{0.0f, 0.0f, 1.0f},	 {1.0f, 1.0f}},

	{{1.0f, 0.0f, 0.0f},	 {2.0f, 2.0f}}, //Right face.
	{{1.0f, 1.0f, 0.0f},	 {2.0f, 0.0f}},
	{{1.0f, 1.0f, 1.0f},	 {0.0f, 0.0f}},
	{{1.0f, 0.0f, 1.0f},	 {0.0f, 2.0f}},

	{{0.0f, 0.0f, 0.0f},	 {0.0f, 0.0f}}, //Bottom face.
	{{1.0f, 0.0f, 0.0f},	 {3.0f, 0.0f}},
	{{1.0f, 0.0f, 1.0f},	 {3.0f, 3.0f}},
	{{0.0f, 0.0f, 1.0f},	 {0.0f, 3.0f}},

	{{1.0f, 1.0f, 0.0f},	 {4.0f, 0.0f}}, //Top face.
	{{0.0f, 1.0f, 0.0f},	 {0.0f, 0.0f}},
	{{0.0f, 1.0f, 1.0f},	 {0.0f, 4.0f}},
	{{1.0f, 1.0f, 1.0f},	 {4.0f, 4.0f}}
};
// Testing indices for a 3D block.
std::vector<GLuint> indices =
{
	0, 1, 3, //Back face
	3, 1, 2, //
	4, 5, 7, //Front face
	7, 5, 6, //
	8, 9, 11,//Left face
	11,9, 10,//
	12,13,15,//Right face
	15,13,14,//
	16,17,19,//Bottom face
	19,17,18,//
	20,21,23,//Top face
	23,21,22 //
};



// Fox's function to resize the canvas when you resize the window.
void framebuffer_size_callback(GLFWwindow* window, int w, int h){
	glViewport(0, 0, w, h);
}



int main()
{
	std::cout << "Starting Hatchetflash..." << std::endl;

	// Initializes glfw and instructs it on it's version and that we will be using it's modern core features.
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Gets the primary monitor you will be using, and sets a variable to something that points to it in case you'd like to reference/use it more directly later.
	GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
	// Gets the video-mode information of your primary monitor, including screen size.
	const GLFWvidmode * monitorVideoMode = glfwGetVideoMode(primaryMonitor);
	// Initializes monitorWidth and monitorHeight to the respective dimensions of your monitor.
	int monitorWidth = monitorVideoMode->width; int monitorHeight = monitorVideoMode->height;

	// Creates the window. (If the 4th argument is filled in with primaryMonitor, having the program open will use the entire direct monitor itself, going into fullscreen mode.)
	GLFWwindow* window = glfwCreateWindow(monitorWidth, monitorHeight, "Hatchetflash - Pre-Alpha, Going 3D!", NULL, NULL); //(width, height, name, fullscreen monitor pointer, not-important)
	// (Error checking for whether the glfw window was created successfully.)
	if (window == NULL)
	{
		std::cout << "Failed to create the GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Maximizes the window before beginning to use it, and then prepares a callback for resizing the canvas when you resize the window.
	glfwMaximizeWindow(window);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// Loads GLAD so that it configures opengl, and error checks.
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initilize GLAD." << std::endl;
		return -1;
	}

	// Gets the width and height of the window (instead of monitor) after it's created for the viewport, accounting for operating system specific weirdness such as Window's taskbar.
	int windowWidth, windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);
	// Specifies the viewport of opengl in the window.
	glViewport(0, 0, windowWidth, windowHeight);

	// Gives the window it's icon.
	stbi_set_flip_vertically_on_load(false);
	GLFWimage windowIconImage;
	windowIconImage.pixels = stbi_load("Utility_Images/Hatchetflash_Window_Icon_A.png", &windowIconImage.width, &windowIconImage.height, 0, 4);
	glfwSetWindowIcon(window, 1, &windowIconImage);
	stbi_image_free(windowIconImage.pixels);

	std::cout << "Hatchetflash Opengl-window created successfully..." << std::endl;



	// Generates the Shader object using the shaders "defualt.vert" and "default.frag".
	Shader shaderProgram("default.vert", "default.frag");

	// (A texture used for testing.)
	stbi_set_flip_vertically_on_load(true);
	Texture testingTexture("Block_Textures/HF_window_icon_16x.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	testingTexture.texUnit(shaderProgram, "tex0", 0);

	// Intitializes an imperminant testing mat4 which is used for rotating the cube over time.
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.5f, -0.5f, -0.5f));

	// Specifies the base color that the window is cleared/drawn-over with.
	glClearColor(0.02f, 0.15f, 0.17f, 1.0f);
	// Enable depth testing so things are rendered in the correct order.
	glEnable(GL_DEPTH_TEST);

	// Create a simple mesh.
	//Mesh mesh(vertices, indices);
	Model model("bunny.gltf");

	// Creates the camera object.
	Camera camera(windowWidth, windowHeight, glm::vec3(0.0f, 0.0f, 2.0f));

	// (Used for our current method of getting the fps.)
	double last_FPS_time = glfwGetTime();

	while (!glfwWindowShouldClose(window)) //Checks to see if you've "X-d out" the window.
	{
		// Clears the window canvas with it's basic clear color.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		// Handles camera inputs.
		camera.Inputs(window);
		// Updates and exports the camera matrix to the Vertex Shader.
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Binds the testing texture so that it appears in rendering.
		testingTexture.Bind();
		// Assigns a value to the model uniform; NOTE: Must always be done after activating the Shader Program
		GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "modelMatrix");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		// Draw the mesh on screen.
		//mesh.draw();
		model.draw();

		// Shows the fps in the window's title.
		glfwSetWindowTitle(window, std::to_string(1.0f / (glfwGetTime() - last_FPS_time)).c_str());
		last_FPS_time = glfwGetTime();

		//Swaps the window's back buffer canvas and it's front buffer canvas.
		glfwSwapBuffers(window);
		// Checks for window events.
		glfwPollEvents();
	}
	std::cout << "Closing Hatchetflash..." << std::endl;
	// Cleanly deletes all of the created rendering-based objects.
	testingTexture.Delete();
	shaderProgram.Delete();
	// Cleas up the data in the mesh. {WHAT DOES THIS MEAN? PROBABLY EXPLAIN.}
	//mesh.cleanup();
	model.cleanup();
	// Destroys the window, stops glfw stuff and ends the program.
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
