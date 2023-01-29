#include "c_base_entity.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/mathematics/mathematics.h"
#include "../../utilities/render/render.h"

#include "../../memory/memory.h"
#include "../enumerations/e_flags.h"
#include "../enumerations/e_item_definition_index.h"
#include "../structs/studio.h"

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

// bool c_base_entity::can_shoot( )
//{
//	const float server_time = static_cast< int >( 0.5f + static_cast< float >( this->tick_base( ) ) / memory.m_globals->m_interval_per_tick );
//	if ( this->ammo( ) <= 0 )
//		return false;
//
//	if ( this->get_next_attack( ) > server_time )
//		return false;
//
//	const short definition_index = this->item_definition_index( );
//	if ( ( definition_index == e_item_definition_index::weapon_famas || definition_index == e_item_definition_index::weapon_glock ) &&
//	     this->is_burst_mode( ) && this->burst_shots_remaining( ) > 0 )
//		return true;
//
//	if ( this->next_primary_attack( ) > server_time )
//		return false;
//
//	if ( definition_index == e_item_definition_index::weapon_revolver && this->fire_ready_time( ) > server_time )
//		return false;
//
//	return true;
// }

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

bool c_base_entity::has_bomb( )
{
	static auto has_bomb =
		reinterpret_cast< bool( __thiscall* )( void* ) >( memory.m_modules[ e_module_names::client ].find_pattern( ( "56 8B F1 85 F6 74 31" ) ) );

	return has_bomb( this );
}

c_player_animation_layer* c_base_entity::get_anim_layers( )
{
	return *reinterpret_cast< c_player_animation_layer** >( reinterpret_cast< DWORD >( this ) + 0x2990 );
}

c_player_animation_layer* c_base_entity::get_anim_layer( const int i )
{
	if ( i < 15 )
		return &get_anim_layers( )[ i ];
	return nullptr;
}

void c_base_entity::get_animation_layers( c_player_animation_layer* layers )
{
	std::memcpy( layers, get_anim_layers( ), sizeof( c_player_animation_layer ) * 13 );
}

void c_base_entity::set_animation_layers( c_player_animation_layer* layers )
{
	std::memcpy( get_anim_layers( ), layers, sizeof( c_player_animation_layer ) * 13 );
}

int c_base_entity::get_sequence_activity( int sequence )
{
	const auto model = this->client_renderable( )->model( );
	if ( !model )
		return -1;

	const auto studio_hdr = interfaces.m_model_info->get_studio_model( model );
	if ( !studio_hdr )
		return -1;

	static auto get_sequence_activity = reinterpret_cast< int( __fastcall* )( void*, studiohdr_t*, int ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( ( "55 8B EC 53 8B 5D 08 56 8B F1 83" ) ) );

	return get_sequence_activity( this, studio_hdr, sequence );
}

bool c_base_entity::reloading( )
{
	c_player_animation_layer* layer = this->get_anim_layer( 1 );

	if ( !layer )
		return false;

	if ( !layer->owner )
		return false;

	if ( const auto activity = this->get_sequence_activity( layer->sequence ); activity == 967 && layer->weight != 0.f )
		return true;

	return false;
}

bool c_base_entity::is_visible( c_base_entity* entity, const c_vector& end_position )
{
	const c_vector start_position = this->eye_position( );

	const ray_t ray( start_position, end_position );
	c_trace_filter filter( this );

	trace_t trace = { };
	interfaces.m_engine_trace->trace_ray( ray, e_mask::mask_shot, &filter, &trace );

	if ( ( trace.is_visible( ) || trace.m_hit_entity == entity ) )
		return true;

	return false;
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

int c_base_entity::get_bone_by_hash( const unsigned int bone_hash )
{
	if ( const auto model = this->client_renderable( )->model( ); model != nullptr ) {
		if ( const auto studio_hdr = interfaces.m_model_info->get_studio_model( model ); studio_hdr ) {
			for ( int i = 0; i < studio_hdr->n_bones; i++ ) {
				if ( const auto bone = studio_hdr->get_bone( i ); bone && fnv1a::hash( bone->get_name( ) ) == bone_hash )
					return i;
			}
		}
	}
	return -1;
}

void c_base_entity::get_bone_position( const int bone, c_vector& origin )
{
	static auto get_bone_position_fn = reinterpret_cast< void( __thiscall* )( void*, int, c_vector* ) >(
		memory.m_modules[ e_module_names::client ].find_pattern( "55 8B EC 83 E4 F8 56 8B F1 57 83" ) );

	c_vector return_value[ 4 ]{ };
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
