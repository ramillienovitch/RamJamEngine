#pragma once

#include "Types.h"
#include "MathHelper.h"

//////////////////////////////////////////////////////////////////////////
struct Transform
{
	Vector3					Position;
	Vector3					LocalPosition;
	//-----------
	Vector3					Scale;
	Vector3					LocalScale;
	//-----------
	Vector3					EulerAngles;
	Vector3					LocalEulerAngles;
	Quaternion				Rotation;
	Quaternion				LocalRotation;
	//-----------
// 	Matrix44				WorldMatrix;
// 	Matrix44				LocalMatrix;
	//-----------
	Transform*				Parent;
	std::set<Transform*>	Children;

	//---------------------------

	Transform();

	Vector3 Right();
	Vector3 Up();
	Vector3 Forward();

	Matrix44 WorldMatrix ();

	//---------------------------

	static Matrix44 MatrixFromTextureProperties(Vector2 tiling, Vector2 offset, f32 rotationAngle);

	// TODO: add transform "everything" !
};