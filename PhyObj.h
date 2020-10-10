#pragma once
#include "CProcessing/inc/cprocessing.h"

#define INITIAL_SIZE 3
#define RESOLUTION_ITERATIONS 5

#define MANIFOLD_CIRCLE_CIRCLE 0
#define MANIFOLD_CIRCLE_OBOX 1

#define BOUNDING_CIRCLE 11
#define BOUNDING_OBOX 12

typedef struct PhyObjBoundingShape {
	unsigned int _id; // position in shapes array
	int			_type;
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
	CP_Vector	_contact_normal;
	CP_Vector	_contact_position;
	CP_Vector	_contact_position_2;
	float		_penetration;
	float		_penetration_2;
} PhyObjManifold;

extern PhyObjBoundingShape**	PhyObj_bounding_shapes;
extern int						PhyObj_bounding_shapes_size;
extern int						PhyObj_bounding_shapes_max_size;

extern PhyObjBoundingCircle*	PhyObj_bounding_circles;
extern int						PhyObj_bounding_circles_size;
extern int						PhyObj_bounding_circles_max_size;

extern PhyObjOBoundingBox*		PhyObj_bounding_obox;
extern int						PhyObj_bounding_obox_size;
extern int						PhyObj_bounding_obox_max_size;

extern PhyObjManifold*			PhyObj_manifolds;
extern int						PhyObj_manifolds_size;
extern int						PhyObj_manifolds_max_size;

extern CP_Image					PhyObj_circle_image;
extern CP_Image					PhyObj_square_image;

extern float					gravity;

/*____________________________________________________________________________________________________________________________________*/
// MISCELLANEOUS
/*____________________________________________________________________________________________________________________________________*/
void	PhyObj_Initialize();
void	PhyObj_DrawCircles();
void	PhyObj_DrawOBoxes();
void	PhyObj_SetPosition(PhyObjBoundingShape* s, CP_Vector p);
void	PhyObj_AddShape(PhyObjBoundingShape* s);
void	PhyObj_AddManifold(PhyObjManifold m);
PhyObjBoundingCircle*	PhyObj_AddCircle(const float x, const float y, const float m, const float r);
PhyObjOBoundingBox*		PhyObj_AddOBox(const float x, const float y, const float m, const float w, const float h);

void	PhyObj_Update(const float dt);
void	PhyObj_Render();
void	PhyObj_Free();

/*____________________________________________________________________________________________________________________________________*/
// MATH
/*____________________________________________________________________________________________________________________________________*/
float	PhyObj_2DCross(const CP_Vector v1, const CP_Vector v2);
float	PhyObj_VectorSquareMagnitude(const CP_Vector v);
float	PhyObj_Mod(const float in); // always returns a positive value
CP_Vector	PhyObj_2DPerpendicular(const CP_Vector v);

/*____________________________________________________________________________________________________________________________________*/
// PHYSICS
/*____________________________________________________________________________________________________________________________________*/
void	PhyObj_AddVelocity(PhyObjBoundingShape* s, const CP_Vector v);
void	PhyObj_AddAngularVelocity(PhyObjBoundingShape* s, const float v);
void	PhyObj_ApplyImpulse(PhyObjBoundingShape* s, const CP_Vector v);
void	PhyObj_UpdatePosition(const float dt);
void	PhyObj_UpdateRotation(const float dt);
void	PhyObj_GlobalAcceleration();

/*____________________________________________________________________________________________________________________________________*/
// PRIMITIVE TESTS
/*____________________________________________________________________________________________________________________________________*/
CP_Vector PhyObj_NearestPointOnOBBToPoint(const CP_Vector p, const PhyObjOBoundingBox* b, int* point_in_box);

/*____________________________________________________________________________________________________________________________________*/
// COLLISION DETECTION
/*____________________________________________________________________________________________________________________________________*/
int		PhyObj_CircleCircle(PhyObjBoundingCircle* c1, PhyObjBoundingCircle* c2, PhyObjManifold* m);
int		PhyObj_CircleOBox(PhyObjBoundingCircle* c, PhyObjOBoundingBox* b, PhyObjManifold* m);
int		PhyObj_BoxBox(PhyObjOBoundingBox* b1, PhyObjOBoundingBox* b2, PhyObjManifold* m);

/*____________________________________________________________________________________________________________________________________*/
// COLLISION RESOLUTION
/*____________________________________________________________________________________________________________________________________*/
void	PhyObj_CheckForCollisions();
void	PhyObj_ResolveManifolds();
void	PhyObj_ResolveContact(const CP_Vector contact_position, const float contact_penetration, PhyObjManifold m);
void	PhyObj_IterativeSolveManifolds(const int count);

// --------------------------------------------------------//
// Some Additional Info:
// moment of inertia of a circle, I=mr^2
// --------------------------------------------------------//