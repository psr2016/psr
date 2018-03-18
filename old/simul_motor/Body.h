/*
 * Body.h
 */

#ifndef __BODY_H
#define __BODY_H

#include "World.h"
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_2_1>

class Body : public Core::World::PreTickListener {
 public:
    Body(QOpenGLFunctions_2_1 * ogl_link, Core::World * world);
    virtual void init() = 0;
    virtual void draw() = 0;
    virtual void reshape(int width, int height) = 0;
    virtual void preTick(float dt) = 0;
 protected:
    QOpenGLFunctions_2_1 * m_ogl_link;
    Core::World * m_world;
};

#endif
