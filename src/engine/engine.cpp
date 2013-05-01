#include <crashburn/engine/engine.h>

#include <GL/glfw.h>
#include <cassert>

namespace crashburn
{

Engine& Engine::instance()
{
    static Engine engine;
    return engine;
}

void Engine::start()
{
    running_ = true;
    while (running_ && glfwGetWindowParam(GLFW_OPENED))
    {
        // Rendering stuff
        // ...
        
        glfwSwapBuffers();
    }
}

void Engine::stop()
{
    running_ = false;
}

Engine::Engine()
  : running_(false)
{
    int err = 0;
    
    err = glfwInit();
    assert(err && "error: failed to initialize glfw");

    err = glfwOpenWindow(1024, // width
                         768,  // height
                         32,   // red depth
                         32,   // green depth
                         32,   // blue depth
                         32,   // alpha depth
                         32,   // depth depth
                         0,    // stencil depth
                         GLFW_WINDOW);
    assert(err && "error: failed to open glfw window");
}

Engine::~Engine()
{
    glfwCloseWindow();
    glfwTerminate();
}

} // end of namespace 'crashburn'
