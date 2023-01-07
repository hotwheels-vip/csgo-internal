#pragma once

namespace math
{
	// basically a vec4
	template< typename T = float >
	struct quaternion {
		T x, y, z, w;

		quaternion( ){ };

		constexpr quaternion( float ix, float iy, float iz, float iw ) : x( ix ), y( iy ), z( iz ), w( iw ){ };

		constexpr void init( float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f )
		{
			x = ix;
			y = iy;
			z = iz;
			w = iw;
		}
	};
} // namespace math
