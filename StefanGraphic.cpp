#include "StefanGraphic.h"
#include "Texture.h"
#include "logging.h"
#include "config.h"

using sgl::StefanGraphic;

int StefanGraphic::screen_w, StefanGraphic::screen_h;
float StefanGraphic::mouse_offset_x, StefanGraphic::mouse_offset_y, StefanGraphic::mouse_last_x, 
	StefanGraphic::mouse_last_y;
GLFWwindow *StefanGraphic::win;
sgl::CameraFPS StefanGraphic::cam;
Shader StefanGraphic::shader;
glm::mat4 StefanGraphic::projection;
sgl::ProjectLight StefanGraphic::flashlight;
sgl::DirectionalLight StefanGraphic::directional_light;
sgl::Plane *StefanGraphic::plane = NULL;
sgl::GameObject *StefanGraphic::car = NULL;

namespace
{
	const int textures_number = 3;
	const char block_texture_path[] = "assets\\textures\\box.png";
	const char floor_texture_path[] = "assets\\textures\\floor.png";
	const char block_litemap_texture_path[] = "assets\\textures\\lightmap.png";
	const float sensetivity = 0.007f;
	const float speed = 10.f;
	const float texture_apart = 1.f / 3.f;
	// textures: <0px> bottom side, top <48px>
	const float block_bottom_texture_x = 0.f;
	const float block_side_texture_x = texture_apart;
	const float block_top_texture_x = texture_apart * 2;
	float block_vertices[] = {
		//sides
		// positions						textures pos			 normals			diffuse colors		specular colors
		 //back
		 -0.5f, -0.5f, -0.5f,	 block_side_texture_x,	 0.f,		0.f, 0.f, -1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f, -0.5f, -0.5f,	 block_top_texture_x,	 0.f,		0.f, 0.f, -1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f,  0.5f, -0.5f,	 block_top_texture_x,	 1.f,		0.f, 0.f, -1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 -0.5f,  0.5f, -0.5f,	 block_side_texture_x,	 1.f,		0.f, 0.f, -1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 //front										 
		 -0.5f, -0.5f, 0.5f,	 block_side_texture_x,	 0.f,		0.f, 0.f, 1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f, -0.5f, 0.5f,	 block_top_texture_x,	 0.f,		0.f, 0.f, 1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f,  0.5f, 0.5f,	 block_top_texture_x,	 1.f,		0.f, 0.f, 1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 -0.5f,  0.5f, 0.5f,	 block_side_texture_x,	 1.f,		0.f, 0.f, 1.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 //left											 
		 -0.5f,  0.5f,  0.5f,	 block_top_texture_x,	 1.f,		-1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 -0.5f,  0.5f, -0.5f,	 block_side_texture_x,	 1.f,	   	-1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 -0.5f, -0.5f, -0.5f,	 block_side_texture_x,	 0.f,	   -1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 -0.5f, -0.5f,  0.5f,	 block_top_texture_x,	 0.f,		-1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 //right
		  0.5f,  0.5f,  0.5f,	 block_top_texture_x,	 1.f,	   1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f,  0.5f, -0.5f,	 block_side_texture_x,	 1.f,	   1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f, -0.5f, -0.5f,	 block_side_texture_x,	 0.f,	   1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f, -0.5f,  0.5f,	 block_top_texture_x,	 0.f,	   1.f, 0.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  //bottom
		 -0.5f, -0.5f, -0.5f,	 block_bottom_texture_x, 1.0f,		0.f, -1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f, -0.5f, -0.5f,	 block_side_texture_x,   1.0f,		0.f, -1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f, -0.5f,  0.5f,	 block_side_texture_x,   0.0f,		0.f, -1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 -0.5f, -0.5f,  0.5f,	 block_bottom_texture_x, 0.0f,		0.f, -1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 //up
		 -0.5f,  0.5f, -0.5f,	 block_top_texture_x,	 1.0f,		0.f, 1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f,  0.5f, -0.5f,	 1.f,					 1.0f,		0.f, 1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		  0.5f,  0.5f,  0.5f,	 1.f,					 0.0f,		0.f, 1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		 -0.5f,  0.5f,  0.5f,	 block_top_texture_x,	 0.0f,		0.f, 1.f, 0.f,		0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f
	};
	glm::vec3 cube_pos[] = {
		glm::vec3(0.0f,  0.f, 0.f),
		glm::vec3(1.0f,  0.f, 0.f),
		glm::vec3(-1.f, 0.f, 0.f),
		glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f, 2.f, 0.f)
	};
	uint block_indices[] = {
		0, 1, 2,
		0, 3, 2,

		4, 5, 6,
		4, 7, 6,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 12,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};
	float floor_vertices[] = {
		//coords				texture		normals		  	diffuse colors		specular colors
	   -20.f, -5.f, -20.f,		0.f, 1.f,	0.f, 1.f, 0.f,	0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		20.f, -5.f, -20.f,		1.f, 1.f,	0.f, 1.f, 0.f,	0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
		20.f, -5.f,  20.f,		1.f, 0.f,	0.f, 1.f, 0.f,	0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f,
	   -20.f, -5.f,  20.f,		0.f, 0.f,	0.f, 1.f, 0.f, 	0.f,0.f,0.f,0.f,	0.f,0.f,0.f,0.f
	};
	uint floor_indices[] = {
		0, 1, 2,
		2, 3, 0
	};
}

void StefanGraphic::init(const char *window_name, int win_width, int win_height)
{
	screen_w = win_width;
	screen_h = win_height;
	mouse_offset_x = 0.;
	mouse_offset_y = 0.;
	mouse_last_x = screen_w / 2.f, mouse_last_y = screen_h / 2.f;
	createWindow(window_name);
	createShader();
	createLights();
	plane = new Plane(0.f, -5.f, 0.f, 40.f, 40.f, 0.f, glm::radians(30.f), 0.f, 64,
		block_texture_path, block_litemap_texture_path);
	car = new GameObject("assets\\3d models\\lada\\source\\\Wavefront\\testtt.obj");
	projection = glm::mat4(1.f);
	cam = CameraFPS(glm::vec3(0.f, 1.f, 5.f));
}

void StefanGraphic::createWindow(const char *win_name)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	win = glfwCreateWindow(screen_w, screen_h, win_name, NULL, NULL);
	if (!win)
	{
		save_error_log("Failed to create game window");
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(win);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		save_error_log("Failed to initialize GLAD");
		exit(2);
	}
	glfwSetFramebufferSizeCallback(win,
		[](GLFWwindow* win, int w, int h)
		{
			screen_h = h;
			screen_w = w;
			glViewport(0, 0, w, h);
		}
	);
	glViewport(0, 0, screen_w, screen_h);
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(win,
		[](GLFWwindow* win, double xpos, double ypos)
		{
			mouse_offset_x = mouse_last_x - xpos;
			mouse_offset_y = mouse_last_y - ypos;
			mouse_last_x = xpos;
			mouse_last_y = ypos;
		}
	);
	glfwSetCursorPos(win, screen_w / 2.f, screen_h / 2.f);
}

void StefanGraphic::createShader()
{
	shader = Shader("vertex shader.glsl", "fragment shader.glsl");
	Shader::CompileStatus status = shader.getCompilestatus();
	if (status != Shader::CompileStatus::OK)
		exit((int)status);
	shader.use();
}

void StefanGraphic::createLights()
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

void StefanGraphic::mainLoop()
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
		plane->draw(shader);
		plane->rotate(0.f, 0.001f, 0.f);
		car->draw(shader);
		glfwSwapBuffers(win);
		glfwPollEvents();
	}
}

void StefanGraphic::moveCamera()
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

void StefanGraphic::terminate()
{
	delete plane;
	delete car;
	glfwTerminate();
}