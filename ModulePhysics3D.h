#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2DynArray.h"
#include "Primitive.h"
#include "Bullet/src/btBulletDynamicsCommon.h"

// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects

#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btRigidBody;
class DebugDrawer;
struct PhysBody3D;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(Application* app, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	PhysBody3D* AddBody(const Cube& cube, float mass = 1.0f);
	PhysBody3D* AddBody(const Sphere& sphere, float mass = 1.0f);
	PhysBody3D* AddBody(const Cylinder& cylinder, float mass = 1.0f);
	PhysBody3D* AddBody(const Plane& plane);

private:

	bool debug;

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld*			world;
	DebugDrawer*						debug_draw;

	p2DynArray<PhysBody3D*> bodies;
};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0,0,0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	Line line;
	Primitive point;
};