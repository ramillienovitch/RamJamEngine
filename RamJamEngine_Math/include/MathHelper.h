#pragma once

#include <windows.h>
#include <float.h>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix44.h"

namespace RJE
{
#define RJE_PI			3.141592653589793238462643383279
#define RJE_PI_F		3.1415926535f
#define RJE_TWO_PI		(RJE_PI * 2.0)
#define RJE_TWO_PI_F	(RJE_PI_F * 2.0f)
#define RJE_HALF_PI		(RJE_PI * 0.5)
#define RJE_HALF_PI_F	(RJE_PI_F * 0.5f)

#define DegreesToRadian(X) X * (RJE_PI / 180.0)
#define DegreesToRadian_F(X) X * (RJE_PI_F / 180.0f)
#define RadianToDegrees(X) X * (180.0 / RJE_PI)
#define RadianToDegrees_F(X) X * (180.0f / RJE_PI_F)

	class Math
	{
	public:
		// Returns random float in [0, 1[.
		static float RandF()
		{
			return (float)(rand()) / (float)RAND_MAX;
		}

		// Returns random float in [a, b[.
		static float RandF(float a, float b)
		{
			return a + RandF()*(b-a);
		}

		template<typename T>
		static T Min(const T& a, const T& b)
		{
			return a < b ? a : b;
		}

		template<typename T>
		static T Max(const T& a, const T& b)
		{
			return a > b ? a : b;
		}

		template<typename T>
		static T Lerp(const T& a, const T& b, float t)
		{
			return a + (b-a)*t;
		}

		template<typename T>
		static T Clamp(const T& x, const T& low, const T& high)
		{
			return x < low ? low : (x > high ? high : x); 
		}

		// Returns the polar angle of the point (x,y) in [0, 2*PI[.
		static float AngleFromXY(float x, float y);

		static const float Infinity;
		static const float Pi;
		static const float Pi_Half;
		static const float Pi_Two;
	};
}