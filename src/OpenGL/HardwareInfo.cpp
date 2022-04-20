#include "HardwareInfo.h"
#include <GL/glew.h>

namespace Gum
{
    namespace HardwareInfo
    {
        int MAX_TEXTURE_SIZE = 0;


        void init()
        {
            glGetIntegerv(GL_MAX_TEXTURE_SIZE, &MAX_TEXTURE_SIZE);
        }
    };
};