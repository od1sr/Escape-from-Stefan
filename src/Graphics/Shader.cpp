#include "Shader.h"
#include <fstream>
#include "logging.h"
Shader::Shader(const char* vertexPath, const char* fragmentPath)
	:	compile_status(OK), ID(0)
{
	std::ifstream vsh_file,
		frsh_file;
	vsh_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	frsh_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	std::stringstream vsh_stream, fsh_stream;
	int frag_sh_id, vert_sh_id;
	std::string VertexShaderCode, FragmentShaderCode;
	const char *vertex_sh_code_ptr, *fragment_sh_code_ptr;
	for(int i = 0; i < 2; ++i)
	{
		std::string fname;
		try
		{
			if (!i)
				vsh_file.open((fname = vertexPath));
			else	
				frsh_file.open((fname = fragmentPath));
		}
		catch (std::ifstream::failure &e)
		{
			save_error_log(std::string("[shader error] Error: file cant be opening: ") + fname);
			compile_status = E_OPEN_FILE;
			return;
		}
	}
	try
	{
		vsh_stream << vsh_file.rdbuf();
		vsh_file.close();
		VertexShaderCode = vsh_stream.str();
		vertex_sh_code_ptr = VertexShaderCode.c_str();
	}
	catch (std::ifstream::failure &e)
	{
		save_error_log(std::string("Error: file \"") + vertexPath +
			 "\" cant be reading");
		compile_status = E_READ_FILE;
		return;
	}
	try 
	{
		fsh_stream << frsh_file.rdbuf();
		frsh_file.close();
		FragmentShaderCode = fsh_stream.str();
		fragment_sh_code_ptr = FragmentShaderCode.c_str();
	}
	catch (std::ifstream::failure& e)
	{
		save_error_log(std::string("Error: file \"") + fragmentPath +
			"\" cant be reading");
		compile_status = E_READ_FILE;
		return;
	}
	GL_CALL(vert_sh_id = glCreateShader(GL_VERTEX_SHADER));
	GL_CALL(glShaderSource(vert_sh_id, 1, &vertex_sh_code_ptr, NULL));
	GL_CALL(glCompileShader(vert_sh_id));
	int  success;
	char infoLog[512];
	GL_CALL(glGetShaderiv(vert_sh_id, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GL_CALL(glGetShaderInfoLog(vert_sh_id, 512, NULL, infoLog));
		save_error_log(std::string("Error: shader \"") + vertexPath + 
			"\" compilation failed;\n" + infoLog);
		compile_status = E_COMPILE_SH;
		return;
	}

	GL_CALL(frag_sh_id = glCreateShader(GL_FRAGMENT_SHADER));
	GL_CALL(glShaderSource(frag_sh_id, 1, &fragment_sh_code_ptr, NULL));
	GL_CALL(glCompileShader(frag_sh_id));
	GL_CALL(glGetShaderiv(frag_sh_id, GL_COMPILE_STATUS, &success));
	if (!success)
	{
		GL_CALL(glGetShaderInfoLog(frag_sh_id, 512, NULL, infoLog));
		save_error_log(std::string("Error: shader \"") + fragmentPath + 
			"\" compilation failed;\n" + infoLog);
		compile_status = E_COMPILE_SH;
		return;
	}
	GL_CALL(ID = glCreateProgram());
	GL_CALL(glAttachShader(ID, vert_sh_id));
	GL_CALL(glAttachShader(ID, frag_sh_id));
	GL_CALL(glLinkProgram(ID));
	GL_CALL(glGetProgramiv(ID, GL_LINK_STATUS, &success));
	if (!success) {
		GL_CALL(glGetProgramInfoLog(ID, 512, NULL, infoLog));
		save_error_log(std::string("Error: link shader program failed\n") + 
			infoLog);
		compile_status = E_LINK_SH_PROG;
		return;
	}
	GL_CALL(glDeleteShader(frag_sh_id));
	GL_CALL(glDeleteShader(vert_sh_id));
}
Shader::Shader() : ID(0), compile_status(NOT_COMPILED_YET)
{
}
void Shader::use() const
{
	GL_CALL(glUseProgram(ID));
}
Shader::CompileStatus Shader::getCompilestatus() const
{
	return compile_status;
}
unsigned int Shader::getId() const
{
	return ID;
}
int Shader::uniformLocation(const char* name) const
{
	GL_CALL(return glGetUniformLocation(ID, name));
}
void Shader::setBool(const char* name, bool value) const
{
	GL_CALL(glUniform1i(glGetUniformLocation(ID, name), (int)value));
}
void Shader::setInt(const char* name, int value) const
{
	GL_CALL(GLint location = glGetUniformLocation(ID, name));
	GL_CALL(glUniform1i(location, value));
}
void Shader::setFloat(const char* name, float value) const
{
	GL_CALL(glUniform1f(glGetUniformLocation(ID, name), value));
}
void Shader::setVec3(const char* name, float x, float y, float z) const
{
	GL_CALL(glUniform3f(glGetUniformLocation(ID, name), x, y, z));
}
void Shader::setVec3(const char* name, glm::vec3 v) const
{
	GL_CALL(glUniform3f(glGetUniformLocation(ID, name), v.x, v.y, v.z));
}
void Shader::setVec4(const char* name, float x, float y, float z, float w) const
{
	GL_CALL(glUniform4f(glGetUniformLocation(ID, name), x, y, z, w));
}
void Shader::setVec4(const char* name, glm::vec4 v) const
{
	GL_CALL(glUniform4f(glGetUniformLocation(ID, name), v.x, v.y, v.z, v.w));
}
void Shader::setMat4(const char *name,  const glm::mat4 &matrix) const
{
	GL_CALL(glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE,
		glm::value_ptr(matrix)));
}
Shader::~Shader()
{
}
