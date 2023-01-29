#include "mathematics.h"
#include "../../memory/memory.h"

void mathematics_t::sin_cos( float radians, float* sine, float* cosine )
{
	*sine   = sinf( radians );
	*cosine = cosf( radians );
}

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

void mathematics_t::vector_angles( const c_vector vector, c_angle& view )
{
	float pitch{ }, yaw{ };

	if ( vector.m_x == 0.f && vector.m_y == 0.f ) {
		pitch = ( vector.m_z > 0.f ) ? 270.f : 90.f;
		yaw   = 0.f;
	} else {
		pitch = std::atan2f( -vector.m_z, vector.length_2d( ) ) * 180.f / PI;

		if ( pitch < 0.f )
			pitch += 360.f;

		yaw = std::atan2f( vector.m_y, vector.m_x ) * 180.f / PI;

		if ( yaw < 0.f )
			yaw += 360.f;
	}

	view.m_x = pitch;
	view.m_y = yaw;
	view.m_z = 0.f;
}

void mathematics_t::angle_matrix( const c_angle& angles, matrix3x4_t& matrix ) {
	float sr{ }, sp{ }, sy{ }, cr{ }, cp{ }, cy{ };

	this->sin_cos( DEG2RAD( angles.m_y ), &sy, &cy );
	this->sin_cos( DEG2RAD( angles.m_x ), &sp, &cp );
	this->sin_cos( DEG2RAD( angles.m_z ), &sr, &cr );

	matrix[ 0 ][ 0 ] = cp * cy;
	matrix[ 1 ][ 0 ] = cp * sy;
	matrix[ 2 ][ 0 ] = -sp;

	matrix[ 0 ][ 1 ] = sr * sp * cy + cr * -sy;
	matrix[ 1 ][ 1 ] = sr * sp * sy + cr * cy;
	matrix[ 2 ][ 1 ] = sr * cp;
	matrix[ 0 ][ 2 ] = ( cr * sp * cy + -sr * -sy );
	matrix[ 1 ][ 2 ] = ( cr * sp * sy + -sr * cy );
	matrix[ 2 ][ 2 ] = cr * cp;

	matrix[ 0 ][ 3 ] = 0.0f;
	matrix[ 1 ][ 3 ] = 0.0f;
	matrix[ 2 ][ 3 ] = 0.0f;
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

float mathematics_t::calculate_fov( const c_angle& view_point, const c_angle& aim_angle )
{
	c_vector forward_angles{ }, forward_aim_angles{ };

	this->angle_vectors( view_point, &forward_aim_angles );
	this->angle_vectors( aim_angle, &forward_angles );

	return RAD2DEG( acos( forward_aim_angles.dot_product( forward_angles ) / forward_aim_angles.length_squared( ) ) );
}

c_vector mathematics_t::to_angle( const c_vector& in )
{
	return c_vector{ RAD2DEG( std::atan2( -in.m_z, std::hypot( in.m_x, in.m_y ) ) ), RAD2DEG( std::atan2( in.m_y, in.m_x ) ), 0.0f };
};

c_angle mathematics_t::calculate_angle( const c_vector& start, const c_vector& end )
{
	c_angle view{ };
	const c_vector delta = end - start;
	this->vector_angles( delta, view );
	view.normalize( );

	return view;
}

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
