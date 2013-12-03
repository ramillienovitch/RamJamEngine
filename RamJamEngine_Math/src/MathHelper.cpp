#include "MathHelper.h"

#include <windows.h>
#include <float.h>
#include <cmath>

namespace RJE
{
	const double Math::Infinity   = DBL_MAX;
	const double Math::Pi         = RJE_PI;
	const double Math::Pi_Half    = RJE_HALF_PI;
	const double Math::Pi_Two     = RJE_TWO_PI;
	const double Math::Deg2Rad    = Math::Pi / 180.0;
	const double Math::Rad2Deg    = 180.0 / Math::Pi;
	//-----------------
	const float  Math::Infinity_f = FLT_MAX;
	const float  Math::Pi_f       = RJE_PI_F;
	const float  Math::Pi_Half_f  = RJE_HALF_PI_F;
	const float  Math::Pi_Two_f   = RJE_TWO_PI_F;
	const float  Math::Deg2Rad_f  = Math::Pi_f / 180.0f;
	const float  Math::Rad2Deg_f  = 180.0f / Math::Pi_f;
}