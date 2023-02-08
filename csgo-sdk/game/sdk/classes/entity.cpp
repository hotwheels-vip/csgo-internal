#include "entity.h"
#include "../../../globals/includes/includes.h"
#include "../includes/includes.h"

#include <array>

void c_client_entity::set_abs_origin( const c_vector& origin )
{
	static auto set_abs_origin_fn = reinterpret_cast< void( __thiscall* )( void*, const c_vector& ) >(
		g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) );

	set_abs_origin_fn( this, origin );
}

void c_client_entity::set_abs_angles( const c_angle& angle )
{
	static auto set_abs_angles_fn = reinterpret_cast< void( __thiscall* )( void*, const c_angle& ) >(
		g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) );

	set_abs_angles_fn( this, angle );
}

void c_base_entity::modify_eye_position( const c_animation_state* animation_state, c_vector* position ) const
{
	if ( g_interfaces.m_engine_client->is_hltv( ) || g_interfaces.m_engine_client->is_playing_demo( ) )
		return;

	const auto entity = animation_state->m_entity;
	if ( !entity )
		return;

	const auto ground_entity = g_interfaces.m_client_entity_list->get< c_client_entity >( entity->get_ground_entity_handle( ) );

	if ( !animation_state->m_hit_ground_animation && animation_state->m_duck_amount == 0.f && ground_entity )
		return;

	const auto head_position = entity->get_bone_position( entity->get_bone_by_hash( HASH_BT( "head_0" ) ) );
	if ( head_position.is_zero( ) )
		return;

	c_vector modified_position = head_position;
	modified_position.m_z += 1.7f;

	if ( modified_position.m_z < position->m_z ) {
		float factor       = 0.f;
		const float delta  = std::fabsf( position->m_z - modified_position.m_z );
		const float offset = ( delta - 4.0f ) / 6.0f;

		if ( offset >= 0.f )
			factor = min( offset, 1.0f );

		const float factor_squared = ( factor * factor );
		position->m_z += ( ( modified_position.m_z - position->m_z ) * ( ( factor_squared * 3.0f ) - ( ( factor_squared * 2.0f ) * factor ) ) );
	}
}

void c_base_entity::set_next_think( int think )
{
	static auto set_next_think_fn =
		reinterpret_cast< void( __thiscall* )( void*, int ) >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) );

	set_next_think_fn( this, think );
}

void c_base_entity::post_think( )
{
	static auto post_think_vphysics = reinterpret_cast< bool( __thiscall* )( c_base_entity* ) >(
		g_modules[ CLIENT_DLL ].find_pattern( ( ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) ) ) );

	static auto simulate_player_simulated_entities = reinterpret_cast< void( __thiscall* )( c_base_entity* ) >(
		g_modules[ CLIENT_DLL ].find_pattern( ( ( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ) ) ) );

	g_interfaces.m_model_cache->begin_lock( );

	if ( this->is_alive( ) ) {
		this->update_collision_bounds( );

		if ( this->get_flags( ) & e_flags::fl_onground )
			*this->get_fall_velocity( ) = 0.f;

		if ( this->get_sequence( ) == -1 )
			this->set_sequence( 0 );

		this->studio_frame_advance( );
		post_think_vphysics( this );
	}

	simulate_player_simulated_entities( this );

	g_interfaces.m_model_cache->end_lock( );
}

bool c_base_entity::physics_run_think( int think_method )
{
	static auto physics_run_think_fn =
		reinterpret_cast< bool( __thiscall* )( void*, int ) >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) );

	return physics_run_think_fn( this, think_method );
}

bool c_base_entity::can_shoot( )
{
	const auto weapon = g_interfaces.m_client_entity_list->get< c_base_entity >( this->get_active_weapon_handle( ) );
	if ( !weapon )
		return false;

	const float server_time = static_cast< float >( this->get_tick_base( ) ) * g_interfaces.m_global_vars_base->m_interval_per_tick;

	if ( weapon->get_ammo( ) <= 0 )
		return false;

	if ( this->get_next_attack( ) > server_time )
		return false;

	const short definition_index = weapon->get_item_definition_index( );

	if ( ( definition_index == e_item_definition_index::weapon_famas || definition_index == e_item_definition_index::weapon_glock ) &&
	     weapon->is_burst_mode( ) && weapon->get_burst_shots_remaining( ) > 0 )
		return true;

	if ( weapon->get_next_primary_attack( ) > server_time )
		return false;

	if ( definition_index == e_item_definition_index::weapon_revolver && weapon->get_fire_ready_time( ) > server_time )
		return false;

	return true;
}

bool c_base_entity::is_enemy( c_base_entity* entity )
{
	if ( g_interfaces.m_game_types->get_current_game_type( ) == 6 /* GAMETYPE_FREEFORALL */ )
		return ( this->get_survival_team( ) != entity->get_survival_team( ) );

	static auto mp_teammates_are_enemies = g_convars[ HASH_BT( "mp_teammates_are_enemies" ) ];

	if ( mp_teammates_are_enemies && mp_teammates_are_enemies->get_bool( ) && this->get_team( ) == entity->get_team( ) && this != entity )
		return true;

	if ( this->get_team( ) != entity->get_team( ) )
		return true;

	return false;
}

int c_base_entity::get_bone_by_hash( const unsigned int hash ) const
{
	if ( const auto model = this->get_model( ); model ) {
		if ( const auto studio_model = g_interfaces.m_model_info->get_studio_model( model ); studio_model ) {
			for ( int i = 0; i < studio_model->n_bones; i++ ) {
				if ( const auto bone = studio_model->get_bone( i ); bone && HASH_RT( bone->get_name( ) ) == hash )
					return i;
			}
		}
	}

	return -1;
}

int c_base_entity::get_max_health( )
{
	if ( g_interfaces.m_game_types->get_current_game_type( ) == 6 /* GAMETYPE_FREEFORALL */ )
		return 120;

	return 100;
}

c_vector c_base_entity::get_bone_position( int bone )
{
	std::array< matrix3x4_t, 128 > out = { };

	if ( this->setup_bones( out.data( ), out.size( ), 0x0007ff00, 0.f ) )
		return out[ bone ][ 3 ];

	return c_vector( );
}

c_vector c_base_entity::get_eye_position( bool should_correct )
{
	c_vector out{ };

	g_virtual.call< void >( this, 169, std::ref( out ) );

	if ( this->is_used_new_animation_state( ) && should_correct ) {
		if ( const auto animation_state = this->get_animation_state( ); animation_state )
			this->modify_eye_position( animation_state, &out );
	}

	return out;
}

c_user_cmd& c_base_entity::get_last_command( )
{
	static const unsigned int get_last_command_fn =
		*reinterpret_cast< unsigned int* >( g_modules[ CLIENT_DLL ].find_pattern( "8D 8E ? ? ? ? 89 5C 24 3C" ) + 0x2 );

	return *reinterpret_cast< c_user_cmd* >( reinterpret_cast< unsigned int >( this ) + get_last_command_fn );
}
