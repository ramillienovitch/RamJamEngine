#include "Transform.h"

//////////////////////////////////////////////////////////////////////////
Transform::Transform()
{
	Scale = Vector3::one;
}

//////////////////////////////////////////////////////////////////////////
Matrix44 Transform::WorldMatrix()
{
	Matrix44 position  = Matrix44::Translation(Position);
	Matrix44 scale     = Matrix44::Scaling(Scale);
	Matrix44 rotation  = Rotation.ToMatrix();
	Matrix44 world = scale * rotation * position;

	return world;
}

//////////////////////////////////////////////////////////////////////////
Vector3 Transform::Right()
{ return Rotation.GetRightVector(); }
//----------------------------------------
Vector3 Transform::Up()
{ return Rotation.GetUpVector(); }
//----------------------------------------
Vector3 Transform::Forward()
{ return Rotation.GetForwardVector(); }
