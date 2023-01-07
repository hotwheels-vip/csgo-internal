#include "../../sdk/structs/color.h"

color hsv::to_color( )
{
	float _s = s / 100.f;
	float _v = v / 100.f;
	float _c = _s * _v;
	float _x = static_cast< float >( _c * ( 1 - abs( fmod( h / 60.0, 2 ) - 1 ) ) );
	float m  = _v - _c;
	float r, g, b;

	if ( h >= 0 && h < 60 ) {
		r = _c, g = _x, b = 0;
	} else if ( h >= 60 && h < 120 ) {
		r = _x, g = _c, b = 0;
	} else if ( h >= 120 && h < 180 ) {
		r = 0, g = _c, b = _x;
	} else if ( h >= 180 && h < 240 ) {
		r = 0, g = _x, b = _c;
	} else if ( h >= 240 && h < 300 ) {
		r = _x, g = 0, b = _c;
	} else {
		r = _c, g = 0, b = _x;
	}

	auto _r = static_cast< int >( ( r + m ) * 255 );
	auto _g = static_cast< int >( ( g + m ) * 255 );
	auto _b = static_cast< int >( ( b + m ) * 255 );

	return { _r, _g, _b, a };
}
