#include <iostream>
#include <cstdlib>

#include <crashburn/crashburn.h>
#include <crashburn/engine/engine.h>
#include <crashburn/scene/scene.h>
#include <crashburn/engine/2d_text.h>

class MyTestScene : public crashburn::Scene
{
public:
    MyTestScene(crashburn::Engine& engine)
        : crashburn::Scene(engine)
    {
    }

public:
    virtual bool setup()
    {
        // Clear values
        glClearColor(0.25, 0.5, 0.75, 0);
        glClearDepth(0);

        // Setup projection matrix
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho (0.0, 1024, 0.0, 768, -1.0, 1.0);

        // Setup modelview matrix
        glMatrixMode(GL_MODELVIEW);

        // Setup the text properties
        text_.set_text("HELLO");
        text_.set_color(1., 0., 0.);

        return true;
    }

    virtual bool cleanup()
    {
        return true;
    }

    virtual void handle_events()
    {
        // Exit when we press Escape
        if (glfwGetKey(GLFW_KEY_ESC)==GLFW_PRESS)
            engine_.stop();
    }

    virtual void update(double time_delta)
    {
    }

    virtual void render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        text_.render();
    }

private:
    crashburn::Text2DItem text_;
};

int main(int argc, char** argv)
{
    crashburn::Engine& engine = crashburn::Engine::instance();

    engine.set_scene(new MyTestScene(engine));
	
    engine.start();

    return EXIT_SUCCESS;
}
