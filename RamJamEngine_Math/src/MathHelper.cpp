#include "MathHelper.h"

#include <windows.h>
#include <float.h>
#include <cmath>

namespace RJE
{
	const float Math::Infinity = FLT_MAX;
	const float Math::Pi       = RJE_PI;

	float Math::AngleFromXY(float x, float y)
	{
		float theta = 0.0f;

		if(x >= 0.0f)	// Quadrant I or IV
		{
			// If x = 0, then atanf(y/x) = +pi/2 if y > 0
			//                atanf(y/x) = -pi/2 if y < 0
			theta = atanf(y / x); // in [-pi/2, +pi/2]

			if(theta < 0.0f)
				theta += 2.0f*Pi; // in [0, 2*pi[.
		}
		else	theta = atanf(y/x) + Pi; // in [0, 2*pi[.

		return theta;
	}
}