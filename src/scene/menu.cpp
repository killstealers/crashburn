#include <crashburn/scene/menu.h>

#include <GL/glfw.h>

#include <crashburn/engine/engine.h>

namespace crashburn
{

Menu::Menu(Engine& engine)
    : Scene(engine)
{
}

Menu::~Menu()
{
}

bool Menu::setup()
{
    glClearColor(0.25, 0.75, 0.5, 0);
    glClearDepth(0);
    return true;
}

bool Menu::cleanup()
{
    return true;
}

void Menu::handle_events()
{
}

void Menu::update(double time_delta)
{
}

void Menu::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // end of namespace 'crashburn'
