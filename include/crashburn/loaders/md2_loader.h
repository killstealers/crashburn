#ifndef CRASHBURN_LOADER_MD2_H
#define CRASHBURN_LOADER_MD2_H

#include <vector>
#include <string>
#include <map>

#include <GL/glfw.h>

#include <crashburn/crashburn.h>

namespace crashburn
{

// MD2 Header
struct MD2Header_t
{
    int ident;          // Magic number, "IDP2"
    int version;        // MD2 format version, should be 8

    int textureWidth;     
    int textureHeight;    

    int frameSize;      // in Bytes

    int num_textures;      
    int num_vertices;   // Number of vertices per frame
    int num_st;         
    int num_tris;       
    int num_glcmds;     // Number of OpenGL commands
    int num_frames;     

    int offset_textures;   
    int offset_st;      
    int offset_tris;    
    int offset_frames;  
    int offset_glcmds;  
    int offset_end;    
};


// Texture data
struct MD2Texture_t
{
    char name[64];  
};


// Texture coords.
struct MD2TexCoord_t
{
    short s;
    short t;
};


// Triangle data
struct MD2Triangle_t
{
    unsigned short vertex[3];  // Triangle's vertex indices
    unsigned short st[3];      // Texture coords indices
};


// Vertex data
struct MD2Vertex_t
{
    unsigned char v[3];         // Compressed vertex position    
};


// Frame data
struct MD2Frame_t
{
    // Destructor
    ~MD2Frame_t () 
    {
        delete [] verts;
    }

    vec3f_t scale;        
    vec3f_t translate;    
    char name[16];       
    MD2Vertex_t *verts;  
};


// OpenGL commands
struct MD2Glcmd_t
{
    float s;    // S texture coord.
    float t;    // T texture coord.
    int index;  // Vertex index
};


// Animation infos (frame index)
struct MD2Anim_t
{
    int start;  
    int end;   
};


/////////////////////////////////////////////////////////////////////////////
//
// MD2 Mesh Loader Class.
//
/////////////////////////////////////////////////////////////////////////////

class MD2MeshLoader
{
public:
    // Constructors/Destructors
    MD2MeshLoader(const std::string &filename); 
    ~MD2MeshLoader();

public:
    // Internal types
    typedef std::map<std::string, GLuint> TextureMap;
    typedef std::map<std::string, MD2Anim_t> AnimMap;

public:
    // Public interface  
    
    //virtual bool load(Mesh* mesh, std::string filename); 

    void setScale(GLfloat scale) { _scale = scale; }

    // Accessors
    const TextureMap &Textures() const { return _textureIds; }
    const AnimMap &anims() const { return _anims; }

private:
    // Internal functions
    //void setupAnimations ();

private:
    // Member variables

    // Constants   
    static int _kMD2Ident;
    static int _kMD2Version;

    // Model data
    MD2Header_t _header;
    MD2Texture_t *_textures;
    MD2TexCoord_t *_texCoords;
    MD2Triangle_t *_triangles;
    MD2Frame_t *_frames;
    int *_glcmds;

    GLfloat _scale;
    GLuint _tex;

    TextureMap _textureIds;
    AnimMap _anims;
};

} // end of namespace 'crashburn'

#endif // CRASHBURN_LOADER_MD2_H
