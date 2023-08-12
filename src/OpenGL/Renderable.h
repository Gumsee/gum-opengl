#pragma once
#include <Maths/vec.h>

class Renderable
{

public:
    Renderable() {};
    virtual ~Renderable() {};

    virtual void onProjectionUpdate() {};
    virtual void prerender() {};
    virtual void render() {};
    virtual void renderID() {};
};