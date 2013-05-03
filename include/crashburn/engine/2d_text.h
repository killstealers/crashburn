/////////////////////////////////////////////////////////////////////////////
//
// 2D Text object to be displayed on the screen
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CRASHBURN_2D_TEXT_H
#define CRASHBURN_2D_TEXT_H

#include <string>

#include <GL/glfw.h>

#include "crashburn/crashburn.h"

namespace crashburn
{

class Text2DItem
{

public:
    // Constructors/Destructors
    Text2DItem(); 
    Text2DItem(const std::string& text); 
    ~Text2DItem();

public:
    // Getters/Setters
    void resize(const float size);

    void set_text(const std::string& text);

    void set_color(const vec3& color);

    void set_position(const vec2& position);

    const float size() const;

    const std::string text() const;

    const vec3& color() const;

    const vec2& position() const;

public:
    // functions
    void draw();

private:
    // Member variables
	vec2 _position;
	vec3 _color;
    float _size;
    std::string _text;
    
};

} // end of namespace crashburn

#endif // CRASHBURN_2D_TEXT_H