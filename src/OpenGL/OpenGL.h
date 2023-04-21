#pragma once
#include <Desktop/Window.h>
#include "Framebuffer.h"
#include "System/Output.h"

namespace Gum {
namespace OpenGL
{
    void addFramebufferToWindow(Gum::Window* window)
    {
        Framebuffer* framebuffer = new Framebuffer(window->getSize(), true);
        framebuffer->bind();
        framebuffer->resetViewport();

        window->setFramebufferCallback([framebuffer](ivec2 size, int mode) {
            switch(mode)
            {
                case 0:
                    framebuffer->setSize(size);
                    framebuffer->resetViewport();
                    break;
                case 1:
                    framebuffer->bind();
            };   
        });
    }
}}