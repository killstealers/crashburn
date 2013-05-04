#include <crashburn/loaders/tga_loader.h>

#include <fstream>
#include <iostream>
#include <cassert>

#include <netinet/in.h>

#include <crashburn/crashburn.h>

namespace crashburn
{

// Place internal linkage symbols here
namespace {
typedef struct __attribute__((__packed__))
{
    uint8_t  id_length;

    uint8_t  color_map_type;

    uint8_t  image_type;

    uint16_t color_map_offset;
    uint16_t color_map_length;
    uint8_t  color_map_entry_size;

    uint16_t x_origin;
    uint16_t y_origin;
    uint16_t width;
    uint16_t height;
    uint8_t  depth;
    uint8_t  image_descriptor;
} header_t;

typedef struct __attribute__((__packed__))
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
} color_t;

typedef struct __attribute__((__packed__))
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t alpha;
} tgacolor_t;
}

void* load_tga(const std::string& filename, TextureInfo& ti)
{
    // We only support one type of format:
    // 32bits unsigned ints RGBA 
    ti.target = GL_TEXTURE_2D;
    ti.level = 0;
    ti.internalFormat = GL_RGBA;
    ti.format = GL_RGBA;
    ti.type = GL_UNSIGNED_BYTE;
    
    // Read TGA header
    std::ifstream ifs(filename.c_str(), std::ifstream::binary);

    assert(ifs && "error: failed to load tga file");

    header_t header;

    // Read the whole file header
    ifs.read((char*)&header, sizeof(header_t));

    ti.width = header.width;
    ti.height = header.height;

    // We don't care of the ID field
    if (header.id_length)
        ifs.ignore(header.id_length);

    // We do not support color maps
    if (header.color_map_type)
        ifs.ignore(header.color_map_length*header.color_map_entry_size);

    // We only support 32bpp, aka 8x8x8x8 RGBA
    ti.data = new uint8_t[header.width*header.height*sizeof(color_t)];

    // Loop thought the colors, transform them in our internal format
    color_t* color = (color_t*)ti.data;
    tgacolor_t bgra;
    do
    {
        ifs.read((char*)&bgra, sizeof(bgra));
        color->red   = bgra.red;
        color->green = bgra.green;
        color->blue  = bgra.blue;
        //color->alpha = bgra.alpha;
    } while (++color-(color_t*)ti.data < ti.width*ti.height);

    ifs.close();

    return 0;
}

} // end of namespace 'crashburn'
