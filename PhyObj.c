#include "PhyObj.h"
#include <stdlib.h>
#include <stdio.h>

// --------------------------------------------------------//
// GLOBAL VARIABLE DEFINITIONS
// --------------------------------------------------------//
PhyObjBoundingCircle* PhyObj_bounding_circles;
int PhyObj_bounding_circles_size;
int PhyObj_bounding_circles_max_size;

PhyObjOBoundingBox* PhyObj_bounding_obox;
int PhyObj_bounding_obox_size;
int PhyObj_bounding_obox_max_size;

PhyObjManifold* PhyObj_manifolds;
int PhyObj_manifolds_size;
int PhyObj_manifolds_max_size;

CP_Image PhyObj_circle_image;
CP_Image PhyObj_square_image;

// --------------------------------------------------------//
// MISCELLANEOUS
// - functions that are not about physics or collision
// --------------------------------------------------------//
void PhyObj_Initialize()
{
	PhyObj_bounding_circles = (PhyObjBoundingCircle*)malloc(INITIAL_SIZE * sizeof(PhyObjBoundingCircle));
	PhyObj_bounding_circles_max_size = INITIAL_SIZE;
	PhyObj_bounding_circles_size = 0;

	PhyObj_bounding_obox = (PhyObjOBoundingBox*)malloc(INITIAL_SIZE * sizeof(PhyObjOBoundingBox));
	PhyObj_bounding_obox_max_size = INITIAL_SIZE;
	PhyObj_bounding_obox_size = 0;

	PhyObj_manifolds = (PhyObjManifold*)malloc(INITIAL_SIZE * sizeof(PhyObjManifold));
	PhyObj_manifolds_max_size = INITIAL_SIZE;
	PhyObj_manifolds_size = 0;

	PhyObj_circle_image = CP_Image_Load("./circle.png");
	PhyObj_square_image = CP_Image_Load("./square.png");
}

PhyObjBoundingCircle* PhyObj_AddCircle(const float x, const float y, const float m, const float r)
{
	if (++PhyObj_bounding_circles_size > PhyObj_bounding_circles_max_size) {
		PhyObjBoundingCircle* check = (PhyObjBoundingCircle*)realloc(PhyObj_bounding_circles, sizeof(PhyObjBoundingCircle) * PhyObj_bounding_circles_max_size * 2);
		PhyObj_bounding_circles_max_size *= 2;
		if (check != NULL) {
			PhyObj_bounding_circles = check;
		}
		else {
			printf("Collision.c::PhyObj_AddCircle - realloc returned null");
		}
	}
	// moment of inertia of a circle = m * r^2
	float moment_of_inertia = m * r * r;
	PhyObjBoundingCircle temp = { {x,y},0.0f,{0.0f,0.0f},0.0f,m,1.0f / m,moment_of_inertia,1 / moment_of_inertia,r };
	PhyObj_bounding_circles[PhyObj_bounding_circles_size - 1] = temp;
	return &PhyObj_bounding_circles[PhyObj_bounding_circles_size - 1];
}

PhyObjOBoundingBox* PhyObj_AddOBox(const float x, const float y, const float m, const float w, const float h)
{
	if (++PhyObj_bounding_obox_size > PhyObj_bounding_obox_max_size) {
		PhyObjOBoundingBox* check = (PhyObjOBoundingBox*)realloc(PhyObj_bounding_obox, sizeof(PhyObjOBoundingBox) * PhyObj_bounding_obox_max_size * 2);
		PhyObj_bounding_obox_max_size *= 2;
		if (check != NULL) {
			PhyObj_bounding_obox = check;
		}
		else {
			printf("Collision.c::PhyObj_AddOBox - realloc returned null");
		}
	}
	// moment of inertia of a rectangle = (w * h^3)/12
	float moment_of_inertia = (w * h * h * h) / 12;
	PhyObjOBoundingBox temp = { {x,y},0.0f,{0.0f,0.0f},0.0f,m,1.0f / m,moment_of_inertia,1 / moment_of_inertia,w,h };
	PhyObj_bounding_obox[PhyObj_bounding_obox_size - 1] = temp;
	return &PhyObj_bounding_obox[PhyObj_bounding_obox_size - 1];
}

void PhyObj_AddManifold(PhyObjManifold m)
{
	if (++PhyObj_manifolds_size > PhyObj_manifolds_max_size) {
		PhyObjManifold* check = (PhyObjManifold*)realloc(PhyObj_manifolds, sizeof(PhyObjManifold) * PhyObj_manifolds_max_size * 2);
		PhyObj_manifolds_max_size *= 2;
		if (check != NULL) {
			PhyObj_manifolds = check;
		}
		else {
			printf("Collision.c::PhyObj_AddManifold - realloc returned null");
		}
	}
	PhyObj_manifolds[PhyObj_manifolds_size - 1] = m;
}

void PhyObj_SetPosition(PhyObjBoundingCircle* c, CP_Vector p)
{
	c->_position = p;
}

float PhyObj_2DCross(const CP_Vector v1, const CP_Vector v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

CP_Vector PhyObj_2DPerpendicular(const CP_Vector v)
{
	return (CP_Vector){v.y,-v.x};
}

void PhyObj_DrawCircles()
{
	for (int i = 0; i < PhyObj_bounding_circles_size; i++) {
		CP_Image_DrawAdvanced(PhyObj_circle_image, PhyObj_bounding_circles[i]._position.x, PhyObj_bounding_circles[i]._position.y, PhyObj_bounding_circles[i]._radius * 2.0f, PhyObj_bounding_circles[i]._radius * 2.0f, 255, PhyObj_bounding_circles[i]._rotation);
	}
}

void PhyObj_DrawOBoxes()
{
	for (int i = 0; i < PhyObj_bounding_obox_size; i++) {
		CP_Image_DrawAdvanced(PhyObj_square_image, PhyObj_bounding_obox[i]._position.x, PhyObj_bounding_obox[i]._position.y, PhyObj_bounding_obox[i]._horizontal_extent * 2.0f, PhyObj_bounding_obox[i]._vertical_extent * 2.0f, 255, PhyObj_bounding_obox[i]._rotation);
	}
}

// --------------------------------------------------------//
// PHYSICS
// - physics functions, e.g. gravity, force, acceleration 
// --------------------------------------------------------//
void PhyObj_AddVelocity(PhyObjBoundingCircle* c, const CP_Vector v)
{
	c->_velocity = CP_Vector_Add(c->_velocity, v);
}

void PhyObj_AddAngularVelocity(PhyObjBoundingCircle* c, const float v)
{
	c->_angular_velocity += v;
}

void PhyObj_ApplyImpulse(PhyObjBoundingCircle* c, const CP_Vector v)
{
	PhyObj_AddVelocity(c, CP_Vector_Scale(v, c->_inv_mass));
}

void PhyObj_UpdatePosition(const float dt)
{
	for (int i = 0; i < PhyObj_bounding_circles_size; i++) {
		PhyObj_bounding_circles[i]._position = CP_Vector_Add(PhyObj_bounding_circles[i]._position, CP_Vector_Scale(PhyObj_bounding_circles[i]._velocity, dt));
	}
	for (int i = 0; i < PhyObj_bounding_obox_size; i++) {
		PhyObj_bounding_obox[i]._position = CP_Vector_Add(PhyObj_bounding_obox[i]._position, CP_Vector_Scale(PhyObj_bounding_obox[i]._velocity, dt));
	}
}

void PhyObj_UpdateRotation(const float dt)
{
	for (int i = 0; i < PhyObj_bounding_circles_size; i++) {
		// angular velocity is radians per second, but rotation takes degrees
		float angular_velocity_degrees = PhyObj_bounding_circles[i]._angular_velocity * 57.2957795130823f;
		PhyObj_bounding_circles[i]._rotation -= angular_velocity_degrees * dt;
	}
	for (int i = 0; i < PhyObj_bounding_obox_size; i++) {
		// angular velocity is radians per second, but rotation takes degrees
		float angular_velocity_degrees = PhyObj_bounding_obox[i]._angular_velocity * 57.2957795130823f;
		PhyObj_bounding_obox[i]._rotation -= angular_velocity_degrees * dt;
	}
}

void PhyObj_GlobalAcceleration()
{
	CP_Vector center = { 200.0f,200.0f };
	float magnitude = 30.0f;
	for (int i = 0; i < PhyObj_bounding_circles_size; i++) {
		// calculate vector to center
		CP_Vector vector = CP_Vector_Normalize(CP_Vector_Subtract(center, PhyObj_bounding_circles[i]._position));
		vector = CP_Vector_Scale(vector, magnitude);
		vector = CP_Vector_Scale(vector, CP_System_GetDt());
		PhyObj_AddVelocity(&PhyObj_bounding_circles[i], vector);
	}
}

CP_Vector PhyObj_NearestPointOnOBBToPoint(const CP_Vector p, const PhyObjOBoundingBox* b)
{
	CP_Vector result = b->_position;
	// rotate basis vectors
	CP_Matrix mat = CP_Matrix_Rotate(b->_rotation);
	CP_Vector u1 = CP_Vector_MatrixMultiply(mat, (CP_Vector) { 1,0 });
	CP_Vector u2 = CP_Vector_MatrixMultiply(mat, (CP_Vector) { 0,1 });
	// project point-boxposition onto axes and add to result
	CP_Vector box_to_point = CP_Vector_Subtract(p, b->_position);
	float x = CP_Vector_DotProduct(box_to_point, u1);
	float y = CP_Vector_DotProduct(box_to_point, u2);
	// check if point is in box, if so snap to nearest edge
	if (x * x < b->_horizontal_extent * b->_horizontal_extent && y * y < b->_vertical_extent * b->_vertical_extent) {
		if (x * x > y * y) {
			x = x < 0 ? -b->_horizontal_extent : b->_horizontal_extent;
		}
		else {
			y = y < 0 ? -b->_vertical_extent : b->_vertical_extent;
		}
	}
	else {
		if (x < 0) {
			x = x < -b->_horizontal_extent ? -b->_horizontal_extent : x;
		}
		else {
			x = x > b->_horizontal_extent ? b->_horizontal_extent : x;
		}
		if (y < 0) {
			y = y < -b->_vertical_extent ? -b->_vertical_extent : y;
		}
		else {
			y = y > b->_vertical_extent ? b->_vertical_extent : y;
		}
	}
	result = CP_Vector_Add(result, CP_Vector_Scale(u1, x));
	result = CP_Vector_Add(result, CP_Vector_Scale(u2, y));
	return result;
}

// --------------------------------------------------------//
// COLLISION DETECTION
// - collision detection functions
// e.g. seperating axis test, shape test etc
// --------------------------------------------------------//
void PhyObj_CircleCircle(PhyObjBoundingCircle* c1, PhyObjBoundingCircle* c2)
{
	// if colliding
	CP_Vector vector_c1_to_c2 = CP_Vector_Subtract(c2->_position, c1->_position);
	float length = CP_Vector_Length(vector_c1_to_c2);
	if (length < c1->_radius + c2->_radius) {
		PhyObjManifold* manifoldPtr, manifold;
		manifoldPtr = &manifold;
		manifold.A = c1;
		manifold.B = c2;
		manifold._contact_normal = CP_Vector_Normalize(vector_c1_to_c2);
		manifold._contact_position = CP_Vector_Add(c1->_position, CP_Vector_Scale(manifold._contact_normal, 0.5f * (c1->_radius - c2->_radius + length)));
		//CP_Graphics_DrawCircle(manifold._contact_position.x, manifold._contact_position.y, 2);
		// sum of radius + distance between centers
		manifold._penetration = (c1->_radius + c2->_radius) - length;
		PhyObj_AddManifold(manifold);
	}
}

int PhyObj_CircleOBox(PhyObjBoundingCircle* c, PhyObjOBoundingBox* b, PhyObjManifold* m)
{
	CP_Vector distance_pointonbox_to_circlecenter = PhyObj_NearestPointOnOBBToPoint(c->_position, b);
	// dot a vector by itself is the same as magnitude square
	return (CP_Vector_DotProduct(distance_pointonbox_to_circlecenter, distance_pointonbox_to_circlecenter) < c->_radius * c->_radius);
}

void PhyObj_CheckForCollisions()
{
	// reset manifolds
	PhyObj_manifolds_size = 0;
	for (int i = 0; i < PhyObj_bounding_circles_size-1; i++) {
		for (int j = i + 1; j < PhyObj_bounding_circles_size; j++) {
			PhyObj_CircleCircle(&PhyObj_bounding_circles[i], &PhyObj_bounding_circles[j]);
		}
	}
}

void PhyObj_ResolveManifolds()
{
	for (int i = 0; i < PhyObj_manifolds_size; i++) {
		PhyObjManifold m = PhyObj_manifolds[i];
		CP_Vector tangent = PhyObj_2DPerpendicular(m._contact_normal);
		/* ____________________________________________________________________________________________________________ */
		// vector from center of mass to contact point
		CP_Vector rA = CP_Vector_Subtract(m._contact_position, m.A->_position);
		CP_Vector rB = CP_Vector_Subtract(m._contact_position, m.B->_position);
		/* ____________________________________________________________________________________________________________ */
		// relative velocity, linear velocity + angular velocity scaled to vector perpendicular to normal
		CP_Vector vA = CP_Vector_Add(m.A->_velocity, CP_Vector_Scale(PhyObj_2DPerpendicular(rA), m.A->_angular_velocity));
		CP_Vector vB = CP_Vector_Add(m.B->_velocity, CP_Vector_Scale(PhyObj_2DPerpendicular(rB), m.B->_angular_velocity));
		CP_Vector relative_velocity = CP_Vector_Subtract(vB, vA);
		/* ____________________________________________________________________________________________________________ */
		// relative velocity along normal
		float velocity_along_normal = CP_Vector_DotProduct(relative_velocity, m._contact_normal);
		// relative velocity along tangent, for friction
		float velocity_along_tangent = CP_Vector_DotProduct(relative_velocity, tangent);
		/* ____________________________________________________________________________________________________________ */
		// calculate effective mass - in 2d cross product is not well defined and returns a scalar x1y2-x2y1
		// effective mass, 1/(inverse_mass + inverse_moment_of_inertia * (r x n)^2)
		// from eric cattos GDC https://www.youtube.com/watch?v=SHinxAhv1ZE timestamp-14:05
		// effective mass along the normal
		float effective_mass_normal = 1.0f / (m.B->_inv_mass + m.A->_inv_mass + CP_Math_Square(PhyObj_2DCross(m._contact_normal,rA)) * m.A->_inv_moment_of_inertia +
			CP_Math_Square(PhyObj_2DCross(m._contact_normal,rB)) * m.B->_inv_moment_of_inertia);
		// effective mass along the tangent, for friction
		float effective_mass_tangent = 1.0f / (m.B->_inv_mass + m.A->_inv_mass + CP_Math_Square(PhyObj_2DCross(tangent, rA)) * m.A->_inv_moment_of_inertia +
			CP_Math_Square(PhyObj_2DCross(tangent, rB)) * m.B->_inv_moment_of_inertia);
		/* ____________________________________________________________________________________________________________ */
		// calculate baumgarte - basically brute forcing the shape out over a timestep based on penetration depth
		float bias_factor = 0.2f;
		float allowed_penetration = 0.02f;
		float penetration = m._penetration - allowed_penetration;
		penetration = penetration < 0.0f ? 0.0f : penetration;
		float baumgarte = penetration * bias_factor / CP_System_GetDt();
		/* ____________________________________________________________________________________________________________ */
		// if not seperating, do something
		float friction_coefficient = 0.8f; // how slippery whoo~
		if (velocity_along_normal < 0) {
			// magnitude of impulse
			float restitution = 0.1f;
			float normal_lambda = (velocity_along_normal - baumgarte) * (1.0f + restitution) * -effective_mass_normal;
			float tangent_lambda = velocity_along_tangent * -effective_mass_tangent * friction_coefficient;
			CP_Vector impulse_vector = CP_Vector_Add(CP_Vector_Scale(m._contact_normal, normal_lambda),CP_Vector_Scale(tangent, tangent_lambda));
			PhyObj_ApplyImpulse(m.A, CP_Vector_Scale(impulse_vector,-1.0f));
			PhyObj_ApplyImpulse(m.B, impulse_vector);
			m.A->_angular_velocity -= PhyObj_2DCross(impulse_vector, rA) * m.A->_inv_moment_of_inertia;
			m.B->_angular_velocity += PhyObj_2DCross(impulse_vector, rB) * m.B->_inv_moment_of_inertia;
		}
	}
}

void PhyObj_IterativeSolveManifolds(const int count)
{
	for (int i = 0; i < count; i++) {
		PhyObj_ResolveManifolds();
	}
}