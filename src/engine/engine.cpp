#include <crashburn/engine/engine.h>

#include <GL/glfw.h>
#include <cassert>

#include <crashburn/scene/scene.h>

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

    double time = glfwGetTime();
    double current_time = time;
    double time_delta = 0;

    while (running_ && glfwGetWindowParam(GLFW_OPENED))
    {
        if (pending_scene_)
        {
            if (scene_)
            {
                scene_->cleanup();
                delete scene_;
            }
            pending_scene_->setup();

            scene_ = pending_scene_;
            pending_scene_ = 0;
        }

        current_time = glfwGetTime();
        time_delta = current_time - time;
        time = current_time;

        glfwPollEvents();
        scene_->handle_events();
        scene_->update(time_delta);
        scene_->render();
        
        glfwSwapBuffers();
    }

    if (scene_)
    {
        scene_->cleanup();
        delete scene_;
    }

    if (pending_scene_)
    {
        delete pending_scene_;
    }
}

void Engine::set_scene(Scene* scene)
{
    pending_scene_ = scene;
}

void Engine::stop()
{
    running_ = false;
}

Engine::Engine()
  : running_(false),
    scene_(0),
    pending_scene_(0)
{
    int err = 0;
    
    err = glfwInit();
    assert(err && "error: failed to initialize glfw");

    // Enable fullscreen anti-aliasing with 4 multisamples
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);

    // Warning: do not issue any GL call before the context 
    // is created in glfwOpenWindow! This would result in a
    // plain old segfault
    
    // TODO: we should query the user desktop video mode
    // with glfwGetDesktopMode() and setup a fullscreen
    // window based on this information. During debug, a
    // fullscreen window could be a problem though, so we
    // stick to windowed mode for the moment
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

    // Enable multisampling for FSAA
    glEnable(GL_MULTISAMPLE);

    // Window title while windowed mode
    glfwSetWindowTitle("crashburn");

    // Disable vertical synchronization
    glfwSwapInterval(0);    

    // Disable automatic event polling by glfwSwapBuffers,
    // we will poll for events manually
    glfwDisable(GLFW_AUTO_POLL_EVENTS);
}

Engine::~Engine()
{
    glfwCloseWindow();
    glfwTerminate();
}

} // end of namespace 'crashburn'
