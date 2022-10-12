#ifndef _STEFAN_PHYSICS_CLASS_
#define _STEFAN_PHYSICS_CLASS_

#include "btBulletDynamicsCommon.h"
#include <vector>
#include "ITransformable.h"

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
		static std::vector<ITransformable*> physical_objects;

	public:
		static bool collideCallbackHandler(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, int partId0, 
			int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1);
		static void init();
		static void addObject(ITransformable *object);
		static void stepSimulation();
		static void removeCollisonObject(btCollisionObject *body);
		static void terminate();
		static void rayTest(btCollisionWorld::ClosestRayResultCallback &raytest_callback);
	};
}

#endif