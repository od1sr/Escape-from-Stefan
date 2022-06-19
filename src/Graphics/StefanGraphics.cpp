#include "StefanGraphics.h"
#include "Texture.h"
#include "logging.h"
#include "config.h"
#include "StefanPhysics.h"

using sgl::StefanGraphics;

int StefanGraphics::screen_w, StefanGraphics::screen_h;
float StefanGraphics::mouse_offset_x, StefanGraphics::mouse_offset_y, StefanGraphics::mouse_last_x, 
	StefanGraphics::mouse_last_y;
GLFWwindow *StefanGraphics::win;
sgl::CameraFPS StefanGraphics::cam;
Shader StefanGraphics::shader;
glm::mat4 StefanGraphics::projection;
sgl::ProjectLight StefanGraphics::flashlight;
sgl::DirectionalLight StefanGraphics::directional_light;
std::vector<const sgl::IDrawable*> StefanGraphics::objects_to_draw;

namespace
{
	const float sensetivity = 0.007f;
	const float speed = 10.f;
}

void StefanGraphics::init(const char *window_name)
{
	mouse_offset_x = 0.;
	mouse_offset_y = 0.;
	createWindow(window_name);
	createShader();
	createLights();
	
	projection = glm::mat4(1.f);
	cam = CameraFPS(glm::vec3(0.f, 1.f, 5.f));
}

void glfwError(int id, const char *description);
void StefanGraphics::createWindow(const char *win_name)
{
	glfwSetErrorCallback(glfwError);
	glfwInit();
	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(monitor);
	screen_w = mode->width;
	screen_h = mode->height;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	win = glfwCreateWindow(screen_w, screen_h, win_name, NULL, NULL);
	if (!win)
	{
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(win);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		exit(2);
	}
#ifndef _DEBUG // it is difficult to debug in fullscreen mode
	glfwSetWindowMonitor(win, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
#endif // !_DEBUG
	glfwSetFramebufferSizeCallback(win,
		framebufferSizeCallback
	);
	glViewport(0, 0, screen_w, screen_h);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(win,
		mouseMovementCallback
	);
	glfwSetCursorPos(win, mouse_last_x = screen_w / 2.f, mouse_last_y = screen_h / 2.f);
}

void glfwError(int id, const char *description)
{
	int log_len = 40 + strlen(description);
	char *log = new char[log_len];
	sprintf_s(log, log_len, "[GLFW Error] (%d: %s)\n", id, description);
	save_error_log(log);
	delete[] log;
}

void StefanGraphics::framebufferSizeCallback(GLFWwindow *win, int w, int h)
{
	screen_h = h;
	screen_w = w;
	glViewport(0, 0, w, h);
}

void sgl::StefanGraphics::mouseMovementCallback(GLFWwindow* win, double xpos, double ypos)
{
	mouse_offset_x = mouse_last_x - xpos;
	mouse_offset_y = mouse_last_y - ypos;
	mouse_last_x = xpos;
	mouse_last_y = ypos;
}

void StefanGraphics::createShader()
{
	shader = Shader("shaders\\vertex shader.glsl", "shaders\\fragment shader.glsl");
	Shader::CompileStatus status = shader.getCompilestatus();
	if (status != Shader::CompileStatus::OK)
		exit((int)status);
	shader.use();
}

void StefanGraphics::createLights()
{
	flashlight.ambient = vec4(0.1f, 0.1f, 0.1f, 1.f);
	flashlight.diffuse = vec4(0.5f, 0.5f, 0.5f, 1.f);
	flashlight.specular = vec4(0.5f, 0.5f, 0.5f, 1.f);
	flashlight.constant = 1.f;
	flashlight.linear = 0.014f;
	flashlight.quadratic = 0.0007;
	flashlight.setDirection(vec3(0.5f, -0.7f, 0.5f));
	flashlight.pos = vec3(-4.f, 5.f, -6.f);
	flashlight.inside_angle = glm::radians(20.f);
	flashlight.outside_angle = glm::radians(25.f);
	flashlight.turnOn();
	directional_light.setDirection(vec3(0.1f, -1.f, 0.f));
	directional_light.ambient = vec4(0.8f, 0.8f, 0.8f, 1.f);
	directional_light.diffuse = vec4(0.5f, 0.5f, 0.5f, 1.f);
	directional_light.specular = vec4(0.1f);
	flashlight.addToShaderProgram(shader);
	directional_light.addToShaderProgram(shader);
	directional_light.turnOn();
}

void StefanGraphics::mainLoop()
{
	while (!glfwWindowShouldClose(win))
	{
		static float last_time = glfwGetTime();
		float now = glfwGetTime();
		float deltatime = now - last_time;
		glm::vec3 cam_direction = cam.getDirection(), cam_pos = cam.getPos();
		glm::mat4 view = glm::mat4(1.0f);
		last_time = now;
		view = cam.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f),
			(float)screen_w / screen_h,
			0.1f, 1000.0f); 
		GL_CALL(glClearColor(0.1f, 0.f, 0.f, 1.f));
		GL_CALL(glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));
		cam.rotate(mouse_offset_x * sensetivity, mouse_offset_y * sensetivity);
		moveCamera();
		mouse_offset_x = mouse_offset_y = 0.f;
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setVec3("camera_pos", cam_pos);
		flashlight.setDirection(cam_direction);
		flashlight.pos = cam_pos;
		Light::DrawLights(shader);
		StefanPhysics::stepSimulation();
		for (int i = objects_to_draw.size() - 1; i >= 0; --i)
			objects_to_draw[i]->draw(shader);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}

void StefanGraphics::moveCamera()
{
	static double last_time = glfwGetTime();
	double now = glfwGetTime();
	double deltatime = now - last_time;
	last_time = now;
	double offset = speed * deltatime;
	if (glfwGetKey(win, GLFW_KEY_W))
		cam.directionalMovement(sgl::straight, offset);
	if (glfwGetKey(win, GLFW_KEY_S))
		cam.directionalMovement(sgl::straight, -offset);
	if (glfwGetKey(win, GLFW_KEY_D))
		cam.directionalMovement(sgl::right, offset);
	if (glfwGetKey(win, GLFW_KEY_A))
		cam.directionalMovement(sgl::right, -offset);
	if (glfwGetKey(win, GLFW_KEY_SPACE))
		cam.directionalMovement(sgl::up, offset);
	if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT))
		cam.directionalMovement(sgl::up, -offset);
}

void StefanGraphics::terminate()
{
	glfwTerminate();
}

void sgl::StefanGraphics::addObject(const IDrawable *object)
{
	objects_to_draw.push_back(object);
}
