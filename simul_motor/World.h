
/* THANKS TO FABIO D'URSO */

#ifndef LIBCORE_WORLD_H
#define LIBCORE_WORLD_H

#include <QtGlobal>
#include <QSettings>

#include <set>

#include <LinearMath/btScalar.h>

class btDiscreteDynamicsWorld;
class btDynamicsWorld;
class btBroadphaseInterface;
class btCollisionConfiguration;
class btCollisionDispatcher;
class btCollisionObject;
class btGhostPairCallback;
class btSequentialImpulseConstraintSolver;
class btVector3;
class BulletDebugDrawer;

namespace Core
{

class World
{
	public:
		World(QSettings * settings);
		~World();

                QSettings * settings() { return m_settings; };

		btDynamicsWorld *getBulletWorld() const;
		btCollisionDispatcher *getBulletCollisionDispatcher() const;
		qulonglong getElapsedTicks() const { return m_elapsedTicks; }

		bool testCollision(btCollisionObject *objA,
		                   btCollisionObject *objB) const;
		bool testRay(btCollisionObject *exclude,
		             const btVector3 &from, const btVector3 &to) const;

		class PreTickListener
		{
			public:
				virtual void preTick(float dt) = 0;
		};

		void addPreTickListener(PreTickListener *listener);
		void removePreTickListener(PreTickListener *listener);

		class PostTickListener
		{
			public:
				virtual void postTick(float dt) = 0;
		};

		void addPostTickListener(PostTickListener *listener);
		void removePostTickListener(PostTickListener *listener);

	private:
		static void preTickCallback(btDynamicsWorld *world, btScalar dt);
		static void postTickCallback(btDynamicsWorld *world, btScalar dt);

                QSettings * m_settings;

		btBroadphaseInterface *m_bulletBroadphase;
		btCollisionConfiguration *m_bulletCollisionConfig;
		btCollisionDispatcher *m_bulletCollisionDispatcher;
		btSequentialImpulseConstraintSolver *m_bulletSolver;
		btDiscreteDynamicsWorld *m_bulletWorld;
		btGhostPairCallback *m_ghostPairCallback;
		qulonglong m_elapsedTicks;

		std::set<PreTickListener*> m_preTickListeners;
		std::set<PostTickListener*> m_postTickListeners;
};

}

#endif // LIBCORE_WORLD_H
