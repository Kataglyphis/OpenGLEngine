#pragma once
#include "ShadowMap.h"
class OmniShadowMap :
    public ShadowMap
{
public:
    OmniShadowMap();

    virtual bool init(GLuint width, GLuint height);

    void write();

    void read(GLenum texture_unit);

    ~OmniShadowMap();
};

