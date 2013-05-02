#ifndef CRASHBURN_SCENE_MENU_H
#define CRASHBURN_SCENE_MENU_H

#include <crashburn/scene/scene.h>

namespace crashburn
{

class Menu : public Scene
{
public:
    Menu(Engine& engine);
    virtual ~Menu();

public:
    virtual bool setup();
    virtual bool cleanup();
    virtual void handle_events();
    virtual void update(double time_delta);
    virtual void render();
};

} // end of namespace 'crashburn'

#endif // CRASHBURN_SCENE_MENU_H

