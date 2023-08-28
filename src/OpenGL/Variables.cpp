#include <Graphics/Variables.h>
#include <GL/glew.h>

const unsigned short Gum::Graphics::Datatypes::UNSIGNED_CHAR  = GL_UNSIGNED_BYTE;
const unsigned short Gum::Graphics::Datatypes::FLOAT          = GL_FLOAT;
const unsigned short Gum::Graphics::Datatypes::INTEGER        = GL_INT;
const unsigned short Gum::Graphics::Datatypes::UINTEGER       = GL_UNSIGNED_INT;

const unsigned short Gum::Graphics::DataState::STATIC         = GL_STATIC_DRAW;
const unsigned short Gum::Graphics::DataState::DYNAMIC        = GL_DYNAMIC_DRAW;

const unsigned short Gum::Graphics::Features::SCISSOR_TEST    = GL_SCISSOR_TEST;
const unsigned short Gum::Graphics::Features::BLENDING        = GL_BLEND;
const unsigned short Gum::Graphics::Features::CULL_FACE       = GL_CULL_FACE;
const unsigned short Gum::Graphics::Features::DEPTH_TESTING   = GL_DEPTH_TEST;
const unsigned short Gum::Graphics::Features::STENCIL_TESTING = GL_STENCIL_TEST;