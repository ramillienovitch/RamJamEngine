#include "Camera.h"
#include "Input.h"
#include "Timer.h"
#include "System.h"

//////////////////////////////////////////////////////////////////////////
Camera::Camera()
{
	mTrf.Position = Vector3(0.0f, 1.0f, -5.0f);
	mUp     = Vector3(0.0f, 1.0f, 0.0f);
	mLookAt = Vector3::zero;
	mPitch  = 0.0f;
	mYaw    = 0.0f;

	mSpeedMultiplier = 1.0f;
	mSpeed           = 5.0f;

	mView      = Matrix44::identity;
	mPerspProj = Matrix44::identity;
	mOrthoProj = Matrix44::identity;
	mCurrentProjectionMatrix = &mPerspProj;

	bIsOrtho = false;

	mMode = Camera_FPS;
	mSettings.FOV         = 60;
	mSettings.OrthoZoom   = 0.01f;
	mSettings.NearZ       = 0.01f;
	mSettings.FarZ        = 1000.0f;
	mSettings.AspectRatio = 1.77777777777777777f;	// Default 16/9

	mLastMousePos.x  = 0;
	mLastMousePos.y  = 0;
	mCameraTheta	 = 1.5f*RJE::Math::Pi_f;
	mCameraPhi		 = 0.25f*RJE::Math::Pi_f;
	mCameraRadius	 = 10.0f;

	mCameraAnimated = false;
	mAnimationSpeed = 10.0f;
}

//////////////////////////////////////////////////////////////////////////
BOOL Camera::IsOrtho() {return bIsOrtho;}

//////////////////////////////////////////////////////////////////////////
void Camera::SetCameraOrtho(BOOL orthographic = true)
{
	if (orthographic)
	{
		mCurrentProjectionMatrix = &mOrthoProj;
		bIsOrtho = true;
	}
	else
	{
		mCurrentProjectionMatrix = &mPerspProj;
		bIsOrtho = false;
	}
}

//////////////////////////////////////////////////////////////////////////
void Camera::Update()
{
	if (Console::Instance()->IsActive())
		return;

	int mouseX = Input::Instance()->GetMousePosX();
	int mouseY = Input::Instance()->GetMousePosY();

	if (Input::Instance()->GetKeyboard(LeftShift))
	{
		if( Input::Instance()->GetMouseButton(RButton) )
		{
			if (IsOrtho())
			{
				// Make each pixel correspond to 0.001 unit in the scene.
				float dx = 0.001f*static_cast<float>(mouseX - mLastMousePos.x);

				mSettings.OrthoZoom += dx;
				mSettings.OrthoZoom = RJE::Math::Clamp(mSettings.OrthoZoom, 0.0001f, 10000.0f);
				UpdateProjMatrix((float)System::Instance()->mScreenWidth, (float)System::Instance()->mScreenHeight);
			}
			else
			{
				// Make each pixel correspond to 0.05 unit in the scene.
				float dx = 0.05f*static_cast<float>(mouseX - mLastMousePos.x);

				mSettings.FOV += dx;
				mSettings.FOV = RJE::Math::Clamp(mSettings.FOV, 1.0f, 179.0f);
				UpdateProjMatrix((float)System::Instance()->mScreenWidth, (float)System::Instance()->mScreenHeight);
			}
		}
		if (Input::Instance()->GetKeyboardDown(Return))
		{
			if (mMode == Camera_TrackBall)	mMode = Camera_FPS;
			else
			{
				mMode = Camera_TrackBall;
				mLookAt = Vector3::zero;
			}
		}
	}
	else
	{
		if (Input::Instance()->GetKeyboardDown(Return))
		{
			SetCameraOrtho(!IsOrtho());
			UpdateProjMatrix((float)System::Instance()->mScreenWidth, (float)System::Instance()->mScreenHeight);
		}
	}

	if (mMode == Camera_TrackBall)
	{
		if (Input::Instance()->GetKeyboardDown(Spacebar))
			mCameraAnimated = !mCameraAnimated;

		if (mCameraAnimated)
			mCameraTheta = RJE::Math::Deg2Rad_f*mAnimationSpeed*Timer::Instance()->Time();

		// Wheel Scroll
		if (Input::Instance()->GetMouseButtonDown(WheelUp))
		{
			if (Input::Instance()->GetKeyboard(LeftShift) || Input::Instance()->GetKeyboard(RightShift))
				mCameraRadius -= 1.0f;
			else
				mCameraRadius -= 0.25f;
		}
		if (Input::Instance()->GetMouseButtonDown(WheelDown))
		{
			if (Input::Instance()->GetKeyboard(LeftShift) || Input::Instance()->GetKeyboard(RightShift))
				mCameraRadius += 1.0f;
			else
				mCameraRadius += 0.25f;
		}
		mCameraRadius = RJE::Math::Clamp(mCameraRadius, 2.0f, 500.0f);

		if (Input::Instance()->GetKeyboard(LeftCtrl))
		{
			if( Input::Instance()->GetMouseButton(RButton) )
			{
				if (IsOrtho())
				{
					// Make each pixel correspond to 0.001 unit in the scene.
					float dx = 0.001f*static_cast<float>(mouseX - mLastMousePos.x);

					mSettings.OrthoZoom += dx;
					mSettings.OrthoZoom = RJE::Math::Clamp(mSettings.OrthoZoom, 0.0001f, 10000.0f);
					UpdateProjMatrix((float)System::Instance()->mScreenWidth, (float)System::Instance()->mScreenHeight);
				}
				else
				{
					// Make each pixel correspond to 0.05 unit in the scene.
					float dx = 0.05f*static_cast<float>(mouseX - mLastMousePos.x);

					mSettings.FOV += dx;
					mSettings.FOV = RJE::Math::Clamp(mSettings.FOV, 1.0f, 179.0f);
					UpdateProjMatrix((float)System::Instance()->mScreenWidth, (float)System::Instance()->mScreenHeight);
				}
			}
		}
		else
		{
			if( Input::Instance()->GetMouseButton(LButton) )
			{
				// Make each pixel correspond to a quarter of a degree.
				float dx = RJE::Math::Deg2Rad_f*0.25f*static_cast<float>(mouseX - mLastMousePos.x);
				float dy = RJE::Math::Deg2Rad_f*0.25f*static_cast<float>(mouseY - mLastMousePos.y);

				// Update angles based on input to orbit camera around box.
				if (!mCameraAnimated)
					mCameraTheta -= dx;

				mCameraPhi -= dy;
				mCameraPhi = RJE::Math::Clamp(mCameraPhi, 0.1f, RJE::Math::Pi_f-0.1f);
			}
			else if( Input::Instance()->GetMouseButton(RButton) )
			{
				// Make each pixel correspond to 0.05 unit in the scene.
				float dx = 0.05f*static_cast<float>(mouseX - mLastMousePos.x);

				// Update the camera radius based on input.
				mCameraRadius += dx;
				mCameraRadius = RJE::Math::Clamp(mCameraRadius, 0.5f, 100.0f);
			}
		}

		//-------------------

		// Convert Spherical to Cartesian coordinates.
		float x = mCameraRadius*sinf(mCameraPhi)*cosf(mCameraTheta);
		float z = mCameraRadius*sinf(mCameraPhi)*sinf(mCameraTheta);
		float y = mCameraRadius*cosf(mCameraPhi);

		mTrf.Position = Vector3(x, y, z);
		mUp = Vector3::up;
	}
	else	// FPS Camera Mode
	{
		if( Input::Instance()->GetMouseButton(LButton) )
		{
			float dx = 0.5f*static_cast<float>(mouseX - mLastMousePos.x);
			float dy = 0.5f*static_cast<float>(mouseY - mLastMousePos.y);
			if (mPitch-dy > -90.0f && mPitch-dy < 90)
				mPitch -= dy;
			mYaw   -= dx;
		}
		if( Input::Instance()->GetKeyboardDown(LeftShift) )		mSpeedMultiplier = 5.0f;
		if( Input::Instance()->GetKeyboardUp(LeftShift) )		mSpeedMultiplier = 1.0f;
		if( Input::Instance()->GetKeyboard(Z) )					mTrf.Position += mTrf.Forward() * Timer::Instance()->DeltaTime() * mSpeed * mSpeedMultiplier;
		if( Input::Instance()->GetKeyboard(S) )					mTrf.Position -= mTrf.Forward() * Timer::Instance()->DeltaTime() * mSpeed * mSpeedMultiplier;
		if( Input::Instance()->GetKeyboard(D) )					mTrf.Position += mTrf.Right()   * Timer::Instance()->DeltaTime() * mSpeed * mSpeedMultiplier;
		if( Input::Instance()->GetKeyboard(Q) )					mTrf.Position -= mTrf.Right()   * Timer::Instance()->DeltaTime() * mSpeed * mSpeedMultiplier;
		if( Input::Instance()->GetKeyboard(Spacebar) )			mTrf.Position += Vector3(0,1,0) * Timer::Instance()->DeltaTime() * mSpeed * mSpeedMultiplier;
		if( Input::Instance()->GetKeyboard(LeftCtrl) )			mTrf.Position -= Vector3(0,1,0) * Timer::Instance()->DeltaTime() * mSpeed * mSpeedMultiplier;
		mTrf.Rotation = Quaternion(mPitch, mYaw, 0.0f);
		mLookAt = mTrf.Position + mTrf.Forward();
		mUp = Vector3::up;
		
	}
	mLastMousePos.x = f32(mouseX);
	mLastMousePos.y = f32(mouseY);
	// Build the view matrix.
	UpdateViewMatrix();
}

//////////////////////////////////////////////////////////////////////////
void Camera::UpdateViewMatrix()
{
	Vector3 eyeDir = mLookAt-mTrf.Position;
	mView = Matrix44::LookAt(mTrf.Position, eyeDir, mUp);
}

//////////////////////////////////////////////////////////////////////////
// Updates the current projection matrix
void Camera::UpdateProjMatrix(float sizeWidth, float sizeHeight)
{
	if (bIsOrtho)	UpdateOrthoMatrix(sizeWidth, sizeHeight);
	else			UpdatePerspMatrix();
}

//////////////////////////////////////////////////////////////////////////
// Only updates the perspective matrix
void Camera::UpdatePerspMatrix()
{ mPerspProj = Matrix44::PerspectiveFov(RJE::Math::Deg2Rad_f*mSettings.FOV, mSettings.AspectRatio, mSettings.NearZ, mSettings.FarZ); }

//////////////////////////////////////////////////////////////////////////
// Only updates the orthographic matrix
void Camera::UpdateOrthoMatrix(float sizeWidth, float sizeHeight)
{ mOrthoProj = Matrix44::Orthographic(sizeWidth*mSettings.OrthoZoom, sizeHeight*mSettings.OrthoZoom, mSettings.NearZ, mSettings.FarZ); }
