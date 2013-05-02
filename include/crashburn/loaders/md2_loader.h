
/////////////////////////////////////////////////////////////////////////////
//
// MD2 Loader Header file (MD2 is a 3D Mesh format)
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MD2_LOADER_H_
#define _MD2_LOADER_H_

#include <vector>
#include <string>
#include <map>

#include <GL/gl.h>

namespace crashburn
{

typedef GLfloat vec2_t[2];
typedef GLfloat vec3_t[3];


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

    vec3_t scale;        
    vec3_t translate;    
    char name[16];       
    MD2Vertex_t *verts;  
};


// OpenGL command packet
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
    //bool loadTexture (const std::string &filename);
    //void setTexture (const std::string &filename);

    //void renderFrameImmediate (int frame);
    //void drawModelItpImmediate (int frameA, int frameB, float interp);

    //void renderFrameWithGLcmds (int frame);
    //void drawModelItpWithGLcmds (int frameA, int frameB, float interp);

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

/*
/////////////////////////////////////////////////////////////////////////////
//
// MD2 Object Class.
//
/////////////////////////////////////////////////////////////////////////////

class MD2Object
{
public:
    // Public internal types/enums
    enum MD2RenderMode
    {
        kDrawImmediate = 0,
        kDrawGLcmds,
    };

public:
    // Constructor/destructor
    MD2Object ();
    MD2Object (MD2Model *model);
    ~MD2Object ();

public:
    // Public interface
    void drawObjectItp (bool animated, MD2RenderMode renderMode);
    void drawObjectFrame (int frame, MD2RenderMode renderMode);
    void animate (int startFrame, int endFrame, float percent);
    void animate (float percent);

    void setModel (MD2Model *model);
    void setScale (float scale) { _scale = scale; }
    void setAnim (const std::string &name);

    // Accessors
    const MD2Model *model () const { return _model; }
    float scale () const { return _scale; }
    const std::string &currentAnim () const { return _currentAnim; }

private:
    // Member variables
    MD2Model *_model;

    int _currFrame;
    int _nextFrame;
    float _interp;

    float _percent;
    float _scale;

    // Animation data
    const MD2Anim_t *_animInfo;
    std::string _currentAnim;
};

*/
} // end of namespace 'crashburn'

#endif // _MD2_LOADER_H_