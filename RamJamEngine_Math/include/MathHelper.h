#pragma once

#include <windows.h>
#include <float.h>

namespace RJE
{
#define RJE_PI 3.1415926535f
#define DegreesToRadian(X) X * (RJE_PI / 180.0f)
#define radianToDegrees(X) X * (180.0f / RJE_PI)
	
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
	};
}