#include "logging.h"
#include <iostream>
#include <string>
#include "glad/glad.h"
#include <fstream>

void clear_gl_errors()
{
	while (true)
	{
		GLenum v = glGetError();
		if (v == GL_NO_ERROR)
			break;
	}
}

void check_gl_errors(const char *function_name, const char *filename, int line)
{
	while (GLenum error = glGetError())
	{
		int log_len = 1024;
		char *log = new char[log_len];
		sprintf_s(log, log_len, "[Opengl error] (%d) <in %s in file %s at line %d>", 
			error, function_name, filename, line);
#ifdef _DEBUG
		__debugbreak();
#endif
	}
}

void save_error_log(const std::string& log)
{
#ifdef _DEBUG
	std::cerr << log << std::endl;
#else
	std::ofstream logfile;
	logfile.open("dumps/logs.txt", std::ios::app);
	logfile << log << '\n';
	logfile.close();
#endif
}

