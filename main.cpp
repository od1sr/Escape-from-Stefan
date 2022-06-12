#include "StefanGraphic.h"
#include "StefanPhysics.h"

int main()
{
	sgl::StefanGraphic::init("Minesraft", 720, 480);
	sgl::StefanPhysics::init();
	sgl::StefanGraphic::mainLoop();
	sgl::StefanGraphic::terminate();
	sgl::StefanPhysics::terminate();
	return 0;
}