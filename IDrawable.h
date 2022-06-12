#ifndef _BASIC_GAME_OBJECT_
#define _BASIC_GAME_OBJECT_
#include "Shader.h"

namespace sgl
{
	class IDrawable
	{
	public:
		virtual void draw(Shader &shader) const = 0;
	};
}

#endif // !_BASIC_GAME_OBJECT_