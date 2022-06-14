#ifndef _VAO_CLASS_H_
#define _VAO_CLASS_H_

#include <vector>

typedef unsigned int uint;
class VAO
{
private:
	uint Vao;
	std::vector<uint> VBOs;
	uint EBO;

	void expand_vbo_buffer();
	VAO(VAO &another);
public:
	VAO();
	void use() const; 
	int loadVertexBuffer(void *vertices, int size, int mode, int for_vbo = -1);
	void loadElementsBuffer(void *elem, int size, int mode);
	void setVertexAttribPointer(int layout_pos, int elem_num, int type, 
		int do_normalize, int step, void *offset) const;
	~VAO();
};
#endif // !_VAO_CLASS_H_