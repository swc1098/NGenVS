#ifndef FRAME_OF_REFERENCE
#define FRAME_OF_REFERENCE

#include "Matrix.h"

typedef struct FrameOfReference
{
	Matrix* scale;
	Matrix* rotation; // Where the 3 columns represent the Right, Up, and Back Vectors,
	Vector* position; // In worldspace	

} FrameOfReference;

///
//Allocates memory for a Frame of Reference
//
//Returns:
//	Pointer to newly allocated uninitialized Frame of Reference
FrameOfReference* FrameOfReference_Allocate(void);

///
//Initializes a Frame of REference
//
//Parameters:
//	FoRef: The frame of reference to initialize
void FrameOfReference_Initialize(FrameOfReference* FoRef);

///
//Frees resources allocated for a Frame of Reference
//
//Parameters:
//	FoRef: A pointer to the frame of reference to free
void FrameOfReference_Free(FrameOfReference* FoRef);

///
//Translates the origin of a Frame of Reference
//
//Parameters:
//	FoRef: The frame of reference being translated
//	translation: The 3 component translation Vector
void FrameOfReference_Translate(FrameOfReference* FoRef, const Vector* translation);

///
//Rotates a Frame of Reference
//
//Parameters:
//	FoRef: The frame of reference being rotated
//	axis: The axis being rotated around (Relative to the frame of reference)
//	radians: The amount of radians to rotate by
void FrameOfReference_Rotate(FrameOfReference* FoRef, const Vector* axis, const float radians);

///
//Scales a frame of reference
//
//Parameters:
//	FoRef: The frame of reference being scaled
//	scaleVector: A 3 component Vector containing X, Y, and Z scale factors
void FrameOfReference_Scale(FrameOfReference* FoRef, const Vector* scaleVector);

///
//Constructs a 3x3 rotation matrix to rotate the coordinate system of a frame of reference
//
//PArameters:
//	destination:	The destination of the matrix
//	axis: The axis to rotate around
//	radians: The amound of radians to rotate by
void FrameOfReference_ConstructRotationMatrix(Matrix* destination, const Vector* axis, const float radians);

///
//Compiles the Frame of Reference into a 4x4 matrix
//Where the first 3x3 is a rotation matrix with scale information on the diagnol
//And the 4th column is a Vector 3 holding xyz position info
//
//Parameters:
//	source: THe frame of reference to inscribe into a matrix
//	dest: A pointer to a 4x4 destination matrix
void FrameOfReference_ToMatrix4(FrameOfReference* source, Matrix* dest);

#endif