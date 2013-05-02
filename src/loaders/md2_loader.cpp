#include <crashburn/loaders/md2_loader.h>

#include <fstream>
#include <cassert>

namespace crashburn
{

// Magic number (should be 844121161)
int MD2MeshLoader::_kMD2Ident = 'I' + ('D'<<8) + ('P'<<16) + ('2'<<24);

// MD2 format version
int MD2MeshLoader::_kMD2Version = 8;


// --------------------------------------------------------------------------
// MD2MeshLoader::MD2MeshLoader
//
// Constructor.  Load a mesh from file.
// --------------------------------------------------------------------------

MD2MeshLoader::MD2MeshLoader(const std::string &filename)    
    : _textures(NULL), _texCoords(NULL), _triangles(NULL), _frames(NULL),
        _glcmds(NULL), _scale(1.0f), _tex(0)
{
    // Open the file
    std::ifstream ifs(filename.c_str(), std::ios::binary);
    
    assert(!ifs.fail() && "Couldn't open file");

    // Read header
    ifs.read(reinterpret_cast<char *>(&_header), sizeof(MD2Header_t));

    assert(_header.ident == _kMD2Ident && "Bad file ident"); 
    assert(_header.version == _kMD2Version && "Bad file version");


    // Memory allocation for mesh data
    _textures = new MD2Texture_t[_header.num_textures];
    _texCoords = new MD2TexCoord_t[_header.num_st];
    _triangles = new MD2Triangle_t[_header.num_tris];
    _frames = new MD2Frame_t[_header.num_frames];
    _glcmds = new int[_header.num_glcmds];

    // Read texture names
    ifs.seekg(_header.offset_textures, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(_textures),
            sizeof(MD2Texture_t) * _header.num_textures);

    // Read texture coords.
    ifs.seekg(_header.offset_st, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(_texCoords),
            sizeof (MD2TexCoord_t) * _header.num_st);

    // Read triangle data
    ifs.seekg(_header.offset_tris, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(_triangles),
            sizeof(MD2Triangle_t) * _header.num_tris);

    // Read frames
    ifs.seekg(_header.offset_frames, std::ios::beg);
    for(int i = 0; i < _header.num_frames; ++i)
        {
            // Memory allocation for the vertices of this frame
            _frames[i].verts = new MD2Vertex_t[_header.num_vertices];

            // Read frame data
            ifs.read(reinterpret_cast<char *>(&_frames[i].scale), sizeof(vec3_t));
            ifs.read (reinterpret_cast<char *>(&_frames[i].translate), sizeof(vec3_t));
            ifs.read(reinterpret_cast<char *>(&_frames[i].name), sizeof(char) * 16);
            ifs.read(reinterpret_cast<char *>(_frames[i].verts), sizeof(MD2Vertex_t) * _header.num_vertices);
        }

    // Read OpenGL commands
    ifs.seekg(_header.offset_glcmds, std::ios::beg);
    ifs.read(reinterpret_cast<char *>(_glcmds),
            sizeof(int) * _header.num_glcmds);

    // Close file
    ifs.close();

    // Setup animation infos
    //setupAnimations();
}


// --------------------------------------------------------------------------
// MD2MeshLoader::~MD2MeshLoader
//
// Destructor.  Free all memory allocated for the model.
// --------------------------------------------------------------------------

MD2MeshLoader::~MD2MeshLoader ()
{
    delete [] _textures;
    delete [] _texCoords;
    delete [] _triangles;
    delete [] _frames;
    delete [] _glcmds;
}


} // end of namespace 'crashburn'
