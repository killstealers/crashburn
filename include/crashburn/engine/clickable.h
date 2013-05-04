#ifndef CRASHBURN_CLICKABLE_H
#define CRASHBURN_CLICKABLE_H

#include <string>

#include <GL/glfw.h>

#include <crashburn/crashburn.h>
#include <crashburn/engine/2d_text.h>

namespace crashburn
{

class Clickable
{

public:
    Clickable();     
    virtual ~Clickable();

public:
    void resize(float scale);
    void set_text(const std::string& text);
    void set_color(float r, float g, float b);
    void set_text_color(float r, float g, float b);
    void set_position(float x, float y);
    void set_width(float width);
    void set_height(float height);

public:
    const float        height() const;
    const float        width() const;
    const std::string& text() const;
    const vec3f_t&     color() const;
    const vec3f_t&     text_color() const;
    const vec2f_t&     position() const;
    const vec2f_t&     text_position() const;

public:
    virtual void render() const = 0;
    virtual void execute() const = 0;

protected:
    vec2f_t     position_;
    vec3f_t     color_;
    float       width_;
    float       height_;
    Text2DItem textItem_;   
    
};

} // end of namespace crashburn

#endif // CRASHBURN_CLICKABLE_H
