#include <iostream>
#include <cstdlib>

#include <crashburn/crashburn.h>
#include <crashburn/engine/engine.h>
#include <crashburn/scene/intro.h>

int main(int argc, char** argv)
{

    crashburn::Engine& engine = crashburn::Engine::instance();

    engine.set_scene(new crashburn::Intro(engine));

    engine.start();

    return EXIT_SUCCESS;
}
