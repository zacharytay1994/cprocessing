#include "PhyObj.h"
#include "Camera.h"
#include <stdlib.h>
#include <stdio.h>

// --------------------------------------------------------//
// GLOBAL VARIABLE DEFINITIONS
// --------------------------------------------------------//
PhyObjBoundingShape** PhyObj_bounding_shapes;
int PhyObj_bounding_shapes_size;
int PhyObj_bounding_shapes_max_size;

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

float gravity = 98.1f;

// --------------------------------------------------------//
// MISCELLANEOUS
// - functions that are not about physics or collision
// --------------------------------------------------------//
void PhyObj_Initialize()
{
	PhyObj_bounding_shapes = (PhyObjBoundingShape**)malloc(INITIAL_SIZE * sizeof(PhyObjBoundingShape*));
	PhyObj_bounding_shapes_max_size = INITIAL_SIZE;
	PhyObj_bounding_shapes_size = 0;

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

PhyObjBoundingCircle* PhyObj_AddCircle(const float x, const float y, const float m, const float r, const float f)
{
	if (++PhyObj_bounding_circles_size > PhyObj_bounding_circles_max_size) {
		PhyObjBoundingCircle* check = (PhyObjBoundingCircle*)realloc(PhyObj_bounding_circles, sizeof(PhyObjBoundingCircle) * PhyObj_bounding_circles_max_size * 2);
		PhyObj_bounding_circles_max_size *= 2;
		if (check != NULL) {
			PhyObj_bounding_circles = check;
			// update shapes array with new address
			for (int i = 0; i < PhyObj_bounding_circles_size-1; i++) {
				PhyObj_bounding_shapes[PhyObj_bounding_circles[i].super._id] = (PhyObjBoundingShape*)&PhyObj_bounding_circles[i];
			}
		}
		else {
			printf("Collision.c::PhyObj_AddCircle - realloc returned null");
		}
	}
	// moment of inertia of a circle = m * r^2
	float moment_of_inertia;
	float inv_moment_of_inertia;
	if (m > 0.0f) {
		moment_of_inertia = m * r * r;
		inv_moment_of_inertia = 1.0f / moment_of_inertia;
	}
	else {
		moment_of_inertia = INFINITE_MASS;
		inv_moment_of_inertia = 0.0f;
	}
	//float moment_of_inertia = (m>0.0f?m:INFINITE_MASS) * r * r;
	PhyObjBoundingCircle temp = { {PhyObj_bounding_shapes_size,BOUNDING_CIRCLE,{x,y},0.0f,{0.0f,0.0f},0.0f,m,m>0.0f?1.0f/m:0,moment_of_inertia,inv_moment_of_inertia,CP_Vector_Set(0.0f,0.0f),1,f,0},r };
	PhyObj_bounding_circles[PhyObj_bounding_circles_size - 1] = temp;
	PhyObj_AddShape((PhyObjBoundingShape*)&PhyObj_bounding_circles[PhyObj_bounding_circles_size - 1]);
	return &PhyObj_bounding_circles[PhyObj_bounding_circles_size - 1];
}

PhyObjOBoundingBox* PhyObj_AddOBox(const float x, const float y, const float m, const float he, const float ve, const float f)
{
	if (++PhyObj_bounding_obox_size > PhyObj_bounding_obox_max_size) {
		PhyObjOBoundingBox* check = (PhyObjOBoundingBox*)realloc(PhyObj_bounding_obox, sizeof(PhyObjOBoundingBox) * PhyObj_bounding_obox_max_size * 2);
		PhyObj_bounding_obox_max_size *= 2;
		if (check != NULL) {
			PhyObj_bounding_obox = check;
			// update shapes array with new address
			for (int i = 0; i < PhyObj_bounding_obox_size - 1; i++) {
				PhyObj_bounding_shapes[PhyObj_bounding_obox[i].super._id] = (PhyObjBoundingShape*)&PhyObj_bounding_obox[i];
			}
		}
		else {
			printf("Collision.c::PhyObj_AddOBox - realloc returned null");
		}
	}
	// moment of inertia of a rectangle = (w * h^3)/12
	float moment_of_inertia = (he * ve * ve * ve) / 12.0f;
	PhyObjOBoundingBox temp = { {PhyObj_bounding_shapes_size,BOUNDING_OBOX,{x,y},0.0f,{0.0f,0.0f},0.0f,m,m>0.0f?1.0f/m:0,moment_of_inertia,m>0.0f?1.0f/moment_of_inertia:0.0f,CP_Vector_Set(0.0f,0.0f),1,f,0},he,ve };
	PhyObj_bounding_obox[PhyObj_bounding_obox_size - 1] = temp;
	PhyObj_AddShape((PhyObjBoundingShape*)&PhyObj_bounding_obox[PhyObj_bounding_obox_size - 1]);
	return &PhyObj_bounding_obox[PhyObj_bounding_obox_size - 1];
}

PhyObjBoundingCircle* PhyObj_AddAACircle(const float x, const float y, const float m, const float r, const float f)
{
	PhyObjBoundingCircle* circle = PhyObj_AddCircle(x, y, m, r, f);
	circle->super._moment_of_inertia = INFINITE_MASS;
	circle->super._inv_moment_of_inertia = 0.0f;
	return circle;
}

PhyObjOBoundingBox* PhyObj_AddAABox(const float x, const float y, const float m, const float he, const float ve, const float f)
{
	PhyObjOBoundingBox* box = PhyObj_AddOBox(x, y, m, he, ve, f);
	box->super._moment_of_inertia = INFINITE_MASS;
	box->super._inv_moment_of_inertia = 0.0f;
	return box;
}

void PhyObj_Update(const float dt)
{
	PhyObj_GlobalAcceleration();
	PhyObj_UpdatePosition(dt);
	PhyObj_UpdateRotation(dt);
	PhyObj_CheckForCollisions();
	PhyObj_WarmStarting();
	PhyObj_IterativeSolveManifolds(RESOLUTION_ITERATIONS);
}

void PhyObj_Render()
{
	PhyObj_DrawCircles();
	PhyObj_DrawOBoxes();
}

void PhyObj_Free()
{
	// free arrays
	free(PhyObj_bounding_circles);
	free(PhyObj_bounding_obox);
	free(PhyObj_manifolds);
	free(PhyObj_bounding_shapes);
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

void PhyObj_DrawCircles()
{
	for (int i = 0; i < PhyObj_bounding_circles_size; i++) {
		if (PhyObj_bounding_circles[i].super._visible) {
			CP_Vector cam_position = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), PhyObj_bounding_circles[i].super._position);
			CP_Image_DrawAdvanced(PhyObj_circle_image, cam_position.x, cam_position.y, PhyObj_bounding_circles[i]._radius * 2.0f, PhyObj_bounding_circles[i]._radius * 2.0f, 255, PhyObj_bounding_circles[i].super._rotation);
		}
	}
}

void PhyObj_DrawOBoxes()
{
	for (int i = 0; i < PhyObj_bounding_obox_size; i++) {
		if (PhyObj_bounding_obox[i].super._visible) {
			CP_Vector cam_position = CP_Vector_MatrixMultiply(Camera_GetCameraTransform(), PhyObj_bounding_obox[i].super._position);
			CP_Image_DrawAdvanced(PhyObj_square_image, cam_position.x, cam_position.y, PhyObj_bounding_obox[i]._horizontal_extent * 2.0f, PhyObj_bounding_obox[i]._vertical_extent * 2.0f, 255, PhyObj_bounding_obox[i].super._rotation);
		}
	}
}

void PhyObj_AddShape(PhyObjBoundingShape* shape)
{
	if (++PhyObj_bounding_shapes_size > PhyObj_bounding_shapes_max_size) {
		PhyObjBoundingShape** check = (PhyObjBoundingShape**)realloc(PhyObj_bounding_shapes, sizeof(PhyObjBoundingShape*) * PhyObj_bounding_shapes_max_size * 2);
		PhyObj_bounding_shapes_max_size *= 2;
		if (check != NULL) {
			PhyObj_bounding_shapes = check;
		}
		else {
			printf("Collision.c::PhyObj_AddShape - realloc returned null");
		}
	}
	PhyObj_bounding_shapes[PhyObj_bounding_shapes_size - 1] = shape;
}

void PhyObj_SetPosition(PhyObjBoundingShape* s, CP_Vector p)
{
	s->_position = p;
}

// --------------------------------------------------------//
//	MATH HELPER FUNCTIONS
// --------------------------------------------------------//

float PhyObj_2DCross(const CP_Vector v1, const CP_Vector v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

CP_Vector PhyObj_2DPerpendicular(const CP_Vector v)
{
	return (CP_Vector){v.y,-v.x};
}

PhyObjBoundingShape* PhyObj_GetShape(const int id)
{
	if (id < PhyObj_bounding_shapes_size) {
		return PhyObj_bounding_shapes[id];
	}
	return NULL;
}

void PhyObj_SetVisible(const int id, const int visible)
{
	if (id < PhyObj_bounding_shapes_size) {
		PhyObj_bounding_shapes[id]->_visible = visible;
	}
}

void PhyObj_SetAllVisible(const int visible)
{
	for (int i = 0; i < PhyObj_bounding_shapes_size; i++) {
		PhyObj_bounding_shapes[i]->_visible = visible;
	}
}

float PhyObj_VectorSquareMagnitude(const CP_Vector v)
{
	return CP_Math_Square(v.x) + CP_Math_Square(v.y);
}

float PhyObj_Mod(const float in)
{
	return in < 0.0f ? -in : in;
}

// --------------------------------------------------------//
//	PHYSICS
//	- physics functions, e.g. gravity, force, acceleration 
// --------------------------------------------------------//
void PhyObj_AddVelocity(PhyObjBoundingShape* s, const CP_Vector v)
{
	s->_velocity = CP_Vector_Add(s->_velocity, v);
}

void PhyObj_AddAngularVelocity(PhyObjBoundingShape* s, const float v)
{
	s->_angular_velocity += v;
}

void PhyObj_ApplyImpulse(PhyObjBoundingShape* s, const CP_Vector v)
{
	PhyObj_AddVelocity(s, CP_Vector_Scale(v, s->_inv_mass));
	if (CP_Input_KeyDown(KEY_R)) {
		printf("%f, %f\n", CP_Vector_Scale(v, s->_inv_mass).x, CP_Vector_Scale(v, s->_inv_mass).y);
	}
}

void PhyObj_UpdatePosition(const float dt)
{
	for (int i = 0; i < PhyObj_bounding_shapes_size; i++) {
		PhyObj_bounding_shapes[i]->_position = 
			CP_Vector_Add(PhyObj_bounding_shapes[i]->_position, CP_Vector_Scale(PhyObj_bounding_shapes[i]->_velocity, dt));
	}
}

void PhyObj_UpdateRotation(const float dt)
{
	for (int i = 0; i < PhyObj_bounding_shapes_size; i++) {
		// angular velocity is radians per second, but rotation takes degrees
		float angular_velocity_degrees = PhyObj_bounding_shapes[i]->_angular_velocity * 57.2957795130823f;
		PhyObj_bounding_shapes[i]->_rotation -= angular_velocity_degrees * dt;
	}
}

//	Applies a global acceleration to all bounding shapes in the system, meant for Gravity
void PhyObj_GlobalAcceleration()
{
	//CP_Vector direction = { (float)(CP_System_GetWindowWidth()/2),(float)(CP_System_GetWindowHeight()/2) };
	CP_Vector direction = CP_Vector_Set(0.0f, 1.0f);
	for (int i = 0; i < PhyObj_bounding_shapes_size; i++) {
		if (PhyObj_bounding_shapes[i]->_mass <= 0.01f) {
			continue;
		}
		// calculate vector to center
		/*CP_Vector vector = CP_Vector_Normalize(CP_Vector_Subtract(direction, PhyObj_bounding_shapes[i]->_position));
		vector = CP_Vector_Scale(vector, magnitude);
		vector = CP_Vector_Scale(vector, CP_System_GetDt());*/
		PhyObj_AddVelocity(PhyObj_bounding_shapes[i], CP_Vector_Scale(direction,GRAVITY*CP_System_GetDt()));
		//PhyObj_AddVelocity(PhyObj_bounding_shapes[i], vector);
	}
}

void PhyObj_WarmStarting()
{
	for (int i = 0; i < PhyObj_bounding_shapes_size; i++) {
		if (PhyObj_bounding_shapes[i]->_mass <= 0.01f) {
			continue;
		}
		// apply accumulated impulse to non sleeping colliders
		if (!PhyObj_bounding_shapes[i]->_sleeping) {
			PhyObj_ApplyImpulse(PhyObj_bounding_shapes[i], PhyObj_bounding_shapes[i]->_accumulated_impulse);
		}
		else {
			PhyObj_bounding_shapes[i]->_accumulated_impulse = CP_Vector_Set(0.0f, 0.0f);
		}
	}
}

// --------------------------------------------------------//
//	PRIMITIVE TESTS
//	e.g. point on line, point from line, etc.
// --------------------------------------------------------//
//	More info : Christer Ericson's Realtime Collision Detection (Page. 132)
CP_Vector PhyObj_NearestPointOnOBBToPoint(const CP_Vector p, const PhyObjOBoundingBox* b, int* point_in_box)
{
	CP_Vector result = b->super._position;
	// rotate basis vectors
	CP_Matrix mat = CP_Matrix_Rotate(b->super._rotation);
	CP_Vector u1 = CP_Vector_MatrixMultiply(mat, (CP_Vector) { 1,0 });
	CP_Vector u2 = CP_Vector_MatrixMultiply(mat, (CP_Vector) { 0,1 });
	// project point-boxposition onto axes and add to result
	CP_Vector box_to_point = CP_Vector_Subtract(p, b->super._position);
	float x = CP_Vector_DotProduct(box_to_point, u1);
	float y = CP_Vector_DotProduct(box_to_point, u2);
	// check if point is in box, if so snap to nearest edge
	if (x * x < b->_horizontal_extent * b->_horizontal_extent && y * y < b->_vertical_extent * b->_vertical_extent) {
		*point_in_box = 1;
		if (x * x > y * y) {
			x = x < 0 ? -b->_horizontal_extent : b->_horizontal_extent;
		}
		else {
			y = y < 0 ? -b->_vertical_extent : b->_vertical_extent;
		}
	}
	else {
		*point_in_box = 0;
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
//	COLLISION DETECTION
//	- collision detection functions
//	e.g. seperating axis test, shape test etc
// --------------------------------------------------------//
//	Test to see if a Circle is intersecting a Circle
//	More Info : Christer Ericson's Realtime Collision Detection (Page. 88)
int PhyObj_CircleCircle(PhyObjBoundingCircle* c1, PhyObjBoundingCircle* c2, PhyObjManifold* m)
{
	// if colliding
	CP_Vector vector_c1_to_c2 = CP_Vector_Subtract(c2->super._position, c1->super._position);
	float length = CP_Vector_Length(vector_c1_to_c2);
	if (length < c1->_radius + c2->_radius) {
		//PhyObjManifold* manifoldPtr, manifold;
		//manifoldPtr = &manifold;
		//m.type = MANIFOLD_CIRCLE_CIRCLE;
		m->A = (PhyObjBoundingShape*)c1;
		m->B = (PhyObjBoundingShape*)c2;
		m->_contact_normal = CP_Vector_Normalize(vector_c1_to_c2);
		m->_contact_position = CP_Vector_Add(c1->super._position, CP_Vector_Scale(m->_contact_normal, 0.5f * (c1->_radius - c2->_radius + length)));
		// sum of radius + distance between centers
		m->_penetration = (c1->_radius + c2->_radius) - length;
		m->_contact_position_2 = (CP_Vector){ -100.0f,-100.0f };
		m->_penetration_2 = -1.0f;
		return 1;
		//PhyObj_AddManifoldCvC(manifold);
		//PhyObj_AddManifold(manifold);
	}
	return 0;
}

//	Test to see if a Circle is intesecting an oriented box
//	More Info : Christer Ericson's Realtime Collision Detection (Page. 166)
int PhyObj_CircleOBox(PhyObjBoundingCircle* c, PhyObjOBoundingBox* b, PhyObjManifold* m)
{
	int point_in_box = -1;
	CP_Vector pointonbox_to_circlecenter = PhyObj_NearestPointOnOBBToPoint(c->super._position, b, &point_in_box);
	// check if circle center is within box
	float penetration;
	if (point_in_box) {
		penetration = c->_radius + CP_Vector_Length(CP_Vector_Subtract(pointonbox_to_circlecenter, c->super._position));
		m->_contact_normal = CP_Vector_Normalize(CP_Vector_Subtract(c->super._position, pointonbox_to_circlecenter));
	}
	else {
		penetration = c->_radius - CP_Vector_Length(CP_Vector_Subtract(pointonbox_to_circlecenter, c->super._position));
		m->_contact_normal = CP_Vector_Normalize(CP_Vector_Subtract(pointonbox_to_circlecenter, c->super._position));
	}
	// if colliding
	if (penetration > 0.0f) {
		m->A = (PhyObjBoundingShape*)c;
		m->B = (PhyObjBoundingShape*)b;
		m->_contact_position = pointonbox_to_circlecenter;
		m->_penetration = penetration;
		m->_contact_position_2 =	(CP_Vector){ -100.0f,-100.0f };
		m->_penetration_2 = -1.0f;
		return 1;
	}
	return 0;
}

//	Note : Probably need a more efficient SAT - TBC
/*  _________________________________________________________________________________
	 | Pre  : Box 1 and Box 2 initialized into world with relevant data
	 | Post : Manifold Data filled if collision detected
	 ________________________
	 CURRENT IMPLEMENTATION : 
	 Step 1 - Perform Seperating Axis Theorem (SAT), i.e. detection
	 Step 2 - Find corners of the reference edge, i.e the face of least penetration
	 Step 3 - Find corners of incident box that penetrate the reference edge
	 Step 4 - Snap (step 3) corners onto the reference edge
	_________________________________________________________________________________ */
int PhyObj_BoxBox(PhyObjOBoundingBox* b1, PhyObjOBoundingBox* b2, PhyObjManifold* m)
{
	// perform SAT
	CP_Matrix rotation_b1 = CP_Matrix_Rotate(b1->super._rotation);
	CP_Matrix rotation_b2 = CP_Matrix_Rotate(b2->super._rotation);

	CP_Vector position_b1 = b1->super._position;
	CP_Vector position_b2 = b2->super._position;

	CP_Vector axis1_b1 = CP_Vector_MatrixMultiply(rotation_b1, (CP_Vector) { 1.0f, 0.0f });
	CP_Vector axis2_b1 = CP_Vector_MatrixMultiply(rotation_b1, (CP_Vector) { 0.0f, 1.0f });
	CP_Vector axis1_b2 = CP_Vector_MatrixMultiply(rotation_b2, (CP_Vector) { 1.0f, 0.0f });
	CP_Vector axis2_b2 = CP_Vector_MatrixMultiply(rotation_b2, (CP_Vector) { 0.0f, 1.0f });

	CP_Vector h_extent_b1 = CP_Vector_Scale(axis1_b1, b1->_horizontal_extent);
	CP_Vector v_extent_b1 = CP_Vector_Scale(axis2_b1, b1->_vertical_extent);
	CP_Vector h_extent_b2 = CP_Vector_Scale(axis1_b2, b2->_horizontal_extent);
	CP_Vector v_extent_b2 = CP_Vector_Scale(axis2_b2, b2->_vertical_extent);

	CP_Vector translation = CP_Vector_Subtract(position_b2, position_b1);

	// __________________________________ START - STEP 1 ___________________________________________________________________
	// check the 4 axes
	// proj_axis(translation)^2 >
	// (h_extent_b1 dot axis)^2 + (v_extent_b1 dot axis)^2 + (h_extent_b2 dot axis)^2 + (v_extent_b2 dot axis)^2
	float translation_projected = -1;
	float penetration_check = 0;
	float penetration = 1000000.0f;
	CP_Vector axis_least_penetration = {0,0};
	// checks to see if a flip is needed x1x2_b1 : <0 flip >0 !flip, x1x2_b2 : <0 !flip, >0 flip
	int flip = -1; 
	int isb1 = -1;
	int is_horizontal = -1;
	// case 1 - axis1_b1, if seperation found return 0
	translation_projected = CP_Vector_DotProduct(translation, axis1_b1);
	penetration_check = (PhyObj_Mod(CP_Vector_DotProduct(h_extent_b1, axis1_b1)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b1, axis1_b1)) +
		PhyObj_Mod(CP_Vector_DotProduct(h_extent_b2, axis1_b1)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b2, axis1_b1))) - PhyObj_Mod(translation_projected);
	if (penetration_check < 0.0f) {
		return 0;
	}
	if (penetration_check < penetration) {
		penetration = penetration_check;
		axis_least_penetration = axis1_b1;
		isb1 = 1;
		flip = translation_projected < 0 ? -1 : 1;
		is_horizontal = 1;
	}
	// case 2 - axis2_b1
	translation_projected = CP_Vector_DotProduct(translation, axis2_b1);
	penetration_check = (PhyObj_Mod(CP_Vector_DotProduct(h_extent_b1, axis2_b1)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b1, axis2_b1)) +
		PhyObj_Mod(CP_Vector_DotProduct(h_extent_b2, axis2_b1)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b2, axis2_b1))) - PhyObj_Mod(translation_projected);
	if (penetration_check < 0.0f) {
		return 0;
	}
	if (penetration_check < penetration) {
		penetration = penetration_check;
		axis_least_penetration = axis2_b1;
		isb1 = 1;
		flip = translation_projected < 0 ? -1 : 1;
		is_horizontal = 0;
	}
	// case 3 - axis1_b2
	translation_projected = CP_Vector_DotProduct(translation, axis1_b2);
	penetration_check = (PhyObj_Mod(CP_Vector_DotProduct(h_extent_b1, axis1_b2)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b1, axis1_b2)) +
		PhyObj_Mod(CP_Vector_DotProduct(h_extent_b2, axis1_b2)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b2, axis1_b2))) - PhyObj_Mod(translation_projected);
	if (penetration_check < 0.0f) {
		return 0;
	}
	if (penetration_check < penetration) {
		penetration = penetration_check;
		axis_least_penetration = axis1_b2;
		isb1 = 0;
		flip = translation_projected > 0 ? -1 : 1;
		is_horizontal = 1;
	}
	// case 4 - axis2_b2
	translation_projected = CP_Vector_DotProduct(translation, axis2_b2);
	penetration_check = (PhyObj_Mod(CP_Vector_DotProduct(h_extent_b1, axis2_b2)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b1, axis2_b2)) +
		PhyObj_Mod(CP_Vector_DotProduct(h_extent_b2, axis2_b2)) + PhyObj_Mod(CP_Vector_DotProduct(v_extent_b2, axis2_b2))) - PhyObj_Mod(translation_projected);
	if (penetration_check < 0.0f) {
		return 0;
	}
	if (penetration_check < penetration) {
		penetration = penetration_check;
		axis_least_penetration = axis2_b2;
		isb1 = 0;
		flip = translation_projected > 0 ? -1 : 1;
		is_horizontal = 0;
	}
	// __________________________________ END - STEP 1 ___________________________________________________________________
	// __________________________________ START - STEP 2 _________________________________________________________________
	// calculate corner of reference plane
	CP_Vector ref_corner_1;
	CP_Vector ref_corner_2;
	//CP_Vector axis_horizontal = axis_least_penetration
	if (isb1) {
		if (is_horizontal) {
			ref_corner_1 = CP_Vector_Add(b1->super._position, 
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b1->_horizontal_extent * flip), v_extent_b1));
			ref_corner_2 = CP_Vector_Add(b1->super._position,
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b1->_horizontal_extent * flip), CP_Vector_Scale(v_extent_b1,-1)));
		}
		else {
			ref_corner_1 = CP_Vector_Add(b1->super._position,
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b1->_vertical_extent * flip), h_extent_b1));
			ref_corner_2 = CP_Vector_Add(b1->super._position,
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b1->_vertical_extent * flip), CP_Vector_Scale(h_extent_b1, -1)));
		}
	}
	else {
		if (is_horizontal) {
			ref_corner_1 = CP_Vector_Add(b2->super._position,
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b2->_horizontal_extent * flip), v_extent_b2));
			ref_corner_2 = CP_Vector_Add(b2->super._position,
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b2->_horizontal_extent * flip), CP_Vector_Scale(v_extent_b2, -1)));
		}
		else {
			ref_corner_1 = CP_Vector_Add(b2->super._position,
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b2->_vertical_extent * flip), h_extent_b2));
			ref_corner_2 = CP_Vector_Add(b2->super._position,
				CP_Vector_Add(CP_Vector_Scale(axis_least_penetration, b2->_vertical_extent * flip), CP_Vector_Scale(h_extent_b2, -1)));
		}
	}
	// __________________________________ END - STEP 2 _________________________________________________________________
	// __________________________________ Start - STEP 3 _______________________________________________________________
	// calculate point 2 potential incident edges
	// corner of intersection is the corner of the incident box closest to the reference box center
	int number_of_contacts = 0;
	CP_Vector corner_of_intersection1 = { 0,0 };
	CP_Vector corner_of_intersection2 = { 0,0 };
	float contact_penetration1 = -1;
	float contact_penetration2 = -1;
	CP_Vector coi_check = { 0,0 };
	float d_corner_to_center = 1000000.0f;
	float d_ctc_check = -1;
	if (isb1) { // if b1 has the reference edge
		float check_extent = is_horizontal == 1 ? b1->_horizontal_extent : b1->_vertical_extent;
		// top-right
		coi_check = CP_Vector_Add(b2->super._position,CP_Vector_Add(h_extent_b2, v_extent_b2));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check,b1->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b1->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		// if corner is intersecting box
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
		// top-left
		coi_check = CP_Vector_Add(b2->super._position, CP_Vector_Add(CP_Vector_Scale(h_extent_b2, -1), v_extent_b2));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check, b1->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b1->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
		// bottom-right
		coi_check = CP_Vector_Add(b2->super._position,CP_Vector_Add(h_extent_b2, CP_Vector_Scale(v_extent_b2, -1)));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check, b1->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b1->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
		// bottom-left
		coi_check = CP_Vector_Add(b2->super._position, CP_Vector_Add(CP_Vector_Scale(h_extent_b2, -1), CP_Vector_Scale(v_extent_b2, -1)));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check, b1->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b1->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
	}
	else { // b2 has the reference edge
		float check_extent = is_horizontal == 1 ? b2->_horizontal_extent : b2->_vertical_extent;
		// top-right
		coi_check = CP_Vector_Add(b1->super._position, CP_Vector_Add(h_extent_b1, v_extent_b1));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check, b2->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b2->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
		// top-left
		coi_check = CP_Vector_Add(b1->super._position, CP_Vector_Add(CP_Vector_Scale(h_extent_b1, -1), v_extent_b1));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check, b2->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b2->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
		// bottom-right
		coi_check = CP_Vector_Add(b1->super._position, CP_Vector_Add(h_extent_b1, CP_Vector_Scale(v_extent_b1, -1)));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check, b2->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b2->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
		// bottom-left
		coi_check = CP_Vector_Add(b1->super._position, CP_Vector_Add(CP_Vector_Scale(h_extent_b1, -1), CP_Vector_Scale(v_extent_b1, -1)));
		//d_ctc_check = CP_Vector_Length(CP_Vector_Subtract(coi_check, b2->super._position));
		d_ctc_check = CP_Vector_DotProduct(CP_Vector_Subtract(coi_check, b2->super._position), CP_Vector_Scale(axis_least_penetration, (float)flip));
		if (d_ctc_check < check_extent) {
			d_corner_to_center = d_ctc_check;
			if (number_of_contacts) {
				corner_of_intersection2 = coi_check;
				number_of_contacts = 2;
				contact_penetration2 = check_extent - d_ctc_check;
			}
			else {
				corner_of_intersection1 = coi_check;
				number_of_contacts = 1;
				contact_penetration1 = check_extent - d_ctc_check;
			}
		}
	}
	// __________________________________ End - STEP 3 _______________________________________________________________
	// __________________________________ Start - STEP 4 _____________________________________________________________
	float coi_length;
	if (number_of_contacts == 1) {
		// check if contact is on reference plane
		if (isb1) {
			corner_of_intersection1 = CP_Vector_Add(b1->super._position, CP_Vector_Add(CP_Vector_Subtract(corner_of_intersection1, b1->super._position), CP_Vector_Scale(axis_least_penetration, contact_penetration1 * flip)));
		}
		else {
			corner_of_intersection1 = CP_Vector_Add(b2->super._position, CP_Vector_Add(CP_Vector_Subtract(corner_of_intersection1, b2->super._position), CP_Vector_Scale(axis_least_penetration, contact_penetration1 * flip)));
		}
	}
	else if (number_of_contacts == 2) {
		if (isb1) {
			float ref_corner_length = CP_Math_Square(b1->_horizontal_extent) + CP_Math_Square(b1->_vertical_extent);
			corner_of_intersection1 = CP_Vector_Add(b1->super._position, CP_Vector_Add(CP_Vector_Subtract(corner_of_intersection1, b1->super._position), CP_Vector_Scale(axis_least_penetration, contact_penetration1 * flip)));
			corner_of_intersection2 = CP_Vector_Add(b1->super._position, CP_Vector_Add(CP_Vector_Subtract(corner_of_intersection2, b1->super._position), CP_Vector_Scale(axis_least_penetration, contact_penetration2 * flip)));
			// clip to reference plane - i.e check if within ref corner lengths
			coi_length = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(corner_of_intersection1,position_b1));
			if (coi_length > ref_corner_length) {
				corner_of_intersection1 = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_1, corner_of_intersection1)) < PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_2, corner_of_intersection1)) ?
					ref_corner_1 : ref_corner_2;
			}
			coi_length = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(corner_of_intersection2, position_b1));
			if (coi_length > ref_corner_length) {
				corner_of_intersection2 = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_1, corner_of_intersection2)) < PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_2, corner_of_intersection2)) ?
					ref_corner_1 : ref_corner_2;
			}
		}
		else {
			float ref_corner_length = CP_Math_Square(b2->_horizontal_extent) + CP_Math_Square(b2->_vertical_extent);
			corner_of_intersection1 = CP_Vector_Add(b2->super._position, CP_Vector_Add(CP_Vector_Subtract(corner_of_intersection1, b2->super._position), CP_Vector_Scale(axis_least_penetration, contact_penetration1 * flip)));
			corner_of_intersection2 = CP_Vector_Add(b2->super._position, CP_Vector_Add(CP_Vector_Subtract(corner_of_intersection2, b2->super._position), CP_Vector_Scale(axis_least_penetration, contact_penetration2 * flip)));
			// clip to reference plane - i.e check if within ref corner lengths
			coi_length = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(corner_of_intersection1, position_b2));
			if (coi_length > ref_corner_length) {
				corner_of_intersection1 = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_1, corner_of_intersection1)) < PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_2, corner_of_intersection1)) ?
					ref_corner_1 : ref_corner_2;
			}
			coi_length = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(corner_of_intersection2, position_b2));
			if (coi_length > ref_corner_length) {
				corner_of_intersection2 = PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_1, corner_of_intersection2)) < PhyObj_VectorSquareMagnitude(CP_Vector_Subtract(ref_corner_2, corner_of_intersection2)) ?
					ref_corner_1 : ref_corner_2;
			}
		}
	}

	// output manifold
	if (isb1) {
		m->A = (PhyObjBoundingShape*)b1;
		m->B = (PhyObjBoundingShape*)b2;
	}
	else {
		m->A = (PhyObjBoundingShape*)b2;
		m->B = (PhyObjBoundingShape*)b1;
	}
	m->_contact_normal = CP_Vector_Scale(axis_least_penetration,(float)flip);
	m->_contact_position = corner_of_intersection1;
	m->_penetration = contact_penetration1;
	if (number_of_contacts == 2) {
		m->_contact_position_2 = corner_of_intersection2;
		m->_penetration_2 = contact_penetration2;
	}
	else {
		m->_contact_position_2 = (CP_Vector){ 0.0f,0.0f };
		m->_penetration_2 = -1.0f;
	}

	//printf("%f, %f\n", contact_penetration1, contact_penetration2);
	// -------------------- DEBUG CODE -----------------------------------------
	/*CP_Graphics_DrawCircle(corner_of_intersection1.x, corner_of_intersection1.y, 20.0f);
	CP_Graphics_DrawCircle(corner_of_intersection2.x, corner_of_intersection2.y, 20.0f);*/
	// draw axis of least penetration
	//float scale = flip == 1 ? -1.0f : 1.0f;
	/*CP_Vector temp = CP_Vector_Scale(axis_least_penetration, 30.0f * flip);
	CP_Vector temp_pos = isb1 == 1 ? b1->super._position : b2->super._position;
	temp = CP_Vector_Add(temp_pos, temp);
	CP_Graphics_DrawLine(temp_pos.x, temp_pos.y,temp.x,temp.y);
	CP_Graphics_DrawCircle(100, 100, 10);*/
	// -------------------- DEBUG CODE -----------------------------------------
	return 1;
}

/*  _________________________________________________________________________________
	 | PRE  : PhyObj_manifolds empty, i.e. no manifolds to resolve
	 | POST : PhyObj_manifolds filled, i.e. manifolds to resolve
	 ________________________
	 CURRENT IMPLEMENTATION :
	 Step 1 - Loops through PhyObj_bounding_shapes, iteratively comparing with each other
	 Step 2 - Check type of shape for both, and call the relevant functions
	 e.g. PhyObj_CircleCircle(), PhyObj_CircleOBox(), PhyObj_BoxBox()
	_________________________________________________________________________________ */
void PhyObj_CheckForCollisions()
{
	// reset manifolds
	PhyObj_manifolds_size = 0;
	// reset sleep
	for (int i = 0; i < PhyObj_bounding_shapes_size; i++) {
		PhyObj_bounding_shapes[i]->_sleeping = 1;
		// reset num contacts of shapes
		PhyObj_bounding_shapes[i]->_num_contacts = 0;
		PhyObj_bounding_shapes[i]->_grounded = 0;
	}
	for (int i = 0; i < PhyObj_bounding_shapes_size - 1; i++) {
		for (int j = i + 1; j < PhyObj_bounding_shapes_size; j++) {
			// if both shape's mass <= 0, i.e. static, skip checks continue
			if (PhyObj_bounding_shapes[i]->_mass <= 0.0f && PhyObj_bounding_shapes[j]->_mass <= 0.0f) {
				continue;
			}
			PhyObjManifold* manifoldPtr, manifold;
			manifoldPtr = &manifold;
			if (PhyObj_bounding_shapes[i]->_type == BOUNDING_CIRCLE && PhyObj_bounding_shapes[j]->_type == BOUNDING_CIRCLE) {
				if (PhyObj_CircleCircle((PhyObjBoundingCircle*)PhyObj_bounding_shapes[i], (PhyObjBoundingCircle*)PhyObj_bounding_shapes[j], manifoldPtr)) {
					PhyObj_AddManifold(manifold);
				}
			}
			else if (PhyObj_bounding_shapes[i]->_type == BOUNDING_CIRCLE && PhyObj_bounding_shapes[j]->_type == BOUNDING_OBOX) {
				if (PhyObj_CircleOBox((PhyObjBoundingCircle*)PhyObj_bounding_shapes[i], (PhyObjOBoundingBox*)PhyObj_bounding_shapes[j], manifoldPtr)) {
					PhyObj_AddManifold(manifold);
				}
			}
			else if (PhyObj_bounding_shapes[i]->_type == BOUNDING_OBOX && PhyObj_bounding_shapes[j]->_type == BOUNDING_CIRCLE) {
				if (PhyObj_CircleOBox((PhyObjBoundingCircle*)PhyObj_bounding_shapes[j], (PhyObjOBoundingBox*)PhyObj_bounding_shapes[i], manifoldPtr)) {
					PhyObj_AddManifold(manifold);
				}
			}
			else if (PhyObj_bounding_shapes[i]->_type == BOUNDING_OBOX && PhyObj_bounding_shapes[j]->_type == BOUNDING_OBOX) {
				if (PhyObj_BoxBox((PhyObjOBoundingBox*)PhyObj_bounding_shapes[i], (PhyObjOBoundingBox*)PhyObj_bounding_shapes[j], manifoldPtr)) {
					PhyObj_AddManifold(manifold);
				}
			}
		}
	}
}

/*  _________________________________________________________________________________
	 | PRE  : Unresolved manifolds
	 | POST : Resolved manifolds, impulse resolution applied to shapes
	 ________________________
	 CURRENT IMPLEMENTATION :
	 Step 1 - Loops through PhyObj_manifolds
	 Step 2 - Check if manifold contains 1 or 2 contact points and resolve
	 (2d convex polygons only generate max 2 contact points, e.g. circle circle overlap
	_________________________________________________________________________________ */
void PhyObj_ResolveManifolds()
{
	for (int i = 0; i < PhyObj_manifolds_size; i++) {
		PhyObjManifold m = PhyObj_manifolds[i];
		// meaning if the manifold has 2 contact points, resolve both
		if (m._penetration_2 != -1.0f) {
			for (int j = 0; j < 2; j++) {
				CP_Vector contact_position;
				float m_penetration;
				if (j == 0) {
					contact_position = m._contact_position;
					m_penetration = m._penetration;
				}
				else {
					contact_position = m._contact_position_2;
					m_penetration = m._penetration_2;
				}
				PhyObj_ResolveContact(contact_position, m_penetration, m);
			}
		}
		// if one contact point only resolve the one
		else {
			PhyObj_ResolveContact(m._contact_position, m._penetration, m);
		}
	}
}

/*  _________________________________________________________________________________
	 | PRE  : Unresolved contact
	 | POST : Resolved contact, impulse resolution applied to shapes
	 ________________________
	 CURRENT IMPLEMENTATION :
	 Step 1 - Calculate relative velocity along the normal and tangent
	 Step 2 - Calculate relative mass along the normal and tangent
	 Step 3 - Calculate Lambda, i.e. impulse scalar
	 Step 4 - Apply to shapes
	 More Info : Physics for Game Programmers - Understanding Constraints (Erin Catto)
	 https://www.youtube.com/watch?v=SHinxAhv1ZE&t=3018s
	_________________________________________________________________________________ */
void PhyObj_ResolveContact(const CP_Vector contact_position, const float contact_penetration, PhyObjManifold m)
{
	CP_Vector tangent = PhyObj_2DPerpendicular(m._contact_normal);
	/* ____________________________________________________________________________________________________________ */
	// vector from center of mass to contact point
	CP_Vector rA = CP_Vector_Subtract(contact_position, m.A->_position);
	CP_Vector rB = CP_Vector_Subtract(contact_position, m.B->_position);
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
	float effective_mass_normal = 1.0f / (m.B->_inv_mass + m.A->_inv_mass + CP_Math_Square(PhyObj_2DCross(m._contact_normal, rA)) * m.A->_inv_moment_of_inertia +
		CP_Math_Square(PhyObj_2DCross(m._contact_normal, rB)) * m.B->_inv_moment_of_inertia);
	// effective mass along the tangent, for friction
	float effective_mass_tangent = 1.0f / (m.B->_inv_mass + m.A->_inv_mass + CP_Math_Square(PhyObj_2DCross(tangent, rA)) * m.A->_inv_moment_of_inertia +
		CP_Math_Square(PhyObj_2DCross(tangent, rB)) * m.B->_inv_moment_of_inertia);
	/* ____________________________________________________________________________________________________________ */
	// calculate baumgarte - basically brute forcing the shape out over a timestep based on penetration depth
	float bias_factor = 0.2f;
	float allowed_penetration = 0.1f;
	float penetration = contact_penetration - allowed_penetration;
	penetration = penetration < 0.0f ? 0.0f : penetration;
	float baumgarte = penetration * bias_factor / CP_System_GetDt();
	/* ____________________________________________________________________________________________________________ */
	// if not seperating, do something
	float friction_coefficient = m.A->_friction < m.B->_friction ? m.A->_friction : m.B->_friction; // how slippery whoo~
	if (velocity_along_normal < baumgarte+1.0f) { // +1 here is a hack sry, i.e. stickiness
		// magnitude of impulse
		float restitution = 0.0f;
		/*printf("Velocity AN : %f\n", velocity_along_normal);
		printf("Baumgarte : %f\n", baumgarte);*/
		float normal_lambda = (velocity_along_normal - baumgarte) * (1.0f + restitution) * -effective_mass_normal;
		float tangent_lambda = velocity_along_tangent * -effective_mass_tangent * friction_coefficient;
		CP_Vector impulse_vector = CP_Vector_Add(CP_Vector_Scale(m._contact_normal, normal_lambda), CP_Vector_Scale(tangent, tangent_lambda));
		// accumulate impulse for warm starting
		m.A->_accumulated_impulse = CP_Vector_Add(m.A->_accumulated_impulse,CP_Vector_Scale(impulse_vector, -1.0f));
		m.B->_accumulated_impulse = CP_Vector_Add(m.B->_accumulated_impulse,impulse_vector);
		m.A->_sleeping = 0;
		m.B->_sleeping = 0;
		// apply current step impulse
		CP_Vector AImpulse = CP_Vector_Scale(impulse_vector, -1.0f);
		CP_Vector BImpulse = impulse_vector;
		//printf("%f, %f", m._contact_normal.x, m._contact_normal.y);
		PhyObj_ApplyImpulse((PhyObjBoundingShape*)m.A, AImpulse);
		PhyObj_ApplyImpulse((PhyObjBoundingShape*)m.B, BImpulse);
		m.A->_angular_velocity -= PhyObj_2DCross(impulse_vector, rA) * m.A->_inv_moment_of_inertia;
		m.B->_angular_velocity += PhyObj_2DCross(impulse_vector, rB) * m.B->_inv_moment_of_inertia;

		// if the contact normal is above a certain vertical threshold and is going upward, 
		// flag the box it is applied to as being grounded
		if (m._contact_normal.y > 0.9f) {
			m.A->_grounded = 1;
		}
		else if (m._contact_normal.y < -0.9f) {
			m.B->_grounded = 1;
		}

		if (m.A->_num_contacts < SHAPE_MAX_NUM_CONTACTS) {
			m.A->_contact_data[m.A->_num_contacts++] = contact_position;
		}
		if (m.B->_num_contacts < SHAPE_MAX_NUM_CONTACTS) {
			m.B->_contact_data[m.B->_num_contacts++] = contact_position;
		}
		// DEBUG CODE
		/*if (CP_Input_KeyDown(KEY_R)) {
			if (m.A->_mass > 0.0f && m.B->_mass > 0.0f) {
				printf("vel : %f, ", velocity_along_normal);
				printf("baum : %f, ", baumgarte);
				printf("normal : %f, %f, ", m._contact_normal.x, m._contact_normal.y);
				printf("effmass : %f\n", effective_mass_normal);
				printf("AImpulse : %f, %f\n", AImpulse.x, AImpulse.y);
				printf("BImpulse : %f, %f\n", BImpulse.x, BImpulse.y);
				printf("A : %f, %f\n", m.A->_velocity.x, m.A->_velocity.y);
				printf("B : %f, %f\n", m.B->_velocity.x, m.B->_velocity.y);
				printf("Apos : %f, %f\n", m.A->_position.x, m.A->_position.y);
				printf("Bpos : %f, %f\n", m.B->_position.x, m.B->_position.y);
				printf("Avel : %f, %f\n", m.A->_velocity.x, m.A->_velocity.y);
				printf("Bvel : %f, %f\n", m.B->_velocity.x, m.B->_velocity.y);
			}
		}*/
	}
}

//	Iteratively solve in order to converge onto a stable simulation
//	i.e. running the solver multiple times while applying the impulse every iteration
void PhyObj_IterativeSolveManifolds(const int count)
{
	for (int i = 0; i < count; i++) {
		PhyObj_ResolveManifolds();
	}
}