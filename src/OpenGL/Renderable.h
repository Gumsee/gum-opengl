#pragma once
#include <Maths/vec.h>

class Renderable
{

public:
    Renderable();
    ~Renderable();

    virtual void onProjectionUpdate() {};
    virtual void prerender() {};
    virtual void render() {};
    virtual void renderID() {};
};