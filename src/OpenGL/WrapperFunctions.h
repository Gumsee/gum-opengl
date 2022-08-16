#pragma once
#include <GL/glew.h>
#include <gum-maths.h>

extern bool gumTexImage2D(const unsigned int& target, const int& level, const int& internalformat, ivec2 size, const int& border, const unsigned int& format, const unsigned int& type, const void* pixels);
extern bool gumPixelStorei(const GLenum& pname, const GLint& param);
extern bool gumGenTextures(const GLsizei& n, GLuint* textures);