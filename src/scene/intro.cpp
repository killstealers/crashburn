#include <crashburn/scene/intro.h>

#include <GL/glfw.h>

#include <crashburn/scene/menu.h>
#include <crashburn/engine/engine.h>

namespace crashburn
{

Intro::Intro(Engine& engine)
    : Scene(engine)
{
}

Intro::~Intro()
{
}

bool Intro::setup()
{
    glClearColor(0.25, 0.5, 0.75, 0);
    glClearDepth(0);
    return true;
}

bool Intro::cleanup()
{
    return true;
}

void Intro::handle_events()
{
    if (glfwGetKey(GLFW_KEY_ESC)==GLFW_PRESS)
    {
        Scene* menu = new Menu(engine_);
        engine_.set_scene(menu);
    }
}

void Intro::update(double time_delta)
{
}

void Intro::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // end of namespace 'crashburn'
