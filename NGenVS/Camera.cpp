#include "Camera.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

///
//Allocates memory for a camera
//
//Returns:
//	Pointer to newly allocated camera
Camera* Camera_Allocate()
{

	Camera* cam = (Camera*)malloc(sizeof(Camera));

	return cam;
}

///
//Initializes a camera
//
//Parameters:
//	cam: The cmera to initialize
void Camera_Initialize(Camera* cam)
{
	cam->aspectX = 1.0f;
	cam->aspectY = 1.0f;

	cam->nearPlane = 1.0f;
	cam->farPlane = 100.0f;

	cam->leftPlane = -1.0f;
	cam->rightPlane = 1.0f;

	cam->bottomPlane = -1.0f;
	cam->topPlane = 1.0f;



	cam->translationMatrix = Matrix_Allocate();
	Matrix_Initialize(cam->translationMatrix, 4, 4);

	cam->rotationMatrix = Matrix_Allocate();
	Matrix_Initialize(cam->rotationMatrix, 4, 4);

	cam->projectionMatrix = Matrix_Allocate();
	Matrix_Initialize(cam->projectionMatrix, 4, 4);

	*Matrix_Index(cam->projectionMatrix, 0, 0) = (2.0f * cam->aspectX * cam->nearPlane) / (cam->rightPlane - cam->leftPlane);
	*Matrix_Index(cam->projectionMatrix, 0, 2) = (cam->rightPlane + cam->leftPlane) / (cam->rightPlane - cam->leftPlane);

	*Matrix_Index(cam->projectionMatrix, 1, 1) = (2.0f * cam->aspectY * cam->nearPlane) / (cam->topPlane - cam->bottomPlane);
	*Matrix_Index(cam->projectionMatrix, 1, 2) = (cam->topPlane + cam->bottomPlane) / (cam->topPlane - cam->bottomPlane);

	*Matrix_Index(cam->projectionMatrix, 2, 2) = -(cam->farPlane * cam->nearPlane) / (cam->farPlane - cam->nearPlane);
	*Matrix_Index(cam->projectionMatrix, 2, 3) = (-2.0f * cam->farPlane * cam->nearPlane) / (cam->farPlane - cam->nearPlane);

	*Matrix_Index(cam->projectionMatrix, 3, 2) = -1.0f;
	*Matrix_Index(cam->projectionMatrix, 3, 3) = 0.0f;


	Matrix_Print(cam->projectionMatrix);
}

///
//Frees resources being taken by a camera
//
//Parameters:
//	cam: The camera which we are deleting
void Camera_Free(Camera* cam)
{
	Matrix_Free(cam->translationMatrix);
	Matrix_Free(cam->rotationMatrix);
	Matrix_Free(cam->projectionMatrix);
	free(cam);
}

void Camera_RefreshPerspectiveMatrix(Camera* cam)
{
	*Matrix_Index(cam->projectionMatrix, 0, 0) = (16.0f * 2.0f * cam->aspectX * cam->nearPlane) / (cam->rightPlane - cam->leftPlane);
	*Matrix_Index(cam->projectionMatrix, 0, 2) = (cam->rightPlane + cam->leftPlane) / (cam->rightPlane - cam->leftPlane);

	*Matrix_Index(cam->projectionMatrix, 1, 1) = (9.0f * 2.0f * cam->aspectY * cam->nearPlane) / (cam->topPlane - cam->bottomPlane);
	*Matrix_Index(cam->projectionMatrix, 1, 2) = (cam->topPlane + cam->bottomPlane) / (cam->topPlane - cam->bottomPlane);

	*Matrix_Index(cam->projectionMatrix, 2, 2) = -(cam->farPlane * cam->nearPlane) / (cam->farPlane - cam->nearPlane);
	*Matrix_Index(cam->projectionMatrix, 2, 3) = (-2.0f * cam->farPlane * cam->nearPlane) / (cam->farPlane - cam->nearPlane);
}

///
//Translates the camera
//
//Parameters:
//	cam: The camera to translate
//	translation: The Vector to translate by 
void Camera_Translate(Camera* cam, Vector* translation)
{
	for (int i = 0; i < 3; i++)
	{
		*Matrix_Index(cam->translationMatrix, i, 3) += -1.0f * translation->components[i];
	}
}

//
// Set the position of the camera
//
// Parameters:
//  cam: The camera object
//  translation: The vector position
void Camera_SetPosition(Camera* cam, Vector* translation)
{
	for (int i = 0; i < 3; i++)
	{
		*Matrix_Index(cam->translationMatrix, i, 3) = -1.0f * translation->components[i];
	}
}


///
//Rotates the camera
//
//Parameters:
//	Cam: The camera to rotate
//	axis: The axis to rotate around (Will be normalized!)
//	radians: The angle in radians to rotate by
void Camera_Rotate(Camera* cam, Vector* axis, const float radians)
{
	Vector_Normalize(axis);

	//Construct a rotation matrix based on rodriguez' formula
	Matrix rotMat;
	Matrix_INIT_ON_STACK(rotMat, 4, 4);

	//Row 1
	*Matrix_Index(&rotMat, 0, 0) = cosf(radians) + powf(axis->components[0], 2.0f) * (1.0f - cosf(radians));
	*Matrix_Index(&rotMat, 0, 1) = axis->components[0] * axis->components[1] * (1.0f - cosf(radians)) - axis->components[2] * sinf(radians);
	*Matrix_Index(&rotMat, 0, 2) = axis->components[0] * axis->components[2] * (1.0f - cosf(radians)) + axis->components[1] * sinf(radians);
	*Matrix_Index(&rotMat, 0, 3) = 0.0f;

	//Row 2
	*Matrix_Index(&rotMat, 1, 0) = axis->components[0] * axis->components[1] * (1.0f - cosf(radians)) + axis->components[2] * sinf(radians);
	*Matrix_Index(&rotMat, 1, 1) = cosf(radians) + powf(axis->components[1], 2.0f) * (1.0f - cosf(radians));
	*Matrix_Index(&rotMat, 1, 2) = axis->components[1] * axis->components[2] * (1.0f - cosf(radians)) - axis->components[0] * sinf(radians);
	*Matrix_Index(&rotMat, 1, 3) = 0.0f;


	//Row 3
	*Matrix_Index(&rotMat, 2, 0) = axis->components[0] * axis->components[2] * (1.0f - cosf(radians)) - axis->components[1] * sinf(radians);
	*Matrix_Index(&rotMat, 2, 1) = axis->components[1] * axis->components[2] * (1.0f - cosf(radians)) + axis->components[0] * sinf(radians);
	*Matrix_Index(&rotMat, 2, 2) = cosf(radians) + powf(axis->components[2], 2.0f) * (1.0f - cosf(radians));
	*Matrix_Index(&rotMat, 2, 3) = 0.0f;

	*Matrix_Index(&rotMat, 3, 3) = 1.0f;

	Matrix_TransformMatrix(&rotMat, cam->rotationMatrix);
}

///
//Scales the camera
//
//Not really sure how this will act yet...
//
//Parameters:
//	cam: thegamera to scale
//	scaleVector: a Vec-3 containing x, y, and z scale factors
void Camera_Scale(Camera* cam, const Vector* scaleVector)
{
	//For now this doesn't do anything.. Because the camera doesn't have a scale matrix.
}

///
//Compiles the camera into a 4x4 view matrix containing a translation to camera view
//Where the first 3x3 is a rotation matrix with scale information on the diagnol
//And the 4th column is a Vector 3 holding Rx(x)+Ry(y)+Rz(z)
//
//Parameters:
//	source: THe camera to inscribe into a matrix
//	dest: A pointer to a 4x4 destination matrix
void Camera_ToMatrix4(Camera* source, Matrix* dest)
{
	Matrix_GetProductMatrix(dest, source->rotationMatrix, source->translationMatrix);
}