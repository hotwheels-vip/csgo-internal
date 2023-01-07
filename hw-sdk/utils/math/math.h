#pragma once
#include "types/matrix.h"
#include "types/quaternion.h"
#include "types/vector.h"
#include <corecrt_math_defines.h>
#include <numbers>
#include <random>

namespace
{
	static std::random_device rd;
	static std::mt19937 gen{ rd( ) };
} // namespace

#undef min
#undef max

namespace math
{
	namespace util
	{
		constexpr static long double pi = std::numbers::pi_v< long double >;
		constexpr static float pi_f     = static_cast< float >( pi );

		// functions below might be unused, but why not have them.
		void fast_sqrt( float* __restrict p_out, float* __restrict p_in );

		float fast_asin( float x );

		float fast_sin( float x );
		float fast_cos( const float x );
	} // namespace util

	math::vec3 angle_vector( const math::vec3& angle );

	void angle_vectors( const math::vec3& angles, math::vec3* fw, math::vec3* rg = nullptr, math::vec3* up = nullptr );

	void vector_angles( math::vec3& fw, math::vec3& angles );

	math::vec3 calculate_angle( const math::vec3& src, const math::vec3& dst );

	bool world_to_screen( const math::vec3 origin, math::vec3& screen );

	void clamp_angle( math::vec3& angle );

	float approach_angle( float to, float from, const float speed );

	float snap_yaw( float value );

	void sin_cos( const float a, float* s, float* c );

	math::vec3 matrix_position( const math::matrix_3x4& src );

	template< typename T = float >
	T get_fov( const math::vec3& view_angles, const math::vec3& start, const math::vec3& end );

	template< typename T >
	constexpr T min( const T& t1, const T& t2 );

	template< typename T, typename... ts_ >
	constexpr T min( const T& t1, const T& t2, ts_&&... ts );

	template< typename T >
	constexpr T max( const T& t1, const T& t2 );

	template< typename T, typename... ts_ >
	constexpr T max( const T& t1, const T& t2, ts_&&... ts );

	float normalize_yaw( float yaw );

	template< typename T >
	constexpr T random( const T min, const T max );

	constexpr float rad2deg( const float x );
	constexpr float deg2rad( const float x );

	vec3 vector_transform( vec3 vector, matrix_3x4 matrix );

} // namespace math

#include "math.inl"
