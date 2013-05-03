#include <iostream>
#include <cstdlib>

#include <crashburn/crashburn.h>
#include <crashburn/engine/engine.h>
#include <crashburn/scene/intro.h>

int main(int argc, char** argv)
{

    crashburn::Engine& engine = crashburn::Engine::instance();

    engine.set_scene(new crashburn::Intro(engine));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	    
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0.0, 1024, 0.0, 768, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	
    engine.start();

    return EXIT_SUCCESS;
}
