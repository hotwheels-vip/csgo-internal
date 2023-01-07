#include "math.h"
#include "../../globals/ctx/ctx.h"
#include "../modules/modules.h"
#include "types/vector.h"
#include <algorithm>

math::vec3 math::angle_vector( const math::vec3& angle )
{
	const auto sy = std::sin( angle.y / 180.f * math::util::pi_f );
	const auto cy = std::cos( angle.y / 180.f * math::util::pi_f );

	const auto sp = std::sin( angle.x / 180.f * math::util::pi_f );
	const auto cp = std::cos( angle.x / 180.f * math::util::pi_f );

	return math::vec3( cp * cy, cp * sy, -sp );
}

float math::normalize_yaw( float yaw )
{
	while ( yaw < -180.0f )
		yaw += 360.0f;
	while ( yaw > 180.0f )
		yaw -= 360.0f;

	return yaw;
}

void math::angle_vectors( const math::vec3& angles, math::vec3* fw, math::vec3* rg, math::vec3* up )
{
	const float cp = std::cos( math::deg2rad( angles.x ) ), sp = std::sin( math::deg2rad( angles.x ) );
	const float cy = std::cos( math::deg2rad( angles.y ) ), sy = std::sin( math::deg2rad( angles.y ) );
	const float cr = std::cos( math::deg2rad( angles.z ) ), sr = std::sin( math::deg2rad( angles.z ) );

	if ( fw ) {
		fw->x = cp * cy;
		fw->y = cp * sy;
		fw->z = -sp;
	}

	if ( rg ) {
		rg->x = -1.f * sr * sp * cy + -1.f * cr * -sy;
		rg->y = -1.f * sr * sp * sy + -1.f * cr * cy;
		rg->z = -1.f * sr * cp;
	}

	if ( up ) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

constexpr float math::rad2deg( const float x )
{
	return x * ( 180.f / math::util::pi_f );
}

constexpr float math::deg2rad( const float x )
{
	return x * ( math::util::pi_f / 180.f );
}

math::vec3 math::vector_transform( vec3 vector, matrix_3x4 matrix )
{
	vec3 output{ };

	for ( int iterator = 0; iterator < 3; iterator++ )
		output[ iterator ] = vector.dot_product( matrix[ iterator ] ) + matrix[ iterator ][ 3 ];

	return output;
}

math::vec3 math::matrix_position( const math::matrix_3x4& src )
{
	return { src[ 0 ][ 3 ], src[ 1 ][ 3 ], src[ 2 ][ 3 ] };
}

float math::approach_angle( float to, float from, const float speed )
{
	float delta = std::remainderf( to - from, 360.f );

	if ( delta > speed )
		from += speed;
	else if ( delta < -speed )
		from -= speed;
	else
		from = to;

	return std::clamp( from, -180.f, 180.f );
}

float math::snap_yaw( float value )
{
	float sign = 1.0f;
	if ( value < 0.0f ) {
		sign  = -1.0f;
		value = -value;
	}

	if ( value < 23.0f )
		value = 0.0f;
	else if ( value < 67.0f )
		value = 45.0f;
	else if ( value < 113.0f )
		value = 90.0f;
	else if ( value < 157 )
		value = 135.0f;
	else
		value = 180.0f;

	return ( value * sign );
}

void math::sin_cos( const float a, float* s, float* c )
{
	*s = std::sinf( a );
	*c = std::cosf( a );
}

void math::vector_angles( math::vec3& fw, math::vec3& angles )
{
	if ( fw.x == 0.f && fw.y == 0.f ) {
		angles.x = ( fw.z > 0.f ) ? 270.f : 90.f;
		angles.y = 0.f;
	} else {
		angles.x = -math::rad2deg( std::atan2( -fw.z, fw.length_2d( ) ) );
		angles.y = math::rad2deg( std::atan2( fw.y, fw.x ) );

		if ( angles.y > 90.f )
			angles.y -= 180.f;
		else if ( angles.y < 90.f )
			angles.y += 180.f;
		else if ( angles.y == 90.f )
			angles.y = 0.f;
	}

	angles.z = 0.f;
}

math::vec3 math::calculate_angle( const math::vec3& src, const math::vec3& dst )
{
	math::vec3 angles{ };
	math::vec3 delta = src - dst;

	math::vector_angles( delta, angles );

	return angles.normalize_in_place( );
}

bool math::world_to_screen( const math::vec3 origin, math::vec3& screen )
{
	static math::matrix_4x4* view = nullptr;

	if ( !view ) {
		auto pattern = g_client_dll.pattern_scan( _( "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) );
		pattern      = ( *reinterpret_cast< uint32_t* >( pattern + 0x3 ) ) + 0xB0;

		view = pattern.as< math::matrix_4x4* >( );
	}

	float w  = 0.0f;
	screen.x = view->data[ 0 ][ 0 ] * origin.x + view->data[ 0 ][ 1 ] * origin.y + view->data[ 0 ][ 2 ] * origin.z + view->data[ 0 ][ 3 ];
	screen.y = view->data[ 1 ][ 0 ] * origin.x + view->data[ 1 ][ 1 ] * origin.y + view->data[ 1 ][ 2 ] * origin.z + view->data[ 1 ][ 3 ];
	w        = view->data[ 3 ][ 0 ] * origin.x + view->data[ 3 ][ 1 ] * origin.y + view->data[ 3 ][ 2 ] * origin.z + view->data[ 3 ][ 3 ];

	if ( w < 0.01f )
		return false;

	const float invw = 1.0f / w;
	screen.x *= invw;
	screen.y *= invw;

	float x = static_cast< float >( g_ctx.screen_size.x ) / 2.0f;
	float y = static_cast< float >( g_ctx.screen_size.y ) / 2.0f;
	x += 0.5f * screen.x * static_cast< float >( g_ctx.screen_size.x ) + 0.5f;
	y -= 0.5f * screen.y * static_cast< float >( g_ctx.screen_size.y ) + 0.5f;
	screen.x = x;
	screen.y = y;
	return true;
}

void math::clamp_angle( math::vec3& angle )
{
	if ( angle.x < -89.f )
		angle.x += 89.f;

	if ( angle.x > 89.f )
		angle.x = 89.f;

	if ( angle.y < -180.f )
		angle.y += 360.f;
	else if ( angle.y > 180.f )
		angle.y -= 360.f;

	angle.z = 0.f;
}

void math::util::fast_sqrt( float* __restrict p_out, float* __restrict p_in )
{
	_mm_store_ss( p_out, _mm_sqrt_ss( _mm_load_ss( p_in ) ) );
}

float math::util::fast_asin( float x )
{
	const auto negate = float( x < 0 );
	x                 = abs( x );
	auto ret          = -0.0187293;
	ret *= x;
	ret += 0.0742610;
	ret *= x;
	ret -= 0.2121144;
	ret *= x;
	ret += 1.5707288;
	ret = 3.14159265358979 * 0.5 - sqrt( 1.0 - x ) * ret;
	return float( ret - 2 * negate * ret );
}

float math::util::fast_sin( float x )
{
	x *= float( 0.159155 );
	x -= floor( x );
	const auto xx = x * x;
	auto y        = -6.87897;
	y             = y * xx + 33.7755;
	y             = y * xx - 72.5257;
	y             = y * xx + 80.5874;
	y             = y * xx - 41.2408;
	y             = y * xx + 6.28077;
	return float( x * y );
}

float math::util::fast_cos( const float x )
{
	return ::math::util::fast_sin( x + 1.5708f );
}
