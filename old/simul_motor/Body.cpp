/*
 * Body.cpp
 */

#include "Body.h"

Body::Body(QOpenGLFunctions_2_1 * ogl_link, Core::World * world)
    : m_ogl_link(ogl_link), m_world(world)
{
}

