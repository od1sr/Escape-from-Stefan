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
    Player *player = NULL;
    ITransformable *other_object = NULL;
    if (obj1->getObjectID() & objectID::PLAYER)
    {
        player = (Player*)obj1;
        other_object = obj2;
    }
    else if (obj2->getObjectID() & objectID::PLAYER)
    {
        player = (Player*)obj2;
        other_object = obj1;
    }
    if (player != NULL)
    {
        glm::vec3 player_pos_glm = player->getCoordinates();
        btVector3 player_pos(player_pos_glm.x, player_pos_glm.y, player_pos_glm.z);
        float half_player_height = ((btCapsuleShape*)player->rigid_body->getCollisionShape())->getHalfHeight();
        btVector3 from = player_pos + btVector3(0.f, -half_player_height, 0.f);
        btVector3 ray = btVector3(0.f, -0.0001f, 0.f);
        btCollisionWorld::ClosestRayResultCallback callback(from, ray);
        world->rayTest(from, ray, callback);
        if (callback.hasHit())
            player->is_standing = true;
    }
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