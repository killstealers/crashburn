#include <crashburn/engine/2d_text.h>

#include <iostream>
#include <cstdlib>
#include <cstring>

static GLubyte space[] =
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static GLubyte dot[] =
{0x18, 0x66, 0x42, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x42, 0x66, 0x18};

static GLubyte letters[][13] = {
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0x66, 0x3c, 0x18}, // A
{0x00, 0x00, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, // B
{0x00, 0x00, 0x7e, 0xe7, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, // V
{0x00, 0x00, 0xfc, 0xce, 0xc7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc7, 0xce, 0xfc}, // D
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xc0, 0xff}, // E
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc, 0xc0, 0xc0, 0xc0, 0xff}, // F
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xcf, 0xc0, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e}, // G
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, // H
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7e}, // I
{0x00, 0x00, 0x7c, 0xee, 0xc6, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06}, // J
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc, 0xc6, 0xc3}, // K
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0}, // L
{0x00, 0x00, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xdb, 0xff, 0xff, 0xe7, 0xc3}, // M
{0x00, 0x00, 0xc7, 0xc7, 0xcf, 0xcf, 0xdf, 0xdb, 0xfb, 0xf3, 0xf3, 0xe3, 0xe3}, // N
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xe7, 0x7e}, // O
{0x00, 0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, // P
{0x00, 0x00, 0x3f, 0x6e, 0xdf, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0x66, 0x3c}, // Q
{0x00, 0x00, 0xc3, 0xc6, 0xcc, 0xd8, 0xf0, 0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe}, // R
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0xe0, 0xc0, 0xc0, 0xe7, 0x7e}, // S
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff}, // T
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, // U
{0x00, 0x00, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, // V
{0x00, 0x00, 0xc3, 0xe7, 0xff, 0xff, 0xdb, 0xdb, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3}, // W
{0x00, 0x00, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, // X
{0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3c, 0x3c, 0x66, 0x66, 0xc3}, // Y
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x7e, 0x0c, 0x06, 0x03, 0x03, 0xff}  // Z
};

namespace crashburn
{

Text2DItem::Text2DItem()
    : size_(12.0)        
{
    std::memset(position_, 0, sizeof(position_));
    color_[0] = 1.0;
    color_[1] = 1.0;
    color_[2] = 1.0;
}

Text2DItem::Text2DItem(const std::string& text)
    : size_(12.0)        
{
    std::memset(position_, 0, sizeof(position_));
    color_[0] = 1.0;
    color_[1] = 1.0;
    color_[2] = 1.0;
    text_ = text;
    
}

Text2DItem::~Text2DItem()
{
}

void Text2DItem::resize(float size)
{
    size_ = size;
}

void Text2DItem::set_text(const std::string& text)
{
    text_ = text;
}

void Text2DItem::set_color(float r, float g, float b)
{
  color_[0] = r;
  color_[1] = g;
  color_[2] = b;
}

void Text2DItem::set_position(float x, float y)
{
    position_[0] = x;
    position_[1] = y;
}

const float Text2DItem::size() const
{
    return size_;
}

const std::string& Text2DItem::text() const
{
    return text_;
}

const vec3f_t& Text2DItem::color() const
{
    return color_;
}

const vec2f_t& Text2DItem::position() const
{
    return position_;
}


void Text2DItem::render() const
{
    glColor3fv(color_);
    glRasterPos2fv(position_);    

    GLint previous_alignment;
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &previous_alignment);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for(int i = 0; i < text_.length() && i < 100; ++i)
    {
        glBitmap(8, 13, 0.0, 2.0, 10.0, 0.0, dot);
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, previous_alignment);
    
    glFlush ();
}

} // end of namespace crashburn
/*
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .

. . . # # . . . 00011000 0x18
. # # . . # # . 01100110 0x66
. # . . . . # . 01000010 0x42
# . . . . . . # 10000001 0x81
# . . . . . . # 10000001 0x81
# . . . . . . # 10000001 0x81
# . . . . . . # 10000001 0x81
# . . . . . . # 10000001 0x81
# . . . . . . # 10000001 0x81
# . . . . . . # 10000001 0x81
. # . . . . # . 01000010 0x42
. # # . . # # . 01100110 0x66
. . . # # . . . 00011000 0x18
*/