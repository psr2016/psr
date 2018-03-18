/*
 * Gear.cpp
 */

#include <math.h>
#include <QtMath>
#include <QDebug>

#include "Gear.h"

#define RADIUS 0.1
#define HEIGHT 0.01

static GLfloat pos[4] = { 5.0, 5.0, 10.0, 0.0 };
static GLfloat red[4] = { 1.0, 0.2, 0.2, 1.0 };
static GLfloat green[4] = { 0.0, 0.8, 0.2, 1.0 };
static GLfloat blue[4] = { 0.2, 0.2, 1.0, 1.0 };
static GLfloat white[4] = { 1.0, 1.0, 1.0, 1.0 };

static GLfloat view_rotx = 20.0, view_roty = 30.0, view_rotz = 0.0;
static GLfloat angle = 0.0;

Gear::Gear(QOpenGLFunctions_2_1 * ogl_link, Core::World * world, float sampling_time)
    : Body(ogl_link, world)
{

    omega_pid = new Pid(world->settings()->value("Pid/kp", 0).toFloat(),
                        world->settings()->value("Pid/ki", 0).toFloat(),
                        world->settings()->value("Pid/kd", 0).toFloat(),
                        0, sampling_time, 0);

    m_input = new Input(world, sampling_time);
}

void Gear::init()
{
   m_ogl_link->glLightfv(GL_LIGHT0, GL_POSITION, pos);
   //m_ogl_link->glEnable(GL_CULL_FACE);
   m_ogl_link->glEnable(GL_LIGHTING);
   m_ogl_link->glEnable(GL_LIGHT0);
   m_ogl_link->glEnable(GL_DEPTH_TEST);

   /* make the gears */
   m_gear = m_ogl_link->glGenLists(1);
   m_ogl_link->glNewList(m_gear, GL_COMPILE);
   make_gear(1.0, 4.0, 1.0, 20, 0.7);
   m_ogl_link->glEndList();


   // make the rigit body
   m_gearshape = new btCylinderShape(btVector3(RADIUS, RADIUS, HEIGHT/2.0));
   btRigidBody::btRigidBodyConstructionInfo ctorinfo(10 /* kg */, NULL, m_gearshape);
   m_gearshape->calculateLocalInertia(ctorinfo.m_mass, ctorinfo.m_localInertia);

   btTransform transf;
   transf.setOrigin(btVector3(0,0,0));

   ctorinfo.m_startWorldTransform = transf;
   ctorinfo.m_linearDamping = .3;
   ctorinfo.m_angularDamping = .9;
   ctorinfo.m_friction = 0.5;
   ctorinfo.m_rollingFriction = 0;

   m_rigidbody = new btRigidBody(ctorinfo);
   //m_bulletRigidBody->setUserPointer((Agent*)this);
   m_world->getBulletWorld()->addRigidBody(m_rigidbody);
   m_world->addPreTickListener(this);

}


void Gear::draw()
{
   m_ogl_link->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   m_ogl_link->glPushMatrix();
   m_ogl_link->glRotatef(view_rotx, 1.0, 0.0, 0.0);
   m_ogl_link->glRotatef(view_roty, 0.0, 1.0, 0.0);
   m_ogl_link->glRotatef(view_rotz, 0.0, 0.0, 1.0);

   m_ogl_link->glPushMatrix();
   m_ogl_link->glTranslatef(0.0, 0.0, 0.0);
   m_ogl_link->glRotatef(angle, 0.0, 0.0, 1.0);
   m_ogl_link->glCallList(m_gear);
   m_ogl_link->glPopMatrix();

   m_ogl_link->glPopMatrix();
}

void Gear::reshape(int width, int height)
{
   GLfloat h = (GLfloat) height / (GLfloat) width;

   m_ogl_link->glViewport(0, 0, (GLint) width, (GLint) height);
   m_ogl_link->glMatrixMode(GL_PROJECTION);
   m_ogl_link->glLoadIdentity();
   m_ogl_link->glFrustum(-1.0, 1.0, -h, h, 5.0, 60.0);
   m_ogl_link->glMatrixMode(GL_MODELVIEW);
   m_ogl_link->glLoadIdentity();
   m_ogl_link->glTranslatef(0.0, 0.0, -40.0);
}

void Gear::preTick(float dt)
{
    btScalar roll, pitch, yaw;
    m_rigidbody->getWorldTransform().getBasis().getEulerYPR(yaw, pitch, roll);
    angle = qRadiansToDegrees(yaw);
    float v = m_input->value();
    qCritical() << (v - get_omega_current());
    //m_rigidbody->applyTorque(btVector3(0, 0, 2));
    m_torque = omega_pid->evaluate(v, get_omega_current());
    m_rigidbody->applyTorque(btVector3(0, 0, m_torque));
}

float Gear::get_omega_current()
{
    btVector3 speed = m_rigidbody->getAngularVelocity();
    return speed[2];
}


void Gear::make_gear(GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
     GLint teeth, GLfloat tooth_depth)
{
   GLint i;
   GLfloat r0, r1, r2;
   GLfloat angle, da;
   GLfloat u, v, len;

   r0 = inner_radius;
   r1 = outer_radius - tooth_depth / 2.0;
   r2 = outer_radius + tooth_depth / 2.0;

   da = 2.0 * M_PI / teeth / 4.0;

   m_ogl_link->glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);

   m_ogl_link->glShadeModel(GL_FLAT);

   m_ogl_link->glNormal3f(0.0, 0.0, 1.0);

   /* draw front face */
   m_ogl_link->glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      m_ogl_link->glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
      m_ogl_link->glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      if (i < teeth) {
	 m_ogl_link->glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
	 m_ogl_link->glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		    width * 0.5);
      }
   }
   m_ogl_link->glEnd();

   /* draw front sides of teeth */
   m_ogl_link->glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      m_ogl_link->glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      m_ogl_link->glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      m_ogl_link->glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 width * 0.5);
      m_ogl_link->glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 width * 0.5);
   }
   m_ogl_link->glEnd();

   m_ogl_link->glNormal3f(0.0, 0.0, -1.0);

   /* draw back face */
   m_ogl_link->glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      m_ogl_link->glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      m_ogl_link->glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      if (i < teeth) {
	 m_ogl_link->glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		    -width * 0.5);
	 m_ogl_link->glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      }
   }
   m_ogl_link->glEnd();

   /* draw back sides of teeth */
   m_ogl_link->glBegin(GL_QUADS);
   da = 2.0 * M_PI / teeth / 4.0;
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      m_ogl_link->glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 -width * 0.5);
      m_ogl_link->glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 -width * 0.5);
      m_ogl_link->glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      m_ogl_link->glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
   }
   m_ogl_link->glEnd();

   /* draw outward faces of teeth */
   m_ogl_link->glBegin(GL_QUAD_STRIP);
   for (i = 0; i < teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;

      m_ogl_link->glVertex3f(r1 * cos(angle), r1 * sin(angle), width * 0.5);
      m_ogl_link->glVertex3f(r1 * cos(angle), r1 * sin(angle), -width * 0.5);
      u = r2 * cos(angle + da) - r1 * cos(angle);
      v = r2 * sin(angle + da) - r1 * sin(angle);
      len = sqrt(u * u + v * v);
      u /= len;
      v /= len;
      m_ogl_link->glNormal3f(v, -u, 0.0);
      m_ogl_link->glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), width * 0.5);
      m_ogl_link->glVertex3f(r2 * cos(angle + da), r2 * sin(angle + da), -width * 0.5);
      m_ogl_link->glNormal3f(cos(angle), sin(angle), 0.0);
      m_ogl_link->glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 width * 0.5);
      m_ogl_link->glVertex3f(r2 * cos(angle + 2 * da), r2 * sin(angle + 2 * da),
		 -width * 0.5);
      u = r1 * cos(angle + 3 * da) - r2 * cos(angle + 2 * da);
      v = r1 * sin(angle + 3 * da) - r2 * sin(angle + 2 * da);
      m_ogl_link->glNormal3f(v, -u, 0.0);
      m_ogl_link->glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 width * 0.5);
      m_ogl_link->glVertex3f(r1 * cos(angle + 3 * da), r1 * sin(angle + 3 * da),
		 -width * 0.5);
      m_ogl_link->glNormal3f(cos(angle), sin(angle), 0.0);
   }

   m_ogl_link->glVertex3f(r1 * cos(0), r1 * sin(0), width * 0.5);
   m_ogl_link->glVertex3f(r1 * cos(0), r1 * sin(0), -width * 0.5);

   m_ogl_link->glEnd();

   m_ogl_link->glShadeModel(GL_SMOOTH);

   /* draw inside radius cylinder */
   m_ogl_link->glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= teeth; i++) {
      angle = i * 2.0 * M_PI / teeth;
      m_ogl_link->glNormal3f(-cos(angle), -sin(angle), 0.0);
      m_ogl_link->glVertex3f(r0 * cos(angle), r0 * sin(angle), -width * 0.5);
      m_ogl_link->glVertex3f(r0 * cos(angle), r0 * sin(angle), width * 0.5);
   }
   m_ogl_link->glEnd();
}


