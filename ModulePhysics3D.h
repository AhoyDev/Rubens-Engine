#ifndef __MODULEPHYSICS_H__
#define __MODULEPHYSICS_H__

#include "Module.h"
#include <list>
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"


// Recommended scale is 1.0f == 1 meter, no less than 0.2 objects
#define GRAVITY btVector3(0.0f, -10.0f, 0.0f) 

struct PhysBody3D;
struct PhysVehicle3D;

class DebugDrawer;

class ModulePhysics3D : public Module
{
public:
	ModulePhysics3D(const char* name, bool start_enabled = true);
	~ModulePhysics3D();

	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	PhysBody3D*		AddBody(const PrimCube& cube, float mass = 1.0f);
	PhysBody3D*		AddBody(const PrimSphere& sphere, float mass = 1.0f);
	PhysBody3D*		AddBody(const PrimCylinder& cylinder, float mass = 1.0f);
	PhysBody3D*		AddBody(const PrimPlane& plane);
	PhysBody3D*		AddHeighField(const char* filename, int width, int height);

	void DeleteBody(PhysBody3D* body);

private:

	bool debug;

	btDefaultCollisionConfiguration*	collision_conf;
	btCollisionDispatcher*				dispatcher;
	btBroadphaseInterface*				broad_phase;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld*			world;
	btDefaultVehicleRaycaster*			vehicle_raycaster;
	DebugDrawer*						debug_draw;

	std::list<btCollisionShape*> shapes;
	std::list<PhysBody3D*> bodies;

};

class DebugDrawer : public btIDebugDraw
{
public:
	DebugDrawer() : line(0, 0, 0)
	{}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	void reportErrorWarning(const char* warningString);
	void draw3dText(const btVector3& location, const char* textString);
	void setDebugMode(int debugMode);
	int	 getDebugMode() const;

	DebugDrawModes mode;
	PrimLine line;
	Primitive point;
};

#endif