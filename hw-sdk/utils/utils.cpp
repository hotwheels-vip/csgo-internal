#include "utils.h"

#include "../dependencies/hash/hash.h"
#include "../dependencies/lazy_importer/lazy_importer.h"
#include "../dependencies/xor/xor.h"
#include "../globals/macros/macros.h"
#include "helpers/primitive_definitions/primitives.h"

#include "../globals/interfaces/interfaces.h"

#include "../globals/ctx/ctx.h"

void utils::sleep( std::uint32_t time )
{
	::std::this_thread::sleep_for( ::chr::milliseconds( time ) );
}

BOOL utils::cheat_create_thread( DWORD WINAPI function( void* ), void* param )
{
	HANDLE handle = LI_FN( CreateThread )( nullptr, 0, function, param, 0, nullptr );
	if ( !handle )
		return FALSE;

	LI_FN( CloseHandle )( handle );

	return TRUE;
}

math::vec2< int > utils::world_to_screen( math::vec3 position )
{
	auto matrix = g_ctx.view_matrix;
	float width = matrix[ 3 ][ 0 ] * position.x + matrix[ 3 ][ 1 ] * position.y + matrix[ 3 ][ 2 ] * position.z + matrix[ 3 ][ 3 ];

	if ( width < 0.001f )
		return math::vec2< int >( -1, -1 );

	float inverse = 1.f / width;

	math::vec2< float > screen;

	screen.x = ( matrix[ 0 ][ 0 ] * position.x + matrix[ 0 ][ 1 ] * position.y + matrix[ 0 ][ 2 ] * position.z + matrix[ 0 ][ 3 ] ) * inverse;
	screen.y = ( matrix[ 1 ][ 0 ] * position.x + matrix[ 1 ][ 1 ] * position.y + matrix[ 1 ][ 2 ] * position.z + matrix[ 1 ][ 3 ] ) * inverse;

	auto screen_size = g_ctx.screen_size;

	screen.x = ( screen_size.x * 0.5f ) + ( screen.x * screen_size.x ) * 0.5f;
	screen.y = ( screen_size.y * 0.5f ) - ( screen.y * screen_size.y ) * 0.5f;

	return math::vec2< int >( static_cast< int >( screen.x ), static_cast< int >( screen.y ) );
}

math::vec2< int > utils::world_to_screen( math::vec3 position, bool& on_screen )
{
	on_screen = true;

	const auto matrix = g_ctx.view_matrix;
	// not really needed but prevents useless math to be done
	if ( !matrix.data ) {
		on_screen = false;
		return { };
	}

	float width = matrix[ 3 ][ 0 ] * position.x + matrix[ 3 ][ 1 ] * position.y + matrix[ 3 ][ 2 ] * position.z + matrix[ 3 ][ 3 ];

	if ( width < 0.001f ) {
		on_screen = false;

		return math::vec2< int >( -1, -1 );
	}

	float inverse = 1.f / width;

	math::vec2< float > screen;

	screen.x = ( matrix[ 0 ][ 0 ] * position.x + matrix[ 0 ][ 1 ] * position.y + matrix[ 0 ][ 2 ] * position.z + matrix[ 0 ][ 3 ] ) * inverse;
	screen.y = ( matrix[ 1 ][ 0 ] * position.x + matrix[ 1 ][ 1 ] * position.y + matrix[ 1 ][ 2 ] * position.z + matrix[ 1 ][ 3 ] ) * inverse;

	auto screen_size = g_ctx.screen_size;

	screen.x = ( screen_size.x * 0.5f ) + ( screen.x * screen_size.x ) * 0.5f;
	screen.y = ( screen_size.y * 0.5f ) - ( screen.y * screen_size.y ) * 0.5f;

	return math::vec2< int >( static_cast< int >( screen.x ), static_cast< int >( screen.y ) );
}

std::string utils::str::wide_to_multi_byte( std::wstring_view str )
{
	auto length = LI_FN( WideCharToMultiByte )( CP_UTF8, 0, str.data( ), str.size( ), nullptr, 0, nullptr, nullptr );

	auto result = std::string( length, 0 );

	LI_FN( WideCharToMultiByte )
	( CP_UTF8, 0, str.data( ), str.size( ), &result[ 0 ], length, nullptr, nullptr );

	return result;
}

std::wstring utils::str::multi_byte_to_wide( std::string_view str )
{
	auto length = LI_FN( MultiByteToWideChar )( CP_UTF8, 0, str.data( ), str.size( ), nullptr, 0 );

	auto result = std::wstring( length, 0 );

	LI_FN( MultiByteToWideChar )( CP_UTF8, 0, str.data( ), str.size( ), &result[ 0 ], length );

	return result;
}
