#include "math.h"

float n_math::impl_t::normalize_angle( float angle, float start, float end ) const
{
	const float delta        = end - start;
	const float offset_angle = angle - start;

	return ( offset_angle - ( floor( offset_angle / delta ) * delta ) ) + start;
}
