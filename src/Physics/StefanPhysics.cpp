#include "StefanPhysics.h"
#include "config.h"
#include "Player.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

using namespace sgl;

btBroadphaseInterface *StefanPhysics::broadphase;
btDefaultCollisionConfiguration *StefanPhysics::collision_configuration;
btCollisionDispatcher *StefanPhysics::dispatcher;
btSequentialImpulseConstraintSolver *StefanPhysics::solver;
btDiscreteDynamicsWorld *StefanPhysics::world;
std::vector<ITransformable*> StefanPhysics::physical_objects;

bool sgl::StefanPhysics::collideCallbackHandler(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, 
    int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1)
{
    ITransformable *obj1 = (ITransformable*)colObj0Wrap->getCollisionObject()->getUserPointer();
    ITransformable *obj2 = (ITransformable*)colObj1Wrap->getCollisionObject()->getUserPointer();
    obj1->collideCallback(obj2, cp);
    obj2->collideCallback(obj1, cp);
    return false;
}

void StefanPhysics::init()
{
    broadphase = new btDbvtBroadphase();
    collision_configuration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collision_configuration);
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);
    world->setGravity(WORLD_GRAVITY);
    gContactAddedCallback = collideCallbackHandler;
}

void sgl::StefanPhysics::stepSimulation()
{
    world->stepSimulation(1.f / 60.f, 10);
    for (int i = physical_objects.size() - 1; i >= 0; --i)
        physical_objects[i]->update();
}

void sgl::StefanPhysics::addObject(ITransformable *object)
{
    world->addRigidBody(object->rigid_body);
    physical_objects.push_back(object);
}

void sgl::StefanPhysics::removeCollisonObject(btCollisionObject *body)
{
    world->removeCollisionObject(body);
}

void StefanPhysics::terminate()
{
    delete world;
    delete broadphase;
    delete collision_configuration;
    delete dispatcher;
    delete solver;
}

void sgl::StefanPhysics::rayTest(btCollisionWorld::ClosestRayResultCallback &raytest_callback)
{
    world->rayTest(raytest_callback.m_rayFromWorld, raytest_callback.m_rayToWorld, raytest_callback);
}
