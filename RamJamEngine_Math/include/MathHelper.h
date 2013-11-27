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
#define RJE_TWO_PI		(RJE_PI   * 2.0)
#define RJE_TWO_PI_F	(RJE_PI_F * 2.0f)
#define RJE_HALF_PI		(RJE_PI   * 0.5)
#define RJE_HALF_PI_F	(RJE_PI_F * 0.5f)

#define DegreesToRadian(X)   X * (RJE_PI   / 180.0)
#define DegreesToRadian_F(X) X * (RJE_PI_F / 180.0f)
#define RadianToDegrees(X)   X * (180.0  / RJE_PI)
#define RadianToDegrees_F(X) X * (180.0f / RJE_PI_F)

	struct Math
	{
		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static T Espilon()
		{ return std::numeric_limits<T>::epsilon(); }
		//--------------------------------------------------

		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static T NaN()
		{ return std::numeric_limits<T>::quiet_NaN(); }
		//--------------------------------------------------

		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static T Abs(T number)
		{ return (number > 0 ? number : -number); }
		//--------------------------------------------------

		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static T Rand()					// Returns random value in [0, 1[.
		{ return (rand()) / RAND_MAX; }
		//------------------------
		template<typename T>
		FORCEINLINE static T Rand(T a, T b)		// Returns random value in [a, b[.
		{ return a + static_cast<T>((rand()) / RAND_MAX)*(b-a); }
		//------------------------

		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static BOOL IsZero(T number)
		{ return (Abs(number) < std::numeric_limits<T>::epsilon()); }
		//------------------------
		template<typename T>
		FORCEINLINE static BOOL IsOne(T number)
		{ return ( number < 0 ? false : IsZero(number-1)); }
		//--------------------------------------------------

		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static T Min(const T& a, const T& b)
		{ return a < b ? a : b; }
		//------------------------
		template<typename T>
		FORCEINLINE static T Max(const T& a, const T& b)
		{ return a > b ? a : b; }
		//--------------------------------------------------

		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static T Clamp(const T& x, const T& low, const T& high)
		{ return x < low ? low : (x > high ? high : x); }
		//------------------------
		template<typename T>
		FORCEINLINE static T Clamp01(const T& x)
		{ return x < 0 ? 0 : (x > 1 ? 1 : x); }
		//--------------------------------------------------

		//--------------------------------------------------
		template<typename T>
		FORCEINLINE static T Lerp(const T& a, const T& b, float t)
		{ return t <= 0 ? a : (t >= 1 ? b : (a + (b-a)*t)); }
		//--------------------------------------------------

		//--------------------------------------------------
		// Returns the polar angle of the point (x,y) in [0, 2*PI[.
		template <typename Real>
		static Real AngleFromXY(Real x, Real y)
		{
			Real theta = static_cast<Real>(0.0);

			if(x >= static_cast<Real>(0.0))	// Quadrant I or IV
			{
				// If x = 0, then atanf(y/x) = +pi/2 if y > 0
				//                atanf(y/x) = -pi/2 if y < 0
				theta = atan(y / x); // in [-pi/2, +pi/2]

				if(theta < static_cast<Real>(0.0))
					theta += Pi_Two_f; // in [0, 2*pi[.
			}
			else	theta = atan(y/x) + Pi_f; // in [0, 2*pi[.

			return theta;
		}
		//--------------------------------------------------

		//--------------------------------------------------
		static const double Infinity;
		static const float  Infinity_f;
		static const double Pi;
		static const float  Pi_f;
		static const double Pi_Half;
		static const float  Pi_Half_f;
		static const double Pi_Two;
		static const float  Pi_Two_f;
	};
}