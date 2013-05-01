#ifndef CRASHBURN_ENGINE_H
#define CRASHBURN_ENGINE_H

namespace crashburn
{

/*
 * This is the main entry point of the game. The Engine class
 * manages the windowing subsystem along with the setup of the
 * event loop.
 * This class is a singleton, if you whish to use it, just use
 * the instance() method.
 */
class Engine
{
public:
    static Engine& instance();

public:
    void start();
    void stop();

private:
    Engine();
    ~Engine();

private:
    bool running_;
};

} // end of namespace 'crashburn'

#endif // CRASHBURN_ENGINE_H

