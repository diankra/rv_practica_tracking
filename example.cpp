#include <stdio.h>
#include <math.h>
#include "aruco.h"
#include "cvdrawingutils.h"
#include <fstream>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <sstream>
#include <string>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader.h"
#include "opengl_tools.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

#ifdef _WIN32
std::string BASE_PATH = "D:/Universidad/Cuarto/Realidad Virtual/rv_practica_tracking/rv_practica_tracking/";
#else
std::string BASE_PATH = "D:/Universidad/Cuarto/Realidad Virtual/rv_practica_tracking/rv_practica_tracking/";
#endif

// aruco
float f = 0.0;

aruco::MarkerDetector MDetector;
cv::VideoCapture TheVideoCapturer;
std::vector<aruco::Marker> TheMarkers;
cv::Mat TheInputImage, TheInputImageGrey, TheInputImageCopy;
aruco::CameraParameters TheCameraParameters;
int iDetectMode = 0, iMinMarkerSize = 0, iCorrectionRate = 0, iShowAllCandidates = 0, iEnclosed = 0, iThreshold, iCornerMode, iDictionaryIndex, iTrack = 0;

class CmdLineParser { int argc; char** argv; public:CmdLineParser(int _argc, char** _argv) : argc(_argc), argv(_argv) {}   bool operator[](std::string param) { int idx = -1;  for (int i = 0; i < argc && idx == -1; i++)if (std::string(argv[i]) == param)idx = i; return (idx != -1); }    std::string operator()(std::string param, std::string defvalue = "-1") { int idx = -1; for (int i = 0; i < argc && idx == -1; i++)if (std::string(argv[i]) == param)idx = i; if (idx == -1)return defvalue; else return (argv[idx + 1]); } };
struct   TimerAvrg { std::vector<double> times; size_t curr = 0, n; std::chrono::high_resolution_clock::time_point begin, end;   TimerAvrg(int _n = 30) { n = _n; times.reserve(n); }inline void start() { begin = std::chrono::high_resolution_clock::now(); }inline void stop() { end = std::chrono::high_resolution_clock::now(); double duration = double(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count())*1e-6; if (times.size() < n) times.push_back(duration); else { times[curr] = duration; curr++; if (curr >= times.size()) curr = 0; } }double getAvrg() { double sum = 0; for (auto t : times) sum += t; return sum / double(times.size()); } };
TimerAvrg Fps;

// camera viewpoint
glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.5f));
glm::vec3 current_pos = glm::vec3(1.0f);

//mis cosas
glm::mat4 model2 = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);

void changeCamera(aruco::Marker cameraMarker)
{
	current_pos = glm::vec3(-cameraMarker.Tvec.at<float>(0, 0),
		cameraMarker.Tvec.at<float>(1, 0),
		-cameraMarker.Tvec.at<float>(2, 0));

	cv::Mat current_rotation;
	cv::Rodrigues(cameraMarker.Rvec, current_rotation);
	cv::Mat orientation = current_rotation * cv::Vec3f(0.0f, 1.0f, 0.0f);

	std::cout << "Marker rotation: " << TheMarkers[0].Rvec << "\n";
	m_view = glm::lookAt(
		current_pos,            // camera pos
		glm::vec3(0, 0, 0),       // look at 

		//glm::vec3(0,-1,0)     // up vector (set to 0,-1,0 to look upside-down)

		glm::vec3(orientation.at<float>(0, 0),
			orientation.at<float>(1, 0),
			orientation.at<float>(2, 0))
	);

	//	ourShader.setMat4("view", current_view);
}

void changePosition(aruco::Marker movementMarker)
{
	// TAREA 4
	model2 = glm::mat4(1.0f);
	cv::Mat current_rotation;
	cv::Rodrigues(movementMarker.Rvec, current_rotation);
	cv::Mat orientation = current_rotation * cv::Vec3f(0.0f, 1.0f, 0.0f);
	
	

	glm::mat4 trans = glm::translate(model2, glm::vec3(movementMarker.Tvec.at<float>(0, 0),
		movementMarker.Tvec.at<float>(1, 0),
		-movementMarker.Tvec.at<float>(2, 0)));
	
	glm::mat4 rotateZ = glm::rotate(model2, orientation.at<float>(2, 0), glm::vec3(0, 0, 1));
	glm::mat4 rotateY = glm::rotate(model2, orientation.at<float>(1, 0), glm::vec3(0, 1, 0));
	glm::mat4 rotateX = glm::rotate(model2, orientation.at<float>(0, 0), glm::vec3(1, 0, 0));
	
	glm::mat4 rotateAxis = rotateZ * rotateY * rotateX;
	model2 = trans * rotateAxis;
}
int main(int argc, char **argv)
{
	ToolsC * tools = new ToolsC(BASE_PATH);

	CmdLineParser cml(argc, argv);
	// read camera parameters if passed
	if (cml["-c"])
		TheCameraParameters.readFromXMLFile(cml("-c"));

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _APPLE_
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "RV – Practica tracking", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	Shader ourShader(std::string(BASE_PATH + "vert.vs").c_str(), std::string(BASE_PATH + "frag.fs").c_str());
	Shader ourShader2D(std::string(BASE_PATH + "vert2D.vs").c_str(), std::string(BASE_PATH + "frag2D.fs").c_str());

	// load and create a texture 
	tools->loadTextures();

	// initializes vertex buffers
	tools->initRenderData();

	std::cout << "VBOs[0] = " << tools->m_VBOs[0] << "\n";
	std::cout << "VAOs[0] = " << tools->m_VAOs[0] << "\n";
	std::cout << "VBOs[1] = " << tools->m_VBOs[1] << "\n";
	std::cout << "VAOs[1] = " << tools->m_VAOs[1] << "\n";
	std::cout << "VBOs[2] = " << tools->m_VBOs[2] << "\n";
	std::cout << "VAOs[2] = " << tools->m_VAOs[2] << "\n";

	// set up shader materials
	ourShader.use();
	ourShader.setInt("material.diffuse", 0);
	ourShader.setInt("material.specular", 1);

	// set up shader materials
	ourShader2D.use();
	ourShader2D.setInt("image", 1);

	// opens video input from webcam
	TheVideoCapturer.open(0);

	// check video is open
	if (!TheVideoCapturer.isOpened())
		throw std::runtime_error("Could not open video");

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		
		// this will contain the image from the webcam
		cv::Mat frame, frameCopy;

		// capture the next frame from the webcam
		TheVideoCapturer >> frame;
		cv::cvtColor(frame, frame, CV_RGB2BGR);
		std::cout << "Frame size: " << frame.cols << " x " << frame.rows << " x " << frame.channels() << "\n";

		// creates a copy of the grabbed frame
		frame.copyTo(frameCopy);

		// Tamaño en cm del marcador
		float TheMarkerSize = 0.1;

		if (TheCameraParameters.isValid())
			std::cout << "Parameters OK\n";

		// TAREA 1: detectar marcadores con MDetector.detect (ver librería Aruco)
		TheMarkers = MDetector.detect(frameCopy, TheCameraParameters, TheMarkerSize);


		// TAREA 1: para cada marcador, dibujar la imagen frameCopy
		//	- el marcador en la imagen usando método draw() de aruco::Marker
		//	- el eje local de coordenadas del marcador con aruco::CvDrawingUtils::draw3dAxis()
		for (unsigned int i = 0; i < TheMarkers.size(); i++)
		{
			TheMarkers[i].draw(frameCopy);
			aruco::CvDrawingUtils::draw3dAxis(frameCopy, TheMarkers[i], TheCameraParameters);

		}

		// copies input image to m_textures[1]
		flip(frameCopy, frameCopy, 0);
		glBindTexture(GL_TEXTURE_2D, tools->m_textures[1]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frameCopy.cols, frameCopy.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frameCopy.ptr());

		// input
		processInput(window);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

		 // bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tools->m_textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tools->m_textures[1]);

		// activate shader
		ourShader.use();

		// create transformations
		glm::mat4 current_view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// pass transformation matrices to the shader
		ourShader.setMat4("projection", projection);
		//ID del que se mueve: 2
		//ID del quieto: 1
		aruco::Marker cameraMarker; //Este es el marker que mueve la cámara (el que te pones en la frente)
		aruco::Marker movementMarker; //Este es el marker que mueve el segundo cubo
		
		if (TheMarkers.size() > 0)
		{

			if (TheMarkers[0].id == 1)
			{
				cameraMarker = TheMarkers[0];
				changeCamera(cameraMarker);
			}
			else
			{
				movementMarker = TheMarkers[0];
				changePosition(movementMarker);
			}
			if (TheMarkers.size() > 1)
			{
				if (TheMarkers[1].id == 2)
				{
					movementMarker = TheMarkers[1];
					changePosition(movementMarker);
				}
				else
				{
					cameraMarker = TheMarkers[1];
					changeCamera(cameraMarker);
				}

			}
			std::cout << "Marker id: " << TheMarkers[0].id << "\n";
						
					
					
					
		}
		//	std::cout << "No marker detected\n";
			ourShader.setMat4("view", m_view);
			ourShader.setVec3("viewPos", current_pos);
		

		ourShader.setVec3("light.direction", -1.0f, .0f, -1.0f);

		// light properties
		ourShader.setVec3("light.ambient", 0.5f, 0.5f, 0.5f);
		ourShader.setVec3("light.diffuse", 0.75f, 0.75f, 0.75f);
		ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		ourShader.setFloat("material.shininess", 1.0f);
		glBindTexture(GL_TEXTURE_2D, tools->m_textures[2]);
		// render object
		glBindVertexArray(tools->m_VAOs[3]);
		
		// calculate the model matrix for each object and pass it to shader before drawing
		
		model = glm::mat4(1.0f);
		// sets model matrix
		ourShader.setMat4("model", model2);

		// draws
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//shader 2
		ourShader.use();
		
		// render object
		glBindVertexArray(tools->m_VAOs[2]);
		glBindTexture(GL_TEXTURE_2D, tools->m_textures[1]);
		// calculate the model matrix for each object and pass it to shader before drawing
		
		//model2 = glm::translate(model2, glm::vec3(0.2f, 0, 0));
		// sets model matrix
		ourShader.setMat4("view", m_view);
		ourShader.setVec3("viewPos", current_pos);
		ourShader.setMat4("model", model);
		// draws
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		ourShader.setInt("material.specular", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tools->m_textures[2]);
		glBindVertexArray(tools->m_VAOs[1]);                 //VAOs[1] is floor
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glUseProgram(0);

		// Prepare transformations
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tools->m_textures[1]);

		ourShader2D.use();
		float screen_width = 2.0f;
		glm::mat4 projection2D = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT, -1.0f, 1.0f);
		glm::mat4 model2D = glm::translate(glm::mat4(1.0f), glm::vec3((float)SCR_WIDTH / 2.0f - 160.0f, 540.0f, 0.f));

		ourShader2D.setMat4("projection2D", projection2D);
		//std::cout << "glGetUniformLocation " << " :" << glGetUniformLocation(ourShader2D.ID, "projection2D");
		ourShader2D.setMat4("model2D", model2D);

		glBindVertexArray(tools->m_VAOs[0]);                 //VAOs[0] is 2D quad for cam input
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glUseProgram(0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &(tools->m_VAOs[2]));
	glDeleteBuffers(1, &(tools->m_VBOs[2]));

	glDeleteVertexArrays(1, &(tools->m_VAOs[1]));
	glDeleteBuffers(1, &(tools->m_VBOs[1]));

	glDeleteVertexArrays(1, &(tools->m_VAOs[0]));
	glDeleteBuffers(1, &(tools->m_VBOs[0]));

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();

	std::cout << "Bye!" << std::endl;
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

