#ifndef _STEFAN_PHYSICS_CLASS_
#define _STEFAN_PHYSICS_CLASS_

#include "btBulletDynamicsCommon.h"

namespace sgl
{

	class StefanPhysics
	{
	private:
		static btBroadphaseInterface *broadphase;
		static btDefaultCollisionConfiguration *collision_configuration;
		static btCollisionDispatcher *dispatcher;
		static btSequentialImpulseConstraintSolver *solver;
		static btDiscreteDynamicsWorld *world;

	public:
		static bool collideCallbackHandler(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, 
			int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1);
		static void init();
		static void addRigidBody(btRigidBody *body);
		static void removeCollisonObject(btCollisionObject *body);
		static void terminate();
	};
}

#endif