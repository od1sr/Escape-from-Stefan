#ifndef _WIDGET_CLASS_
#define _WIDGET_CLASS_

#include "IDrawable.h"
#include "Texture.h"
#include "VAO.h"
#include "glm/vec4.hpp"
#include "Shader.h"

namespace sgl
{
	class BasicWidget : public IDrawable
	{
	 //	BasicWidget  is an abstract class which is about buttons, GUI, pictures of menu and et cetera.
	protected:
		float x, y, width, height;
		VAO *vao;
		void setVAO(float *vertices, int size); // vertices is an 8x4 array

	public:
		// x1,y1 is a coordinate of left-top corner. x2,y2 - right-bottom corner
		BasicWidget(float x1, float y1, float x2, float y2);
		~BasicWidget();
	};

	class Widget : public BasicWidget
	{
	private:
		glm::vec4 color;

	public:
		Widget(float x1, float y1, float x2, float y2, glm::vec4 color);
		void draw(Shader &shader) const override;
	};

	class Sprite : public BasicWidget  // widget with texture
	{
	protected:
		Texture texture;

	public:
		Sprite(float x1, float y1, float x2, float y2, const char *path_to_texture);      
		virtual void draw(Shader &shader) const override;
	};

	class IButton
	{
	public:
		void checkClicking(float mouse_x, float mouse_y, bool is_clicked);
	};

	class Button : public Sprite
	{
	private:
		Texture *tex_when_covered;
		bool is_covered; // the mouse's coursor is above the button
		bool is_pressed; // if it is, it will wait untill mouse-button is up

	public:
		Button(float x1, float y1, float x2, float y2, const char *default_texture, const char *covered_texture = NULL);
		void draw(Shader &shader) const override;
		~Button();
	};
}

#endif