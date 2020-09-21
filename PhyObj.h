#pragma once
#include "CProcessing/inc/cprocessing.h"

#define INITIAL_SIZE 3

#define MANIFOLD_CIRCLE_CIRCLE 0
#define MANIFOLD_CIRCLE_OBOX 1

#define BOUNDING_CIRCLE 11
#define BOUNDING_OBOX 12

typedef struct PhyObjBoundingCircle {
	CP_Vector	_position;
	float		_rotation;
	CP_Vector	_velocity;
	float		_angular_velocity;	// is in radians per seconds
	float		_mass;
	float		_inv_mass;
	float		_moment_of_inertia;
	float		_inv_moment_of_inertia;
	float		_radius;
} PhyObjBoundingCircle;

typedef struct PhyObjOBoundingBox {
	CP_Vector	_position;
	float		_rotation;
	CP_Vector	_velocity;
	float		_angular_velocity;	// is in radians per seconds
	float		_mass;
	float		_inv_mass;
	float		_moment_of_inertia;
	float		_inv_moment_of_inertia;
	float		_horizontal_extent;
	float		_vertical_extent;
} PhyObjOBoundingBox;

typedef struct PhyObjManifold {
	//int type;
	PhyObjBoundingCircle* A;
	PhyObjBoundingCircle* B;
	CP_Vector _contact_normal;
	CP_Vector _contact_position;
	float _penetration;
} PhyObjManifold;

//typedef struct PhyObjManifoldCvC {
//	PhyObjManifold super;
//	PhyObjBoundingCircle* A;
//	PhyObjOBoundingBox* B;
//} PhyObjManifoldCvC;

extern PhyObjBoundingCircle* PhyObj_bounding_circles;
extern int PhyObj_bounding_circles_size;
extern int PhyObj_bounding_circles_max_size;

extern PhyObjOBoundingBox* PhyObj_bounding_obox;
extern int PhyObj_bounding_obox_size;
extern int PhyObj_bounding_obox_max_size;

extern PhyObjManifold* PhyObj_manifolds;
extern int PhyObj_manifolds_size;
extern int PhyObj_manifolds_max_size;

extern CP_Image PhyObj_circle_image;
extern CP_Image PhyObj_square_image;

// Miscellaneous
void PhyObj_Initialize();
void PhyObj_DrawCircles();
void PhyObj_DrawOBoxes();
PhyObjBoundingCircle* PhyObj_AddCircle(const float x, const float y, const float m, const float r);
PhyObjOBoundingBox* PhyObj_AddOBox(const float x, const float y, const float m, const float w, const float h);
void PhyObj_AddManifold(PhyObjManifold m);
void PhyObj_SetPosition(PhyObjBoundingCircle* c, CP_Vector p);
// Math
float PhyObj_2DCross(const CP_Vector v1, const CP_Vector v2);
CP_Vector PhyObj_2DPerpendicular(const CP_Vector v);
// Physics
void PhyObj_AddVelocity(PhyObjBoundingCircle* c, const CP_Vector v);
void PhyObj_AddAngularVelocity(PhyObjBoundingCircle* c, const float v);
void PhyObj_ApplyImpulse(PhyObjBoundingCircle* c, const CP_Vector v);
void PhyObj_UpdatePosition(const float dt);
void PhyObj_UpdateRotation(const float dt);
void PhyObj_GlobalAcceleration();
// Primitive Tests
CP_Vector PhyObj_NearestPointOnOBBToPoint(const CP_Vector p, const PhyObjOBoundingBox* b);
// Collision Detection
void PhyObj_CircleCircle(PhyObjBoundingCircle* c1, PhyObjBoundingCircle* c2);
int PhyObj_CircleOBox(PhyObjBoundingCircle* c, PhyObjOBoundingBox* b, PhyObjManifold* m);
// Collision Resolution
void PhyObj_CheckForCollisions();
void PhyObj_ResolveManifolds();
void PhyObj_IterativeSolveManifolds(const int count);

// --------------------------------------------------------//
// Some Additional Info:
// moment of inertia of a circle, I=mr^2
// --------------------------------------------------------//