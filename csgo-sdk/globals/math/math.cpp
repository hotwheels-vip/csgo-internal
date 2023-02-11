#include "math.h"

float n_math::impl_t::normalize_angle( float angle, float start, float end ) const
{
	const float delta        = end - start;
	const float offset_angle = angle - start;

	return ( offset_angle - ( floor( offset_angle / delta ) * delta ) ) + start;
}

void n_math::impl_t::sin_cos( float radians, float* sine, float* cosine ) const
{
	*sine   = sinf( radians );
	*cosine = cosf( radians );
}
