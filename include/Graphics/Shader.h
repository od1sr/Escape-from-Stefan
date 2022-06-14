#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h> // ���������� glad ��� ��������� ���� ������������ ������ OpenGL
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
class Shader
{
public:
	enum CompileStatus { OK, NOT_COMPILED_YET, E_OPEN_FILE, E_READ_FILE, E_COMPILE_SH, E_LINK_SH_PROG };
private:
	// ID - ������������� ���������
	unsigned int ID;
	CompileStatus compile_status;
public:
	// ����������� ��������� ������ � ��������� ���������� �������
	Shader(const char *vertexPath, const char *fragmentPath);
	Shader();
	CompileStatus getCompilestatus() const;
	// �������������/��������� �������
	void use() const;
	unsigned int getId() const;
	// �������� uniform-�������
	void setBool(const char *name, bool value) const;
	void setInt(const char *name, int value) const;
	void setFloat(const char *name, float value) const;
	void setVec3(const char *name, float x, float y, float z) const;
	void setVec3(const char *name, glm::vec3 v) const;
	void setVec4(const char *name, float x, float y, float z, float w) const;
	void setVec4(const char *name, glm::vec4 v) const;
	void setMat4(const char *name, const glm::mat4 &matrix) const;
	int uniformLocation(const char* name) const;
	~Shader();
};

#endif