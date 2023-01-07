#pragma once
#include <algorithm>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>

#include "../../../dependencies/imgui/imgui.h"
#include "../../../globals/macros/macros.h"

// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-levels-3-and-4-c4244?view=msvc-160
#pragma warning( disable : 4244 )

struct color;

struct hsv {
	int h{ }, s{ }, v{ }, a{ };

	color to_color( );
};

struct color {
	int r{ }, g{ }, b{ }, a{ };

	// ctors
	color( ) : r{ 0 }, g{ 0 }, b{ 0 }, a{ 255 } { };

	color( int re, int gr, int bl, int al = 255 ) : r{ re }, g{ gr }, b{ bl }, a{ al } { };

	color( std::uint32_t hex ) : r( ( hex >> 24 ) & 0xFF ), g( ( hex >> 16 ) & 0xFF ), b( ( hex >> 8 ) & 0xFF ), a( hex & 0xFF ){ };

	constexpr static auto MAX_ALPHA = ( int )255;

	[[nodiscard]] ImU32 get_u32( const float alpha_multiplier = 1.0f ) const
	{
		return ImGui::GetColorU32( this->to_imvec4( alpha_multiplier ) );
	}
	color set_alpha( int new_alpha )
	{
		return color( r, g, b, static_cast< int >( new_alpha ) );
	}
	D3DCOLOR to_d3d( ) const
	{
		return D3DCOLOR_ARGB( a, r, g, b );
	}
	[[nodiscard]] ImVec4 to_imvec4( const float alpha_multiplier = 1.0f ) const
	{
		return ImVec4( this->r / 255.f, this->g / 255.f, this->b / 255.f, this->a / 255.f * alpha_multiplier );
	}
	hsv to_hsv( )
	{
		auto max_lambda = []( float a, float b, float c ) -> float { return ( ( a > b ) ? ( a > c ? a : c ) : ( b > c ? b : c ) ); };

		auto min_lambda = []( float a, float b, float c ) -> float { return ( ( a < b ) ? ( a < c ? a : c ) : ( b < c ? b : c ) ); };

		float hue, saturation, vibrance;
		float red   = r / 255.f;
		float green = g / 255.f;
		float blue  = b / 255.f;

		float cmax = max_lambda( red, green, blue );
		float cmin = min_lambda( red, green, blue );
		float diff = cmax - cmin;
		if ( cmax == cmin )
			hue = 0;
		else if ( cmax == red )
			hue = fmod( ( 60.f * ( ( green - blue ) / diff ) + 360.f ), 360.f );
		else if ( cmax == green )
			hue = fmod( ( 60.f * ( ( blue - red ) / diff ) + 120.f ), 360.f );
		else if ( cmax == blue )
			hue = fmod( ( 60.f * ( ( red - green ) / diff ) + 240.f ), 360.f );

		if ( cmax == 0.f )
			saturation = 0.f;
		else
			saturation = ( diff / cmax ) * 100.f;

		vibrance = cmax * 100.f;

		return { ROUND_UP( hue ), ROUND_UP( saturation ), ROUND_UP( vibrance ), a };
	}
	color lerp( const color col, const float t )
	{
		constexpr auto lerp = []( const int rom, const int to, const float t ) { return ( int )( ( 1.f - t ) * rom + t * to ); };

		color lerped = *this;

		lerped.r = lerp( r, col.r, t );
		lerped.g = lerp( g, col.g, t );
		lerped.b = lerp( b, col.b, t );
		lerped.a = lerp( a, col.a, t );

		return lerped;
	}

	static color blend( const color& a, const color& b, const float alpha_multiplier )
	{
		return color( a.r + std::clamp( alpha_multiplier, 0.f, 1.f ) * ( b.r - a.r ), a.g + std::clamp( alpha_multiplier, 0.f, 1.f ) * ( b.g - a.g ),
		              a.b + std::clamp( alpha_multiplier, 0.f, 1.f ) * ( b.b - a.b ),
		              a.a + std::clamp( alpha_multiplier, 0.f, 1.f ) * ( b.a - a.a ) );
	}

	bool operator==( const color& other ) const
	{
		return other.r == r && other.g == g && other.b == b && other.a == a;
	}
	bool operator!=( const color& other ) const
	{
		return other.r != r || other.g != g || other.b != b || other.a != a;
	}
	color& operator*=( const float f )
	{
		r = static_cast< int >( r * f );
		g = static_cast< int >( g * f );
		b = static_cast< int >( b * f );
		return *this;
	}
	// color operator( )( const int al ) const
	//{
	//	return color( r, g, b, al );
	// }
	color& operator/=( const float d )
	{
		const auto float_division = 1.f / d;
		*this *= float_division;

		return *this;
	}
	color& operator*( const float coeff ) const
	{
		auto color = *this;
		return color *= coeff;
	}
	// color& operator=( color col ) const
	//{
	//	color cf{ *this };
	//	cf.r = col.r;
	//	cf.g = col.g;
	//	cf.b = col.b;
	//	return cf;
	// }
	color& operator+( color col ) const
	{
		color cf = { *this };
		cf.r += col.r;
		cf.g += col.g;
		cf.b += col.b;
		cf.a += col.a;

		return cf;
	}
	color& operator-( color col ) const
	{
		color cf = { *this };
		cf.r -= col.r;
		cf.g -= col.g;
		cf.b -= col.b;
		cf.a -= col.a;

		return cf;
	}
};
