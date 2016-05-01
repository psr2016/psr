
/* THANKS TO FABIO D'URSO */

#include "World.h"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace Core
{

World::World(QSettings * settings)
    : m_settings(settings), m_elapsedTicks(0)
{
	// Initialize world
	m_bulletBroadphase = new btDbvtBroadphase();
	m_bulletCollisionConfig = new btDefaultCollisionConfiguration();
	m_bulletCollisionDispatcher = new btCollisionDispatcher(m_bulletCollisionConfig);
	m_bulletSolver = new btSequentialImpulseConstraintSolver();
	m_bulletWorld = new btDiscreteDynamicsWorld(m_bulletCollisionDispatcher,
	                                            m_bulletBroadphase,
	                                            m_bulletSolver,
	                                            m_bulletCollisionConfig);
	m_bulletWorld->setGravity(btVector3(0, 0, 9.81));

	// Enable ghosts
	m_ghostPairCallback = new btGhostPairCallback();
	m_bulletWorld->getPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);

	// Setup pre/post-tick callbacks
	m_bulletWorld->setInternalTickCallback(preTickCallback, 0, true);
	m_bulletWorld->setInternalTickCallback(postTickCallback, 0, false);
	m_bulletWorld->setWorldUserInfo((World*)this);
}

World::~World()
{
	delete m_bulletWorld;
	delete m_bulletSolver;
	delete m_bulletCollisionDispatcher;
	delete m_bulletCollisionConfig;
	delete m_bulletBroadphase;
	delete m_ghostPairCallback;
}

btDynamicsWorld *World::getBulletWorld() const
{
	return m_bulletWorld;
}

btCollisionDispatcher *World::getBulletCollisionDispatcher() const
{
	return m_bulletCollisionDispatcher;
}

namespace
{

class ContactSensorCallback : public btCollisionWorld::ContactResultCallback
{
	public:
		ContactSensorCallback()
		: m_hasHit(false)
		{
		}

		bool hasHit() const
		{
			return m_hasHit;
		}

		btScalar addSingleResult(btManifoldPoint &cp,
		                         const btCollisionObjectWrapper *colObj0Wrap,
		                         int partId0, int index0,
		                         const btCollisionObjectWrapper *colObj1Wrap,
		                         int partId1, int index1) override
		{
			m_hasHit = true;
			return 0;
		}

	private:
		bool m_hasHit;
};

}

bool World::testCollision(btCollisionObject *objA,
                          btCollisionObject *objB) const
{
	ContactSensorCallback cb;
	m_bulletWorld->contactPairTest(objA, objB, cb);
	return cb.hasHit();
}

namespace
{

class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback
{
	public:
		ClosestNotMeRayResultCallback(btCollisionObject *me, const btVector3 &from, const btVector3 &to)
		: btCollisionWorld::ClosestRayResultCallback(from, to), m_me(me)
		{
		}

		btScalar addSingleResult(btCollisionWorld::LocalRayResult &rayResult, bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject == m_me)
				return 1.0;
			else
				return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
		}

	protected:
		btCollisionObject* m_me;
};

}

bool World::testRay(btCollisionObject *exclude, const btVector3 &from, const btVector3 &to) const
{
	ClosestNotMeRayResultCallback res(exclude, from, to);
	m_bulletWorld->rayTest(from, to, res);
	return res.hasHit();
}

void World::addPreTickListener(World::PreTickListener *listener)
{
	m_preTickListeners.insert(listener);
}

void World::removePreTickListener(World::PreTickListener *listener)
{
	m_preTickListeners.erase(listener);
}

void World::addPostTickListener(World::PostTickListener *listener)
{
	m_postTickListeners.insert(listener);
}

void World::removePostTickListener(World::PostTickListener *listener)
{
	m_postTickListeners.erase(listener);
}

void World::preTickCallback(btDynamicsWorld *world, btScalar dt)
{
	World *w = static_cast<World*>(world->getWorldUserInfo());

	for (std::set<PreTickListener*>::iterator it = w->m_preTickListeners.begin();
			it != w->m_preTickListeners.end(); ++it)
		(*it)->preTick(dt);
}

void World::postTickCallback(btDynamicsWorld *world, btScalar dt)
{
	World *w = static_cast<World*>(world->getWorldUserInfo());

	w->m_elapsedTicks++;

	for (std::set<PostTickListener*>::iterator it = w->m_postTickListeners.begin();
			it != w->m_postTickListeners.end(); ++it)
		(*it)->postTick(dt);
}

}
