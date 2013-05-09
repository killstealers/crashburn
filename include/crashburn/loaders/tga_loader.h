#ifndef CRASHBURN_LOADER_TGA_H
#define CRASHBURN_LOADER_TGA_H

#include <string>

#include <GL/glfw.h>

#include <crashburn/crashburn.h>

namespace crashburn
{

struct TextureInfo
{
    GLenum   target;
    GLint    level;
    GLint    internalFormat;
    GLsizei  width;
    GLsizei  height;
    GLenum   format;
    GLenum   type;
    uint8_t* data;
};

void* load_tga(const std::string& filename, TextureInfo& ti);

} // end of namespace 'crashburn'

#endif // CRASHBURN_LOADER_TGA_H

