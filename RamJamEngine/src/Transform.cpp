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
Matrix44 Transform::WorldMatrixNoScale()
{
	Matrix44 position  = Matrix44::Translation(Position);
	Matrix44 rotation  = Rotation.ToMatrix();
	Matrix44 world = rotation * position;

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


//////////////////////////////////////////////////////////////////////////
Matrix44 Transform::MatrixFromTextureProperties( Vector2 tiling, Vector2 offset, f32 rotationAngle )
{
	Transform t;
	t.Position = Vector3(offset.x, offset.y, 0.0f);
	t.Scale    = Vector3(tiling.x, tiling.y, 1.0f);
	t.Rotation = Quaternion(0,0,rotationAngle);
	t.WorldMat = t.WorldMatrix();
	return t.WorldMat;
}
