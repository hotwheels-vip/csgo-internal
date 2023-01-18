#include "c_base_entity.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../../utilities/render/render.h"

#include "../../memory/memory.h"
#include "../enumerations/e_flags.h"

#include <array>

bool c_base_entity::get_bounding_box( bounding_box_t* bbox )
{
	const auto collideable = this->collideable( );
	if ( !collideable )
		return false;

	/* https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/collisionproperty.h#L77 */

	const c_vector mins = collideable->obb_mins( );
	const c_vector ma   = collideable->obb_max( );

	std::array< c_vector, 8U > points = { c_vector( mins.m_x, mins.m_y, mins.m_z ), c_vector( mins.m_x, ma.m_y, mins.m_z ),
		                                  c_vector( ma.m_x, ma.m_y, mins.m_z ),     c_vector( ma.m_x, mins.m_y, mins.m_z ),
		                                  c_vector( ma.m_x, ma.m_y, ma.m_z ),       c_vector( mins.m_x, ma.m_y, ma.m_z ),
		                                  c_vector( mins.m_x, mins.m_y, ma.m_z ),   c_vector( ma.m_x, mins.m_y, ma.m_z ) };

	if ( !( points.data( ) ) )
		return false;

	const matrix3x4_t& transformed_matrix = this->coordinate_frame( );

	float left   = std::numeric_limits< float >::max( );
	float top    = std::numeric_limits< float >::max( );
	float right  = std::numeric_limits< float >::lowest( );
	float bottom = std::numeric_limits< float >::lowest( );

	std::array< c_vector_2d, 8U > screen_points = { };
	for ( std::size_t i = 0U; i < 8U; i++ ) {
		if ( !render.world_to_screen( mathematics.vector_transform( points[ i ], transformed_matrix ), screen_points[ i ] ) )
			return false;

		left   = std::min( left, screen_points[ i ].m_x );
		top    = std::min( top, screen_points[ i ].m_y );
		right  = std::max( right, screen_points[ i ].m_x );
		bottom = std::max( bottom, screen_points[ i ].m_y );
	}

	bbox->m_left   = static_cast< int >( left );
	bbox->m_top    = static_cast< int >( top );
	bbox->m_right  = static_cast< int >( right );
	bbox->m_bottom = static_cast< int >( bottom );
	bbox->m_width  = static_cast< int >( right ) - static_cast< int >( left );
	bbox->m_height = static_cast< int >( bottom ) - static_cast< int >( top );
	return true;
}

c_user_cmd& c_base_entity::last_command( )
{
	static const std::uintptr_t last_command_offset =
		*reinterpret_cast< std::uintptr_t* >( memory.m_modules[ e_module_names::client ].find_pattern( ( "8D 8E ? ? ? ? 89 5C 24 3C" ) ) + 0x2 );
	return *reinterpret_cast< c_user_cmd* >( reinterpret_cast< std::uintptr_t >( this ) + last_command_offset );
}

bool c_base_entity::physics_run_think( int think_method )
{
	static auto original_physics_run_think = reinterpret_cast< bool( __thiscall* )( void*, int ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) ) );
	return original_physics_run_think( this, think_method );
}

void c_base_entity::set_next_think( int think )
{
	static auto original_set_next_think = reinterpret_cast< void( __thiscall* )( void*, int ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) ) );

	original_set_next_think( this, think );
}

void c_base_entity::set_abs_origin( const c_vector& origin )
{
	static auto original_set_abs_origin = reinterpret_cast< void( __thiscall* )( void*, const c_vector& ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) ) );
	original_set_abs_origin( this, origin );
}

int c_base_entity::lookup_bone( const char* bone )
{
	static auto lookup_bone_fn = reinterpret_cast< int( __thiscall* )( void*, const char* ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( "55 8B EC 53 56 8B F1 57 83 BE ? ? ? ? ? 75" ) );

	return lookup_bone_fn( this, bone );
}

void c_base_entity::get_bone_position( const int bone, c_vector& origin )
{
	static auto get_bone_position_fn = reinterpret_cast< void( __thiscall* )( void*, int, c_vector* ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( "55 8B EC 83 E4 F8 56 8B F1 57 83" ) );

	c_vector return_value[ 4 ];
	get_bone_position_fn( this, bone, return_value );
	origin = { return_value[ 1 ][ 0 ], return_value[ 2 ][ 1 ], return_value[ 3 ][ 2 ] };
}

void c_base_entity::post_think( )
{
	static auto post_think_vphysics = reinterpret_cast< bool( __thiscall* )( c_base_entity* ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( ( ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) ) ) );

	static auto simulate_player_simulated_entities = reinterpret_cast< void( __thiscall* )( c_base_entity* ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( ( ( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ) ) ) );

	interfaces.m_mdl_cache->begin_lock( );

	if ( this->is_alive( ) ) {
		this->update_collision_bounds( );

		if ( this->flags( ) & fl_onground )
			*this->fall_velocity( ) = 0.f;

		if ( this->sequence( ) == -1 )
			this->set_sequence( 0 );

		this->studio_frame_advance( );
		post_think_vphysics( this );
	}

	simulate_player_simulated_entities( this );

	interfaces.m_mdl_cache->end_lock( );
}
