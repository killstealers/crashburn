#include <iostream>
#include <cstdlib>

#include <crashburn/crashburn.h>
#include <crashburn/engine/engine.h>

int main(int argc, char** argv)
{
    crashburn::Engine& engine = crashburn::Engine::instance();

    engine.start();

    return EXIT_SUCCESS;
}
