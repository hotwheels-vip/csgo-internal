#include "dormancy.h"
#include "../players.h"

void n_dormancy::impl_t::start( )
{
	m_cur_sound_list.remove_all( );
	g_interfaces.m_engine_sound->get_active_sounds( m_cur_sound_list );

	if ( !m_cur_sound_list.count( ) )
		return;

	for ( int i = 0; i < m_cur_sound_list.count( ); i++ ) {
		auto& sound = m_cur_sound_list[ i ];

		if ( sound.m_sound_source <= 0 || sound.m_sound_source > 64 )
			continue;

		auto player = g_interfaces.m_client_entity_list->get< c_base_entity >( sound.m_sound_source );

		if ( !player || player == g_ctx.m_local || sound.m_origin->is_zero( ) )
			continue;

		if ( !is_valid_sound( sound ) )
			continue;

		setup_adjust_player( player, sound );

		m_sound_players[ sound.m_sound_source ].override_info( sound );
	}

	g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
		// only dormant players
		if ( !entity || !entity->is_dormant( ) || !entity->is_alive( ) )
			return;

		adjust_player_begin( entity );
	} );

	m_vec_sound_buffer = m_cur_sound_list;
}

void n_dormancy::impl_t::finish( )
{
	adjust_player_finish( );
}

void n_dormancy::impl_t::adjust_player_finish( )
{
	for ( auto& RestorePlayer : m_array_restore_players ) {
		auto player           = RestorePlayer.m_player;
		player->get_flags( )  = RestorePlayer.m_flags;
		// player->get_origin( ) = RestorePlayer.m_origin;
		player->set_abs_origin( RestorePlayer.m_abs_origin );
		player->is_dormant( ) = RestorePlayer.m_dormant; // dormant check
	}

	m_array_restore_players.clear( );
}

void n_dormancy::impl_t::adjust_player_begin( c_base_entity* player )
{
	auto& sound_player = m_sound_players[ player->get_index( ) ];
	bool sound_expired = GetTickCount( ) - sound_player.m_receive_time > EXPIRE_DURATION;
	if ( sound_expired && g_players.get_fading_alpha( player->get_index( ) ) == 0.f )
		return;

	if ( sound_player.m_abs_origin.is_zero( ) )
		return;

	entity_sound_t current_player;
	current_player.m_player     = player;
	current_player.m_dormant    = true;
	current_player.m_flags      = player->get_flags( );
	current_player.m_origin     = player->get_origin( );
	current_player.m_abs_origin = player->get_abs_origin( );
	m_array_restore_players.emplace_back( current_player );

	if ( !sound_expired )
		player->is_dormant( ) = false;
	player->get_flags( )  = sound_player.m_flags;
	// player->get_origin( ) = sound_player.m_origin;
	player->set_abs_origin( sound_player.m_abs_origin );
}

void n_dormancy::impl_t::setup_adjust_player( c_base_entity* player, sound_info_t& sound )
{
	c_vector src{ }, dst{ };
	trace_t trace{ };
	c_trace_filter filter( player );

	src = ( *sound.m_origin ) + c_vector( 0, 0, 1 );
	dst = src - c_vector( 0, 0, 100 );

	ray_t ray( src, dst );

	g_interfaces.m_engine_trace->trace_ray( ray, mask_playersolid, &filter, &trace );

	if ( trace.m_all_solid )
		m_sound_players[ sound.m_sound_source ].m_receive_time = -1;

	*sound.m_origin = ( ( trace.m_fraction < 0.97 ) ? trace.m_end : *sound.m_origin );

	m_sound_players[ sound.m_sound_source ].m_flags = player->get_flags( );
	m_sound_players[ sound.m_sound_source ].m_flags |= ( trace.m_fraction < 0.50f ? fl_ducking : 0 ) | ( trace.m_fraction != 1 ? fl_onground : 0 );
	m_sound_players[ sound.m_sound_source ].m_flags &= ( trace.m_fraction > 0.50f ? ~fl_ducking : 0 ) | ( trace.m_fraction == 1 ? ~fl_onground : 0 );
}

bool n_dormancy::impl_t::is_valid_sound( sound_info_t& sound )
{
	if ( !sound.m_from_server )
		return false;

	for ( int i = 0; i < m_vec_sound_buffer.count( ); i++ ) {
		sound_info_t& cached_sound = m_vec_sound_buffer[ i ];
		if ( cached_sound.m_uid == sound.m_uid )
			return false;
	}

	return true;
}
