#include <Graphics/Graphics.h>
#include <GL/glew.h>

namespace Gum {
namespace Graphics
{
    void enableFeature(const unsigned int& feature)
    {
        glEnable(feature);
    }

    void disableFeature(const unsigned int& feature)
    {
        glDisable(feature);
    }

    void renderWireframe(const bool wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    void cullBackside(const bool cullback)
    {
        glCullFace(cullback ? GL_BACK : GL_FRONT);
    }
}}