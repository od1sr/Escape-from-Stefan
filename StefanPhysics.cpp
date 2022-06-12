#include "StefanPhysics.h"
#include "config.h"

using namespace sgl;

btBroadphaseInterface *StefanPhysics::broadphase;
btDefaultCollisionConfiguration *StefanPhysics::collision_configuration;
btCollisionDispatcher *StefanPhysics::dispatcher;
btSequentialImpulseConstraintSolver *StefanPhysics::solver;
btDiscreteDynamicsWorld *StefanPhysics::world;

bool sgl::StefanPhysics::collideCallbackHandler(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap, 
    int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1
)
{
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

void sgl::StefanPhysics::addRigidBody(btRigidBody *body)
{
    world->addRigidBody(body);
}

void sgl::StefanPhysics::removeCollisonObject(btCollisionObject *body)
{
    world->removeCollisionObject(body);
}

void StefanPhysics::terminate()
{
    delete broadphase;
    delete collision_configuration;
    delete dispatcher;
    delete solver;
    delete world;
}