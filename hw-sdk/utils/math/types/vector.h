#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <corecrt_math.h>
#include <type_traits>
#include <xtr1common>

namespace math
{
	template< class T = float >
	class vec2
	{
	public:
		T x{ }, y{ };
		constexpr vec2( const T _X = 0, const T _Y = 0 ) : x{ _X }, y{ _Y } { };

		bool valid( ) const
		{
			return std::isfinite< T >( x ) && std::isfinite< T >( y );
		};
		// operators:
		vec2 operator-( const vec2& sub ) const
		{
			return vec2( x - sub.x, y - sub.y );
		}
		vec2 operator-( const int& sub ) const
		{
			return vec2( x - sub, y - sub );
		}
		vec2 operator+( const vec2& add ) const
		{
			return vec2( x + add.x, y + add.y );
		}
		vec2 operator+( const int& add ) const
		{
			return vec2( x + add, y + add );
		}
		constexpr vec2& operator-=( const vec2& sub )
		{
			x -= sub.x;
			y -= sub.y;
			return *this;
		}
		constexpr vec2& operator+=( const vec2& add )
		{
			x += add.x;
			y += add.y;
			return *this;
		}
		[[nodiscard]] constexpr float dot_product( const vec2& dot ) const
		{
			return ( x * dot.x + y * dot.y );
		}

		[[nodiscard]] constexpr float length_sqr( ) const
		{
			return dot_product( *this );
		}

		[[nodiscard]] float length( ) const
		{
			return std::sqrtf( length_sqr( ) );
		}
	};

	class vec3
	{
	public:
		float x{ }, y{ }, z{ };

		constexpr vec3( float _x = 0, float _y = 0, float _z = 0 ) : x( _x ), y( _y ), z( _z ){ };

		constexpr vec3( float* _fp ) : x( _fp[ 0 ] ), y( _fp[ 1 ] ), z( _fp[ 2 ] ){ };

		bool valid( ) const
		{
			return std::isfinite( x ) && std::isfinite( y ) && std::isfinite( z );
		}

		constexpr void invalidate( )
		{
			x = y = z = std::numeric_limits< float >::infinity( );
		};

		[[nodiscard]] float* data( )
		{
			return reinterpret_cast< float* >( this );
		}

		bool is_equal( const vec3& to_this, const float error_margin = std::numeric_limits< float >::epsilon( ) ) const
		{
			return ( std::fabsf( x - to_this.x ) < error_margin && std::fabsf( y - to_this.y ) < error_margin &&
			         std::fabsf( z - to_this.z ) < error_margin );
		}

		bool is_zero( ) const
		{
			return ( std::fpclassify( x ) == FP_ZERO && std::fpclassify( y ) == FP_ZERO && std::fpclassify( z ) == FP_ZERO );
		}

		[[nodiscard]] constexpr float dot_product( const vec3& dot ) const
		{
			return ( x * dot.x + y * dot.y + z * dot.z );
		}

		[[nodiscard]] constexpr float length_sqr( ) const
		{
			return dot_product( *this );
		}

		[[nodiscard]] float length( ) const
		{
			return std::sqrtf( length_sqr( ) );
		}

		[[nodiscard]] constexpr float length_2d_sqr( ) const
		{
			return ( x * x + y * y );
		}

		[[nodiscard]] float length_2d( ) const
		{
			return std::sqrtf( length_2d_sqr( ) );
		}

		[[nodiscard]] float dist_to( const vec3& end ) const
		{
			return ( *this - end ).length( );
		}

		[[nodiscard]] constexpr float dist_to_sqr( const vec3& end ) const
		{
			return ( *this - end ).length_sqr( );
		}

		float normalize_in_place( )
		{
			const float m_length = length( ), radius = 1.0f / ( m_length + std::numeric_limits< float >::epsilon( ) );

			x *= radius;
			y *= radius;
			z *= radius;

			return m_length;
		}

		constexpr vec3& normalize( )
		{
			x = std::isfinite( x ) ? std::remainderf( x, 360.f ) : 0.f;
			y = std::isfinite( y ) ? std::remainderf( y, 360.f ) : 0.f;
			z = std::clamp( z, -50.f, 50.f );

			return *this;
		}

		constexpr vec3 normalized( )
		{
			vec3 ret = *this;
			ret.normalize( );

			return ret;
		}

		constexpr vec3 clamp( )
		{
			x = std::clamp( x, -89.f, 89.f );
			y = std::clamp( y, -180.f, 180.f );
			z = std::clamp( z, -50.f, 50.f );

			return *this;
		}

		constexpr vec3& sanitize( )
		{
			normalize( );
			clamp( );

			return *this;
		}

		constexpr vec3 cross_product( const vec3& cross ) const
		{
			return vec3( y * cross.z - z * cross.y, z * cross.x - x * cross.z, x * cross.y - y * cross.x );
		}

		//	operators:
		float& operator[]( const std::size_t i )
		{
			return data( )[ i ];
		}

		bool operator==( const vec3& base ) const
		{
			return is_equal( base );
		}

		bool operator!=( const vec3& base ) const
		{
			return !is_equal( base );
		}

		constexpr vec3& operator=( const vec3& base )
		{
			x = base.x;
			y = base.y;
			z = base.z;
			return *this;
		}

		constexpr vec3& operator+=( const vec3& base )
		{
			x += base.x;
			y += base.y;
			z += base.z;
			return *this;
		}

		constexpr vec3& operator-=( const vec3& base )
		{
			x -= base.x;
			y -= base.y;
			z -= base.z;
			return *this;
		}

		constexpr vec3& operator*=( const vec3& base )
		{
			x *= base.x;
			y *= base.y;
			z *= base.z;
			return *this;
		}

		constexpr vec3& operator/=( const vec3& base )
		{
			x /= base.x;
			y /= base.y;
			z /= base.z;
			return *this;
		}

		constexpr vec3& operator+=( const float add )
		{
			x += add;
			y += add;
			z += add;
			return *this;
		}

		constexpr vec3& operator-=( const float sub )
		{
			x -= sub;
			y -= sub;
			z -= sub;
			return *this;
		}

		constexpr vec3& operator*=( const float mult )
		{
			x *= mult;
			y *= mult;
			z *= mult;
			return *this;
		}

		constexpr vec3& operator/=( const float div )
		{
			x /= div;
			y /= div;
			z /= div;
			return *this;
		}

		vec3 operator+( const vec3& add ) const
		{
			return vec3( x + add.x, y + add.y, z + add.z );
		}

		vec3 operator-( const vec3& sub ) const
		{
			return vec3( x - sub.x, y - sub.y, z - sub.z );
		}

		vec3 operator*( const vec3& mult ) const
		{
			return vec3( x * mult.x, y * mult.y, z * mult.z );
		}

		vec3 operator/( const vec3& div ) const
		{
			return vec3( x / div.x, y / div.y, z / div.z );
		}

		vec3 operator+( const float add ) const
		{
			return vec3( x + add, y + add, z + add );
		}

		vec3 operator-( const float sub ) const
		{
			return vec3( x - sub, y - sub, z - sub );
		}

		vec3 operator*( const float mult ) const
		{
			return vec3( x * mult, y * mult, z * mult );
		}

		vec3 operator/( const float div ) const
		{
			return vec3( x / div, y / div, z / div );
		}
	};

	struct vec4 {
		vec4( ) = default;
		constexpr vec4( float xyzw ) : x( xyzw ), y( xyzw ), z( xyzw ), w( xyzw ){ };
		constexpr vec4( float x, float y ) : x( x ), y( y ), z( x ), w( y ){ };
		constexpr vec4( float x, float y, float z, float w ) : x( x ), y( y ), z( z ), w( w ){ };

		float x, y, z, w;
	};

	struct box {
		box( ) = default;
		constexpr box( int xywh ) : x( xywh ), y( xywh ), w( xywh ), h( xywh ){ };
		constexpr box( int x, int y ) : x( x ), y( y ), w( x ), h( y ){ };
		constexpr box( int x, int y, int w, int h ) : x( x ), y( y ), w( w ), h( h ){ };

		int x, y, w, h;

		vec2< int > width( )
		{
			return { w - x, h - y };
		}
	};
}; // namespace math
