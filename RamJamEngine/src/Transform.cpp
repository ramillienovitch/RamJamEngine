#include "Transform.h"

//////////////////////////////////////////////////////////////////////////
Transform::Transform()
{
	Scale = Vector3::one;
}

//////////////////////////////////////////////////////////////////////////
Matrix44 Transform::WorldMatrix()
{
	Matrix44 world;
	Matrix44 rotation = Matrix44::identity;
	Matrix44 rotationX = Matrix44::RotationX(EulerAngles.x);
	Matrix44 rotationY = Matrix44::RotationY(EulerAngles.y);
	Matrix44 rotationZ = Matrix44::RotationZ(EulerAngles.z);
	Matrix44 scale     = Matrix44::Scaling(Scale);
	Matrix44 position  = Matrix44::Translation(Position);
	world = scale;
	rotation = rotationX * rotationY * rotationZ;
	world *= rotation;
	world *= position;

	return world;
}
