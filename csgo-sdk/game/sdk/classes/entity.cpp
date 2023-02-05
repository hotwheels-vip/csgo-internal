#include "entity.h"
#include "../../../globals/includes/includes.h"
#include "../includes/includes.h"

void c_client_entity::set_abs_origin( const c_vector& origin )
{
	static auto set_abs_origin_fn = g_modules[ HASH_CT( "client.dll" ) ]
	                                    .find_pattern( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" )
	                                    .cast< void( __thiscall* )( void*, const c_vector& ) >( );

	set_abs_origin_fn( this, origin );
}

void c_client_entity::set_abs_angles( const c_angle& angle )
{
	static auto set_abs_angles_fn = g_modules[ HASH_CT( "client.dll" ) ]
	                                    .find_pattern( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" )
	                                    .cast< void( __thiscall* )( void*, const c_angle& ) >( );

	set_abs_angles_fn( this, angle );
}