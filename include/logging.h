#ifndef _LOGGING_ERRORS_H_
#define _LOGGING_ERRORS_H_

#include <iostream>

void save_error_log(const std::string& log);

#ifdef _DEBUG
#	define GL_CALL(x) clear_gl_errors();\
	x;\
	check_gl_errors(#x, __FILE__, __LINE__)
#else
#	define GL_CALL(x) x

#endif // !_DEBUG
void clear_gl_errors();
void check_gl_errors(const char *function_name, const char *filename, int line);

#endif