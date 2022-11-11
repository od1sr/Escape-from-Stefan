#include "Widgets.h"
#include "logging.h"

using namespace sgl;

const uint vbo_for_square[] = {
	0, 1, 2,
	2, 3, 0
};

void sgl::BasicWidget::setVAO(float *vertices, int size)
{
	vao->loadVertexBuffer((void*)vertices, size, GL_DYNAMIC_DRAW);
	int step = size / 4;
	vao->setVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, step, (void*)NULL); // coordinates
	vao->setVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, step, (void*)(2 * sizeof(float))); // texture coordinates
	vao->setVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, step, (void*)(4 * sizeof(float))); // color
	vao->loadElementsBuffer((void*)vbo_for_square, sizeof(vbo_for_square), GL_DYNAMIC_DRAW);
}

BasicWidget::BasicWidget(float x1, float y1, float x2, float y2) : x((x2 - x1) / 2.f), y((y2 - y1) / 2.f), 
	vao(new VAO()), width((x2 - x1)), height((y2-y1))
{
}

BasicWidget::~BasicWidget()
{
	delete vao;
}

Widget::Widget(float x1, float y1, float x2, float y2, glm::vec4 _color) : BasicWidget(x1, y1, x2, y2), color(_color)
{
	float vertices[] = {
		x1, y1,		0.f, 0.f, 	color.r, color.g, color.b, color.a,
		x2, y1,		0.f, 0.f, 	color.r, color.g, color.b, color.a,
		x2, y2,		0.f, 0.f, 	color.r, color.g, color.b, color.a,
		x1, y2,		0.f, 0.f, 	color.r, color.g, color.b, color.a
	};
	setVAO(vertices, sizeof(vertices));
}

void sgl::Widget::draw(Shader &shader) const
{
	shader.use();
	shader.setBool("texture_is_used", 0);
}

Sprite::Sprite(float x1, float y1, float x2, float y2, const char *path_to_texture) : BasicWidget(x1, y1, x2, y2)
{
	loadTexture(&texture, path_to_texture, TextureType::DIFFUSE);
	float vertices[] = {
		x1, y1,		0.f, 1.f, 	0.f, 0.f, 0.f, 0.f,
		x2, y1,		1.f, 1.f, 	0.f, 0.f, 0.f, 0.f,
		x2, y2,		1.f, 0.f, 	0.f, 0.f, 0.f, 0.f,
		x1, y2,		0.f, 0.f, 	0.f, 0.f, 0.f, 0.f
	};
	setVAO(vertices, sizeof(vertices));
}

void sgl::Sprite::draw(Shader &shader) const
{
	shader.use();
	GL_CALL(glActiveTexture(GL_TEXTURE0));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, texture.id));
	shader.setInt("widget_texture", 0);
	shader.setBool("texture_is_used", 1);
}

Button::Button(float x1, float y1, float x2, float y2, const char *default_texture, const char *covered_texture)
	: Sprite(x1, y1, x2, y2, default_texture)
{
	if (covered_texture)
	{
		tex_when_covered = new Texture;
		loadTexture(tex_when_covered, covered_texture, TextureType::DIFFUSE);
	}
	else
		tex_when_covered = NULL;
	is_covered = is_pressed = false;
}

void sgl::Button::draw(Shader &shader) const
{
	Sprite::draw(shader);
	if (tex_when_covered && is_covered)
		GL_CALL(glBindTexture(GL_TEXTURE_2D, tex_when_covered->id));
}

sgl::Button::~Button()
{
	if (tex_when_covered)
		delete tex_when_covered;
}