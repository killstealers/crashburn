
/////////////////////////////////////////////////////////////////////////////
//
// This file load a 3D model from a file in MD2 format
//
/////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <iostream>
#include <fstream>
#include <cassert>

#include "crashburn/loaders/md2_loader.h"

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
            ifs.read(reinterpret_cast<char *>(&_frames[i].scale), sizeof(vec3));
            ifs.read (reinterpret_cast<char *>(&_frames[i].translate), sizeof(vec3));
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

/*
// --------------------------------------------------------------------------
// MD2MeshLoader::loadTexture
//
// Load a texture from file.
// --------------------------------------------------------------------------

bool
MD2MeshLoader::loadTexture (const std::string &filename)
{
    // Get texture manager
    Texture2DManager *texMgr = Texture2DManager::getInstance ();

    // load the texture
    Texture2D *tex = texMgr->load (filename);

    if (!tex->fail ())
        _skinIds.insert (SkinMap::value_type (filename, tex));

    return (!tex->fail ());
}


// --------------------------------------------------------------------------
// MD2MeshLoader::setTexture
//
// Set the current texture for the mesh.
// --------------------------------------------------------------------------

void
MD2MeshLoader::setTexture (const std::string &filename)
{
    SkinMap::iterator itor;
    itor = _skinIds.find (filename);

    if (itor != _skinIds.end ())
        _tex = itor->second;
    else
        _tex = NULL;
}


// --------------------------------------------------------------------------
// MD2MeshLoader::setupAnimations
//
// Setup animation infos.
// --------------------------------------------------------------------------

void
MD2MeshLoader::setupAnimations ()
{
    std::string currentAnim;
    MD2Anim_t animInfo = { 0, 0 };

    for (int i = 0; i < _header.num_frames; ++i)
        {
            std::string frameName = _frames[i].name;
            std::string frameAnim;

            // Extract animation name from frame name
            std::string::size_type len = frameName.find_first_of ("0123456789");
            if ((len == frameName.length () - 3) &&
        (frameName[len] != '0'))
    len++;

            frameAnim.assign (frameName, 0, len);

            if (currentAnim != frameAnim)
    {
        if (i > 0)
            {
                // Previous animation is finished, insert
                // it and start new animation.
                _anims.insert (AnimMap::value_type
                     (currentAnim, animInfo));
            }

        // Initialize new anim info
        animInfo.start = i;
        animInfo.end = i;

        currentAnim = frameAnim;
    }
            else
    {
        animInfo.end = i;
    }
        }

    // Insert last animation
    _anims.insert (AnimMap::value_type (currentAnim, animInfo));
}


// --------------------------------------------------------------------------
// MD2MeshLoader::renderFrameImmediate
//
// Render the model for the specified frame, using immediate mode.
// --------------------------------------------------------------------------

void
MD2MeshLoader::renderFrameImmediate (int frame)
{
    // Compute max frame index
    int maxFrame = _header.num_frames - 1;

    // Check if the frame index is valid
    if ((frame < 0) || (frame > maxFrame))
        return;

    // Bind to model's texture
    if (_tex)
        _tex->bind ();

    glBegin (GL_TRIANGLES);
        // Draw each triangle
        for (int i = 0; i < _header.num_tris; ++i)
            {
    // Draw each vertex of this triangle
    for (int j = 0; j < 3; ++j)
        {
            MD2Frame_t *pFrame = &_frames[frame];
            MD2Vertex_t *pVert = &pFrame->verts[_triangles[i].vertex[j]];
            MD2TexCoord_t *pTexCoords = &_texCoords[_triangles[i].st[j]];

            // Compute final texture coords.
            GLfloat s = static_cast<GLfloat>(pTexCoords->s) / _header.skinwidth;
            GLfloat t = static_cast<GLfloat>(pTexCoords->t) / _header.skinheight;

            glTexCoord2f (s, 1.0f - t);

            // Send normal vector to OpenGL
            glNormal3fv (_kAnorms[pVert->normalIndex]);

            // Uncompress vertex position and scale it
            vec3 v;

            v[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * _scale;
            v[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * _scale;
            v[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * _scale;

            glVertex3fv (v);
        }
            }
    glEnd();
}


// --------------------------------------------------------------------------
// MD2MeshLoader::drawModelItpImmediate
//
// Render the model with frame interpolation, using immediate mode.
// --------------------------------------------------------------------------

void
MD2MeshLoader::drawModelItpImmediate (int frameA, int frameB, float interp)
{
    // Compute max frame index
    int maxFrame = _header.num_frames - 1;

    // Check if frames are valid
    if ((frameA < 0) || (frameB < 0))
        return;

    if ((frameA > maxFrame) || (frameB > maxFrame))
        return;

    // Bind to model's texture
    if (_tex)
        _tex->bind ();

    glBegin (GL_TRIANGLES);
        // Draw each triangle
        for (int i = 0; i < _header.num_tris; ++i)
            {
    // Draw each vertex of this triangle
    for (int j = 0; j < 3; ++j)
        {
            MD2Frame_t *pFrameA = &_frames[frameA];
            MD2Frame_t *pFrameB = &_frames[frameB];

            MD2Vertex_t *pVertA = &pFrameA->verts[_triangles[i].vertex[j]];
            MD2Vertex_t *pVertB = &pFrameB->verts[_triangles[i].vertex[j]];

            MD2TexCoord_t *pTexCoords = &_texCoords[_triangles[i].st[j]];

            // Compute final texture coords.
            GLfloat s = static_cast<GLfloat>(pTexCoords->s) / _header.skinwidth;
            GLfloat t = static_cast<GLfloat>(pTexCoords->t) / _header.skinheight;

            glTexCoord2f (s, 1.0f - t);

            // Compute interpolated normal vector
            const GLfloat *normA = _kAnorms[pVertA->normalIndex];
            const GLfloat *normB = _kAnorms[pVertB->normalIndex];

            vec3 n;
            n[0] = normA[0] + interp * (normB[0] - normA[0]);
            n[1] = normA[1] + interp * (normB[1] - normA[1]);
            n[2] = normA[2] + interp * (normB[2] - normA[2]);

            glNormal3fv (n);

            // Compute final vertex position
            vec3 vecA, vecB, v;

            // First, uncompress vertex positions
            vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
            vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
            vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

            vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
            vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
            vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

            // Linear interpolation and scaling
            v[0] = (vecA[0] + interp * (vecB[0] - vecA[0])) * _scale;
            v[1] = (vecA[1] + interp * (vecB[1] - vecA[1])) * _scale;
            v[2] = (vecA[2] + interp * (vecB[2] - vecA[2])) * _scale;

            glVertex3fv (v);
        }
            }
    glEnd();
}


// --------------------------------------------------------------------------
// MD2MeshLoader::renderFrameWithGLcmds
//
// Render the model for the specified frame, using OpenGL commands.
// --------------------------------------------------------------------------

void
MD2MeshLoader::renderFrameWithGLcmds (int frame)
{
    // Compute max frame index
    int maxFrame = _header.num_frames - 1;

    // Check if the frame index is valid
    if ((frame < 0) || (frame > maxFrame))
        return;

    // Bind to model's texture
    if (_tex)
        _tex->bind ();

    // Pointer to OpenGL commands
    int *pGlcmds = _glcmds;
    int i;

    while ((i = *(pGlcmds++)) != 0)
        {
            if (i < 0)
    {
        glBegin (GL_TRIANGLE_FAN);
        i = -i;
    }
            else
    {
        glBegin (GL_TRIANGLE_STRIP);
    }

            // Parse all OpenGL commands of this group
            for (/* nothing *//*; i > 0; --i, pGlcmds += 3)
    {
        // pGlcmds[0] : final S texture coord.
        // pGlcmds[1] : final T texture coord.
        // pGlcmds[2] : vertex index to draw

        MD2Glcmd_t *pGLcmd = reinterpret_cast<MD2Glcmd_t *>(pGlcmds);
        MD2Frame_t *pFrame = &_frames[frame];
        MD2Vertex_t *pVert = &pFrame->verts[pGLcmd->index];

        // Send texture coords. to OpenGL
        glTexCoord2f (pGLcmd->s, 1.0f - pGLcmd->t);

        // Send normal vector to OpenGL
        glNormal3fv (_kAnorms[pVert->normalIndex]);

        // Uncompress vertex position and scale it
        vec3 v;

        v[0] = (pFrame->scale[0] * pVert->v[0] + pFrame->translate[0]) * _scale;
        v[1] = (pFrame->scale[1] * pVert->v[1] + pFrame->translate[1]) * _scale;
        v[2] = (pFrame->scale[2] * pVert->v[2] + pFrame->translate[2]) * _scale;

        glVertex3fv (v);
    }

            glEnd();
        }
}


// --------------------------------------------------------------------------
// MD2MeshLoader::drawModelItpWithGLcmds
//
// Render the model with frame interpolation, using OpenGL commands.
// --------------------------------------------------------------------------

void
MD2MeshLoader::drawModelItpWithGLcmds (int frameA, int frameB, float interp)
{
    // Compute max frame index
    int maxFrame = _header.num_frames - 1;

    // Check if frames are valid
    if ((frameA < 0) || (frameB < 0))
        return;

    if ((frameA > maxFrame) || (frameB > maxFrame))
        return;

    // Bind to model's texture
    if (_tex)
        _tex->bind ();

    // Pointer to OpenGL commands
    int *pGlcmds = _glcmds;
    int i;

    while ((i = *(pGlcmds++)) != 0)
        {
            if (i < 0)
    {
        glBegin (GL_TRIANGLE_FAN);
        i = -i;
    }
            else
    {
        glBegin (GL_TRIANGLE_STRIP);
    }

            // Parse all OpenGL commands of this group
            for (/* nothing *//*; i > 0; --i, pGlcmds += 3)
    {
        // pGlcmds[0] : final S texture coord.
        // pGlcmds[1] : final T texture coord.
        // pGlcmds[2] : vertex index to draw

        MD2Glcmd_t *pGLcmd = reinterpret_cast<MD2Glcmd_t *>(pGlcmds);

        MD2Frame_t *pFrameA = &_frames[frameA];
        MD2Frame_t *pFrameB = &_frames[frameB];

        MD2Vertex_t *pVertA = &pFrameA->verts[pGLcmd->index];
        MD2Vertex_t *pVertB = &pFrameB->verts[pGLcmd->index];

        // Send texture coords. to OpenGL
        glTexCoord2f (pGLcmd->s, 1.0f - pGLcmd->t);

        // Compute interpolated normal vector
        const GLfloat *normA = _kAnorms [pVertA->normalIndex];
        const GLfloat *normB = _kAnorms [pVertB->normalIndex];

        vec3 n;
        n[0] = normA[0] + interp * (normB[0] - normA[0]);
        n[1] = normA[1] + interp * (normB[1] - normA[1]);
        n[2] = normA[2] + interp * (normB[2] - normA[2]);

        glNormal3fv (n);

        // Compute final vertex position
        vec3 vecA, vecB, v;

        // First, uncompress vertiex positions
        vecA[0] = pFrameA->scale[0] * pVertA->v[0] + pFrameA->translate[0];
        vecA[1] = pFrameA->scale[1] * pVertA->v[1] + pFrameA->translate[1];
        vecA[2] = pFrameA->scale[2] * pVertA->v[2] + pFrameA->translate[2];

        vecB[0] = pFrameB->scale[0] * pVertB->v[0] + pFrameB->translate[0];
        vecB[1] = pFrameB->scale[1] * pVertB->v[1] + pFrameB->translate[1];
        vecB[2] = pFrameB->scale[2] * pVertB->v[2] + pFrameB->translate[2];

        // Linear interpolation and scaling
        v[0] = (vecA[0] + interp * (vecB[0] - vecA[0])) * _scale;
        v[1] = (vecA[1] + interp * (vecB[1] - vecA[1])) * _scale;
        v[2] = (vecA[2] + interp * (vecB[2] - vecA[2])) * _scale;

        glVertex3fv (v);
    }

            glEnd();
        }
}


/////////////////////////////////////////////////////////////////////////////
//
// class MD2Object implementation.
//
/////////////////////////////////////////////////////////////////////////////

// --------------------------------------------------------------------------
// MD2Object::MD2Object
//
// Constructors.
// --------------------------------------------------------------------------

MD2Object::MD2Object ()
    : _model (NULL), _currFrame (0), _nextFrame (0),
        _interp (0.0f), _percent (0.0f), _scale (1.0f)
{
}


MD2Object::MD2Object (MD2MeshLoader *model)
    : _model (model), _currFrame (0), _nextFrame (0),
        _interp (0.0f), _percent (0.0f), _scale (1.0f)
{
    setModel (model);
}


// --------------------------------------------------------------------------
// MD2Object::~MD2Object
//
// Destructor.
// --------------------------------------------------------------------------

MD2Object::~MD2Object ()
{
}


// --------------------------------------------------------------------------
// MD2Object::drawObjectItp
//
// Draw the MD2 object with frame interpolation.
// --------------------------------------------------------------------------

void
MD2Object::drawObjectItp (bool animated, MD2RenderMode renderMode)
{
    glPushMatrix ();
        // Axis rotation
        glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);

        // Set model scale factor
        _model->setScale (_scale);

        glPushAttrib (GL_POLYGON_BIT);
        glFrontFace (GL_CW);

        // Render the model
        switch (renderMode)
            {
            case kDrawImmediate:
    _model->drawModelItpImmediate (_currFrame, _nextFrame, _interp);
    break;

            case kDrawGLcmds:
    _model->drawModelItpWithGLcmds (_currFrame, _nextFrame, _interp);
    break;
            }

        // GL_POLYGON_BIT
        glPopAttrib ();
    glPopMatrix ();

    if (animated)
        {
            // Increase interpolation percent
            _interp += _percent;
        }
}


// --------------------------------------------------------------------------
// MD2Object::drawObjectFrame
//
// Draw the MD2 object for the specified frame.
// --------------------------------------------------------------------------

void
MD2Object::drawObjectFrame (int frame, MD2RenderMode renderMode)
{
    glPushMatrix ();
        // Axis rotation
        glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
        glRotatef (-90.0f, 0.0f, 0.0f, 1.0f);

        // Set model scale factor
        _model->setScale (_scale);

        glPushAttrib (GL_POLYGON_BIT);
        glFrontFace (GL_CW);

        // Render the model
        switch (renderMode)
            {
            case kDrawImmediate:
    _model->renderFrameImmediate (frame);
    break;

            case kDrawGLcmds:
    _model->renderFrameWithGLcmds (frame);
    break;
            }

        // GL_POLYGON_BIT
        glPopAttrib ();
    glPopMatrix ();
}


// --------------------------------------------------------------------------
// MD2Object::Animate
//
// Animate the object.  Compute current and next frames, and the
// interpolation percent.
// --------------------------------------------------------------------------

void
MD2Object::animate (int startFrame, int endFrame, float percent)
{
    // _currFrame must range between startFrame and endFrame
    if (_currFrame < startFrame)
        _currFrame = startFrame;

    if (_currFrame > endFrame)
        _currFrame = startFrame;

    _percent = percent;

    // Compute current and next frames.
    if (_interp >= 1.0)
        {
            _interp = 0.0f;
            _currFrame++;

            if (_currFrame >= endFrame)
    _currFrame = startFrame;

            _nextFrame = _currFrame + 1;

            if (_nextFrame >= endFrame)
    _nextFrame = startFrame;
        }
}


void
MD2Object::animate (float percent)
{
    // Use the current animation
    animate (_animInfo->start, _animInfo->end, percent);
}


// --------------------------------------------------------------------------
// MD2Object::setModel
//
// Attach mesh model to object.
// --------------------------------------------------------------------------

void
MD2Object::setModel (MD2MeshLoader *model)
{
    _model = model;

    if (_model)
        {
            // Set first animation as default animation
            _animInfo = &_model->anims ().begin ()->second;
            _currentAnim = _model->anims ().begin ()->first;
        }
}


// --------------------------------------------------------------------------
// MD2Object::setAnim
//
// Set current object animation.
// --------------------------------------------------------------------------

void
MD2Object::setAnim (const std::string &name)
{
    // Try to find the desired animation
    MD2MeshLoader::AnimMap::const_iterator itor;
    itor = _model->anims ().find (name);

    if (itor != _model->anims ().end ())
        {
            _animInfo = &itor->second;
            _currentAnim = name;
        }
}
*/

} // end of namespace 'crashburn'