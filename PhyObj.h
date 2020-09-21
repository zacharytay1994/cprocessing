#pragma once
#include "CProcessing/inc/cprocessing.h"

#define INITIAL_SIZE 10

#define MANIFOLD_CIRCLE_CIRCLE 0
#define MANIFOLD_CIRCLE_OBOX 1

#define BOUNDING_CIRCLE 11
#define BOUNDING_OBOX 12

typedef struct PhyObjBoundingShape {
	unsigned int _id; // position in shapes array
	CP_Vector	_position;
	float		_rotation;
	CP_Vector	_velocity;
	float		_angular_velocity;	// is in radians per seconds
	float		_mass;
	float		_inv_mass;
	float		_moment_of_inertia;
	float		_inv_moment_of_inertia;
} PhyObjBoundingShape;

typedef struct PhyObjBoundingCircle {
	PhyObjBoundingShape super;
	float		_radius;
} PhyObjBoundingCircle;

typedef struct PhyObjOBoundingBox {
	PhyObjBoundingShape super;
	float		_horizontal_extent;
	float		_vertical_extent;
} PhyObjOBoundingBox;

typedef struct PhyObjManifold {
	PhyObjBoundingShape* A;
	PhyObjBoundingShape* B;
	CP_Vector _contact_normal;
	CP_Vector _contact_position;
	float _penetration;
} PhyObjManifold;

extern PhyObjBoundingShape** PhyObj_bounding_shapes;
extern int PhyObj_bounding_shapes_size;
extern int PhyObj_bounding_shapes_max_size;

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
void PhyObj_AddShape(PhyObjBoundingShape* s);
PhyObjBoundingCircle* PhyObj_AddCircle(const float x, const float y, const float m, const float r);
PhyObjOBoundingBox* PhyObj_AddOBox(const float x, const float y, const float m, const float w, const float h);
void PhyObj_AddManifold(PhyObjManifold m);
void PhyObj_SetPosition(PhyObjBoundingShape* s, CP_Vector p);
// Math
float PhyObj_2DCross(const CP_Vector v1, const CP_Vector v2);
CP_Vector PhyObj_2DPerpendicular(const CP_Vector v);
// Physics
void PhyObj_AddVelocity(PhyObjBoundingShape* s, const CP_Vector v);
void PhyObj_AddAngularVelocity(PhyObjBoundingShape* s, const float v);
void PhyObj_ApplyImpulse(PhyObjBoundingShape* s, const CP_Vector v);
void PhyObj_UpdatePosition(const float dt);
void PhyObj_UpdateRotation(const float dt);
void PhyObj_GlobalAcceleration();
// Primitive Tests
CP_Vector PhyObj_NearestPointOnOBBToPoint(const CP_Vector p, const PhyObjOBoundingBox* b);
// Collision Detection
int PhyObj_CircleCircle(PhyObjBoundingCircle* c1, PhyObjBoundingCircle* c2, PhyObjManifold* m);
int PhyObj_CircleOBox(PhyObjBoundingCircle* c, PhyObjOBoundingBox* b, PhyObjManifold* m);
int PhyObj_OboxObox(PhyObjOBoundingBox* b1, PhyObjOBoundingBox* b2);
// Collision Resolution
void PhyObj_CheckForCollisions();
void PhyObj_ResolveManifolds();
void PhyObj_IterativeSolveManifolds(const int count);

// --------------------------------------------------------//
// Some Additional Info:
// moment of inertia of a circle, I=mr^2
// --------------------------------------------------------//