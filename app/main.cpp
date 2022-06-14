#include "StefanGraphics.h"
#include "StefanPhysics.h"

int main()
{
	sgl::StefanPhysics::init();
	sgl::StefanGraphics::init("Minesraft", 1920, 1080);
	sgl::StefanGraphics::mainLoop();
	sgl::StefanGraphics::terminate();
	sgl::StefanPhysics::terminate();
	return 0;
}