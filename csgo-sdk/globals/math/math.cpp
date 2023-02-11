#include "math.h"
#include "../../game/sdk/classes/c_angle.h"
#include "../../game/sdk/classes/c_vector.h"

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

void n_math::impl_t::angle_vectors( const c_angle& angle, c_vector* forward, c_vector* right, c_vector* up )
{
	float sp = { }, sy = { }, sr = { }, cp = { }, cy = { }, cr = { };

	this->sin_cos( deg2rad( angle.m_x ), &sp, &cp );
	this->sin_cos( deg2rad( angle.m_y ), &sy, &cy );
	this->sin_cos( deg2rad( angle.m_z ), &sr, &cr );

	if ( forward ) {
		forward->m_x = cp * cy;
		forward->m_y = cp * sy;
		forward->m_z = -sp;
	}

	if ( right ) {
		right->m_x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->m_y = -1 * sr * sp * sy + -1 * cr * cy;
		right->m_z = -1 * sr * cp;
	}

	if ( up ) {
		up->m_x = cr * sp * cy + -sr * -sy;
		up->m_y = cr * sp * sy + -sr * cy;
		up->m_z = cr * cp;
	}
}
