#include "VAO.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>
#include "logging.h"

VAO::VAO()
{
	GL_CALL(glGenVertexArrays(1, &Vao));
	GL_CALL(glGenBuffers(1, &EBO));
}

int VAO::loadVertexBuffer(void *vertices, int size, int mode, int for_vbo)
{
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindVertexArray(Vao));
	if (for_vbo == -1)
	{
		expand_vbo_buffer();
		for_vbo = VBOs.size() - 1;
	}
	else if (for_vbo >= VBOs.size() || for_vbo < 0)
	{
		save_error_log(std::string("[VAO ERROR] (VBO index ") +
			std::to_string(for_vbo) + " out of range: " + std::to_string(VBOs.size()) + ")"
		);
		return 0;
	}
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, VBOs[for_vbo]));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, vertices, mode));
	return 1;
}

void VAO::expand_vbo_buffer()
{
	uint new_vbo;
	GL_CALL(glGenBuffers(1, &new_vbo));
	VBOs.push_back(new_vbo);
}

void VAO::loadElementsBuffer(void *elem, int size, int mode)
{
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindVertexArray(Vao));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, elem, mode));
}

void VAO::use() const
{
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindVertexArray(Vao));
}

void VAO::setVertexAttribPointer(int layout_pos, int elem_num, int type, 
	int do_normalize, int step, void *offset) const
{
	//glBindVertexArray(VAOs[current_buffer_index]);
	GL_CALL(glVertexAttribPointer(layout_pos, elem_num, type, do_normalize, step, offset));
	GL_CALL(glEnableVertexAttribArray(layout_pos));
}

VAO::~VAO() 
{
	GL_CALL(glDeleteVertexArrays(1, &Vao));
	for (int i = VBOs.size() - 1; i >= 0; --i)
	{
		GL_CALL(glDeleteBuffers(1, &(VBOs[i])));
	}
	GL_CALL(glDeleteBuffers(1, &EBO));
}