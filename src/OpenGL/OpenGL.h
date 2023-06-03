#pragma once
#include <Desktop/Window.h>
#include <string>
#include "Framebuffer.h"
#include "System/MemoryManagement.h"
#include "System/Output.h"

namespace Gum {
namespace OpenGL
{
    static void addFramebufferToWindow(Gum::Window* window)
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
                    break;

                case 2:
                    delete framebuffer;
                    break;
            };
        });
    }
}}