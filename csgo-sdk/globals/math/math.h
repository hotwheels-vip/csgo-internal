#pragma once
#include <numbers>

class c_angle;
class c_vector;

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
	};
} // namespace n_math

inline n_math::impl_t g_math{ };