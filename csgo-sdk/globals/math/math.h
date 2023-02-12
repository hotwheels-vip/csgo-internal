#pragma once
#include <numbers>

class c_angle;
class c_vector;
struct matrix3x4_t;

constexpr float rad2deg( const float x )
{
	return x * ( 180.f / std::numbers::pi_v< float > );
}

constexpr float deg2rad( const float x )
{
	return x * ( std::numbers::pi_v< float > / 180.f );
}

namespace n_math
{
	struct impl_t {
		float normalize_angle( float angle, float start = -180.f, float end = 180.f ) const;
		void sin_cos( float radians, float* sine, float* cosine ) const;
		void angle_vectors( const c_angle& angle, c_vector* forward, c_vector* right = nullptr, c_vector* up = nullptr );
		c_angle calculate_angle( const c_vector& start, const c_vector& end );
		float calculate_fov( const c_angle& view_point, const c_angle& aim_angle );
		void vector_angles( const c_vector& forward, c_angle& view );
		c_vector vector_transform( const c_vector& transform, const matrix3x4_t& matrix );
	};
} // namespace n_math

inline n_math::impl_t g_math{ };
