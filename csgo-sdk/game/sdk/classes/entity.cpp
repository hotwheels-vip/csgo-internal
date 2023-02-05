#include "entity.h"
#include "../../../globals/includes/includes.h"
#include "../includes/includes.h"

#include <array>

void c_client_entity::set_abs_origin( const c_vector& origin )
{
	static auto set_abs_origin_fn = reinterpret_cast< void( __thiscall* )( void*, const c_vector& ) >(
		g_modules[ HASH_CT( "client.dll" ) ].find_pattern( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) );

	set_abs_origin_fn( this, origin );
}

void c_client_entity::set_abs_angles( const c_angle& angle )
{
	static auto set_abs_angles_fn = reinterpret_cast< void( __thiscall* )( void*, const c_angle& ) >(
		g_modules[ HASH_CT( "client.dll" ) ].find_pattern( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) );

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

	if ( !animation_state->m_hit_ground_animation && animation_state->m_duck_amount == 0.f && ground_entity != nullptr )
		return;

	const auto head_position = entity->get_bone_position( entity->get_bone_by_hash( HASH_CT( "head_0" ) ) );
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
	static auto set_next_think_fn = reinterpret_cast< void( __thiscall* )( void*, int ) >(
		g_modules[ HASH_CT( "client.dll" ) ].find_pattern( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) );

	set_next_think_fn( this, think );
}

void c_base_entity::post_think( )
{
	static auto post_think_vphysics = reinterpret_cast< bool( __thiscall* )( c_base_entity* ) >(
		g_modules[ HASH_CT( "client.dll" ) ]. find_pattern( ( ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) ) ) );

	static auto simulate_player_simulated_entities = reinterpret_cast< void( __thiscall* )( c_base_entity* ) >(
		g_modules[ HASH_CT( "client.dll" ) ].find_pattern( ( ( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ) ) ) );

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
	static auto physics_run_think_fn = reinterpret_cast< bool( __thiscall* )( void*, int ) >(
		g_modules[ HASH_CT( "client.dll" ) ].find_pattern( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) );

	return physics_run_think_fn( this, think_method );
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

c_vector c_base_entity::get_bone_position( int bone )
{
	std::array< matrix3x4_t, 128 > out = { };

	if ( this->setup_bones( out.data( ), out.size( ), 0x0007ff00, 0.f ) )
		return out[ bone ][ 3 ];

	return c_vector( );
}

c_user_cmd& c_base_entity::get_last_command( )
{
	static const unsigned int get_last_command_fn =
		*reinterpret_cast< unsigned int* >( g_modules[ HASH_CT( "client.dll" ) ].find_pattern( "8D 8E ? ? ? ? 89 5C 24 3C" ) + 0x2 );

	return *reinterpret_cast< c_user_cmd* >( reinterpret_cast< unsigned int >( this ) + get_last_command_fn );
}