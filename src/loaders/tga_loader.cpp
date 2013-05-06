#include <crashburn/loaders/tga_loader.h>

#include <fstream>
#include <cassert>
#include <cstring>

#include <crashburn/crashburn.h>

namespace crashburn
{

// Internal linkage symbols
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
    uint32_t extension_offset;
    uint32_t developer_offset;
    char     signature[18];
} footer_t;

typedef struct __attribute__((__packed__))
{
    uint8_t descriptor;
} rlepacket_t;
}

void* load_tga(const std::string& filename, TextureInfo& ti)
{
    // We only support one type of format:
    // 32bits unsigned ints RGBA
    ti.target = GL_TEXTURE_2D;
    ti.level = 0;
    ti.internalFormat = GL_RGBA;
    ti.type = GL_UNSIGNED_BYTE;
    
    std::ifstream ifs(filename.c_str(), std::ifstream::binary);
    assert(ifs && "error: failed to load tga file");

    // Check that the file is TGA2-compliant
#ifndef NDEBUG
    {
        footer_t footer;
        ifs.seekg(-sizeof(footer_t), std::ios::end);
        ifs.read((char*)&footer, sizeof(footer_t));
        assert(std::strncmp(footer.signature, "TRUEVISION-XFILE.", 18)==0 &&
               "error: filetype not supported (legacy TGA1)");
        ifs.seekg(0);
    }
#endif // NDEBUG

    header_t header;

    // Read the whole file header
    ifs.read((char*)&header, sizeof(header_t));

    ti.width = header.width;
    ti.height = header.height;

    assert(header.depth/8==3 || header.depth/8==4 &&
           "error: unsupported tga depth (not 24/32)");
    ti.format = header.depth/8==3 ? GL_RGB : GL_RGBA;

    // We don't care of the ID field
    ifs.ignore(header.id_length);

    // We do not support color maps
    ifs.ignore(header.color_map_length*header.color_map_entry_size);

    // We only support 32 and 24 bpp
    std::size_t data_size = header.width*header.height*header.depth/8;
    ti.data = new uint8_t[data_size];

    // Uncompressed, true-color
    if (header.image_type==2)
    {
        // Load the whole image data
        ifs.read((char*)ti.data, data_size);
    }
    // Compressed (RLE), true-color
    else if (header.image_type==10)
    {
        std::size_t npix = 0;
        rlepacket_t packet;
        while (npix<header.width*header.height)
        {
            const uint8_t rle_mask = 0x80;
            // Read the RLE packet header
            ifs.read((char*)&packet, sizeof(rlepacket_t));
            // Read the first reference pixel
            char bgra[4];
            ifs.read(bgra, header.depth/8);
            std::memcpy(ti.data+(header.depth/8)*npix, bgra, header.depth/8);
            ++npix;
            // Unpack the packet descriptor
            uint8_t count = packet.descriptor & ~rle_mask;
            if (packet.descriptor & rle_mask)
                for (uint8_t i=0; i<count; ++i)
                    memcpy(ti.data+(header.depth/8)*npix+(header.depth/8)*i,
                           bgra, header.depth/8);
            else // RAW packet
                ifs.read((char*)ti.data+(header.depth/8)*npix,
                         header.depth/8*count);
            npix += count;
        }
    }

    // Swap red and blue (BGR[A] -> RGB[A])
    uint8_t* pixel = ti.data;
    do {
        std::swap(pixel[0], pixel[2]);
        pixel += header.depth/8;
    } while (pixel-ti.data < data_size);

    ifs.close();

    return 0;
}

} // end of namespace 'crashburn'
