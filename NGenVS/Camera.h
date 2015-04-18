#include <GL/glew.h>
#include <GL/freeglut.h>

#include "FrameOfReference.h"

typedef struct Camera
{
	Matrix* translationMatrix;
	Matrix* rotationMatrix;
	Matrix* projectionMatrix;

	float aspectX, aspectY;

	float nearPlane, farPlane;
	float leftPlane, rightPlane;
	float topPlane, bottomPlane;
} Camera;

///
//Allocates memory for a camera
//
//Returns:
//	Pointer to newly allocated camera
Camera* Camera_Allocate();

///
//Initializes a camera
//
//Parameters:
//	cam: The cmera to initialize
void Camera_Initialize(Camera* cam);

///
//Frees resources being taken by a camera
//
//Parameters:
//	cam: The camera which we are deleting
void Camera_Free(Camera* cam);

///
//Recalculates the perspective matrix based on the camera's attributes
//
//Parameters:
//	cam: The camera which has a perspective matrix we want to refresh.
void Camera_RefreshPerspectiveMatrix(Camera* cam);

///
//Translates the camera
//
//Parameters:
//	cam: The camera to translate
//	translation: The Vector to translate by (NON CONST)
void Camera_Translate(Camera* cam, Vector* translation);

///
//Rotates the camera
//
//Parameters:
//	Cam: The camera to rotate
//	axis: The axis to rotate around
//	radians: The angle in radians to rotate by
void Camera_Rotate(Camera* cam, Vector* axis, const float radians);

///
//Scales the camera
//
//Parameters:
//	cam: thegamera to scale
//	scaleVector: a Vec-3 containing x, y, and z scale factors
void Camera_Scale(Camera* cam, const Vector* scaleVector);

///
//Compiles the camera into a 4x4 view matrix containing a translation to camera view
//Where the first 3x3 is a rotation matrix with scale information on the diagnol
//And the 4th column is a Vector 3 holding Rx(x)+Ry(y)+Rz(z)
//
//Parameters:
//	source: THe camera to inscribe into a matrix
//	dest: A pointer to a 4x4 destination matrix
void Camera_ToMatrix4(Camera* source, Matrix* dest);
