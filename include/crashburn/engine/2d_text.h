#ifndef CRASHBURN_2D_TEXT_H
#define CRASHBURN_2D_TEXT_H

#include <string>

#include <GL/glfw.h>

#include <crashburn/crashburn.h>

namespace crashburn
{

class Text2DItem
{

public:
    Text2DItem(); 
    Text2DItem(const std::string& text); 
    ~Text2DItem();

public:
    void resize(float size);
    void set_text(const std::string& text);
    void set_color(float r, float g, float b);
    void set_position(float x, float y);

public:
    const float        size() const;
    const std::string& text() const;
    const vec3f_t&     color() const;
    const vec2f_t&     position() const;

public:
    void render() const;

private:
    vec2f_t     position_;
    vec3f_t     color_;
    float       size_;
    std::string text_;   
    
};

} // end of namespace crashburn

#endif // CRASHBURN_2D_TEXT_H
