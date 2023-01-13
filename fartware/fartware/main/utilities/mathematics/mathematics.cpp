#include "mathematics.h"
#include "../../memory/memory.h"

c_vector mathematics_t::vector_transform( const c_vector& vector_to_transform, const matrix3x4_t& matrix )
{
	return c_vector( vector_to_transform.dot_product( matrix[ 0 ] ) + matrix[ 0 ][ 3 ],
	                 vector_to_transform.dot_product( matrix[ 1 ] ) + matrix[ 1 ][ 3 ],
	                 vector_to_transform.dot_product( matrix[ 2 ] ) + matrix[ 2 ][ 3 ] );
}

c_vector mathematics_t::vector_angle( const c_vector vector )
{
	c_vector ret{ };
	if ( vector.m_x == 0.0f && vector.m_y == 0.0f ) {
		ret.m_x = ( vector.m_z > 0.0f ) ? 270.0f : 90.0f;
		ret.m_y = 0.0f;
	} else {
		ret.m_x = RAD2DEG( std::atan2f( -vector.m_z, vector.length_2d( ) ) );
		ret.m_y = RAD2DEG( std::atan2f( vector.m_y, vector.m_x ) );

		if ( ret.m_y < 0.0f )
			ret.m_y += 360.0f;

		if ( ret.m_x < 0.0f )
			ret.m_x += 360.0f;
	}
	ret.m_z = 0.0f;
	return ret;
}

void mathematics_t::angle_vectors( const c_angle& angle, c_vector* forward, c_vector* right, c_vector* up )
{
	float sp = { }, sy = { }, sr = { }, cp = { }, cy = { }, cr = { };

	DirectX::XMScalarSinCos( &sp, &cp, DEG2RAD( angle.m_x ) );
	DirectX::XMScalarSinCos( &sy, &cy, DEG2RAD( angle.m_y ) );
	DirectX::XMScalarSinCos( &sr, &cr, DEG2RAD( angle.m_z ) );

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

std::int32_t mathematics_t::time_to_ticks( float time )
{
	return static_cast< std::int32_t >( 0.5f + time / memory.m_globals->m_interval_per_tick );
}

float mathematics_t::ticks_to_time( std::int32_t ticks )
{
	return static_cast< float >( ticks ) * memory.m_globals->m_interval_per_tick;
}

c_vector mathematics_t::to_angle( const c_vector& in )
{
	return c_vector{ RAD2DEG( std::atan2( -in.m_z, std::hypot( in.m_x, in.m_y ) ) ), RAD2DEG( std::atan2( in.m_y, in.m_x ) ), 0.0f };
};

float mathematics_t::normalize_yaw( float yaw )
{
	if ( !std::isfinite( yaw ) )
		return 0.0f;

	if ( yaw >= -180.f && yaw <= 180.f )
		return yaw;

	const float rot = std::round( std::abs( yaw / 360.f ) );

	yaw = ( yaw < 0.f ) ? yaw + ( 360.f * rot ) : yaw - ( 360.f * rot );
	return yaw;
}
