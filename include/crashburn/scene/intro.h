#ifndef CRASHBURN_SCENE_INTRO_H
#define CRASHBURN_SCENE_INTRO_H

#include <crashburn/scene/scene.h>

namespace crashburn
{

class Intro : public Scene
{
public:
    Intro(Engine& engine);
    virtual ~Intro();

public:
    virtual bool setup();
    virtual bool cleanup();
    virtual void handle_events();
    virtual void update(double time_delta);
    virtual void render();
};

} // end of namespace 'crashburn'

#endif // CRASHBURN_SCENE_INTRO_H

