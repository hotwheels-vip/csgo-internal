#include "lagcomp.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../../globals/logger/logger.h"
#include "../entity_cache/entity_cache.h"

/* TODO ~ don't use 3 entity loops for lagcomp */

float n_lagcomp::impl_t::lerp_time( )
{
	static auto *cl_updaterate = g_convars[ HASH_BT( "sv_maxupdaterate" ) ], *cl_interp_ratio = g_convars[ HASH_BT( "cl_interp_ratio" ) ],
				*cl_interp = g_convars[ HASH_BT( "cl_interp" ) ], *sv_client_min_interp_ratio = g_convars[ HASH_BT( "sv_client_min_interp_ratio" ) ],
				*sv_client_max_interp_ratio = g_convars[ HASH_BT( "sv_client_max_interp_ratio" ) ];

	static auto interp_ratio = max( cl_interp_ratio->get_float( ), 1.f );

	interp_ratio = std::clamp( interp_ratio, sv_client_min_interp_ratio->get_float( ), sv_client_max_interp_ratio->get_float( ) );

	return max( cl_interp->get_float( ), interp_ratio / cl_updaterate->get_int( ) );
}

bool n_lagcomp::impl_t::is_valid( n_lagcomp::impl_t::record_t rec )
{
	const auto net_channel = g_interfaces.m_engine_client->get_net_channel_info( );
	if ( !net_channel )
		return false;

	float correct = 0.f;

	correct += lerp_time( );
	correct += net_channel->get_latency( FLOW_OUTGOING );
	correct += net_channel->get_latency( FLOW_INCOMING );

	correct = std::clamp( correct, 0.f, g_convars[ HASH_BT( "sv_maxunlag" ) ]->get_float( ) );

	const float delta_correct = std::fabsf( correct - ( g_math.ticks_to_time( g_ctx.m_local->get_tick_base( ) ) - rec.m_sim_time ) );

	return delta_correct <= 0.2f;
}

// creds dex
bool n_lagcomp::impl_t::generate_lerped_lag_matrix( const int ent_index, matrix3x4_t* out )
{
	auto entity = g_interfaces.m_client_entity_list->get< c_base_entity >( ent_index );
	if ( !entity )
		return false;

	if ( !entity->is_valid_enemy( ) )
		return false;

	const auto& record_list = this->m_records[ ent_index ];
	if ( !record_list )
		return false;

	auto nci = g_interfaces.m_engine_client->get_net_channel_info( );
	if ( !nci )
		return false;

	auto unlag = g_convars[ HASH_BT( "sv_maxunlag" ) ]->get_float( );

	for ( int i = 0; i < g_ctx.m_max_allocations; i++ ) {
		auto record = &record_list[ i ];
		if ( !record )
			continue;

		bool end = i + 1 == g_ctx.m_max_allocations;

		auto next_record = &record_list[ i + 1 ];

		if ( record && record->m_valid && ( !end && next_record ) ) {
			if ( record->m_vec_origin.dist_to( entity->get_abs_origin( ) ) < 1.f )
				return false;

			c_vector next   = end ? entity->get_abs_origin( ) : next_record->m_vec_origin;
			float time_next = end ? entity->get_simulation_time( ) : next_record->m_sim_time;

			float total_latency = nci->get_avg_latency( FLOW_OUTGOING ) + nci->get_avg_latency( FLOW_INCOMING );
			total_latency       = std::clamp( total_latency, 0.f, unlag );

			float correct    = total_latency + lerp_time( );
			float time_delta = time_next - record->m_sim_time;
			float add        = end ? 1.f : time_delta;
			float deadtime   = record->m_sim_time + correct + add;

			float curtime = g_interfaces.m_global_vars_base->m_current_time;
			float delta   = deadtime - curtime;

			float mul     = 1.f / add;
			c_vector lerp = g_math.interpolate_vector( next, record->m_vec_origin, std::clamp( delta * mul, 0.f, 1.f ) );

			matrix3x4_t ret[ 128 ];

			std::memcpy( ret, record->m_matrix, sizeof( ret ) );

			for ( size_t i{ }; i < 128; ++i ) {
				c_vector matrix_delta = record->m_matrix[ i ].get_origin( ) - record->m_vec_origin;
				ret[ i ].set_origin( matrix_delta + lerp );
			}

			std::memcpy( out, ret, sizeof( ret ) );

			return true;
		}
	}
	return false;
}

void n_lagcomp::impl_t::on_frame_stage_notify( )
{
	g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
		if ( !entity->is_valid_enemy( ) )
			return;

		const auto index = entity->get_index( );

		const auto& record_list = this->m_records[ index ];
		if ( !record_list )
			return;

		for ( int j = 0; j < g_ctx.m_max_allocations; j++ ) {
			auto& record = record_list[ j ];

			if ( record.m_player != index && record.m_player != -1 ) {
				delete[] this->m_records[ index ];

				this->m_records[ index ] = nullptr;

				break;
			}

			record.m_valid = is_valid( record );
		}
	} );

	g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
		if ( !entity )
			return;

		const auto index = entity->get_index( );

		if ( !entity->is_alive( ) || entity->is_dormant( ) || !g_ctx.m_local->is_enemy( entity ) || entity == g_ctx.m_local ) {
			if ( this->m_records[ index ] ) {
				delete[] this->m_records[ index ];

				this->m_records[ index ] = nullptr;
			}

			return;
		}

		auto record    = this->m_records[ index ];
		auto& location = this->m_record_location[ index ];

		if ( !record ) {
			this->m_records[ index ] = new record_t[ g_ctx.m_max_allocations ];

			location = 0;
			record   = this->m_records[ index ];
		}

		if ( location >= g_ctx.m_max_allocations )
			location = 0;

		record_t new_record{ };

		new_record.m_player     = index;
		new_record.m_sim_time   = entity->get_simulation_time( );
		new_record.m_valid      = is_valid( new_record );
		new_record.m_vec_origin = entity->get_abs_origin( );

		if ( !entity->setup_bones( new_record.m_matrix, 128, 0x00000100 /*BONE_USED_BY_HITBOX*/, g_interfaces.m_global_vars_base->m_current_time ) )
			return;

		memcpy( &record[ location ], &new_record, sizeof( record_t ) );

		location++;
	} );
}

void n_lagcomp::impl_t::backtrack_player( record_t* heap_record )
{
	g_ctx.m_record = heap_record;

	if ( !heap_record )
		return;

	g_ctx.m_cmd->m_tick_count = g_math.time_to_ticks( heap_record->m_sim_time + lerp_time( ) );
}

void n_lagcomp::impl_t::backtrack_player( c_base_entity* player )
{
	auto index = player->get_index( );

	auto current_fov         = 180.f;
	record_t* closest_record = nullptr;

	const auto eye_position = g_ctx.m_local->get_eye_position( false );

	c_angle player_angles{ };
	g_interfaces.m_engine_client->get_view_angles( player_angles );

	if ( !m_records[ index ] )
		return;

	if ( const auto record_list = g_lagcomp.m_records[ index ] ) {
		for ( int i = 0; i < g_ctx.m_max_allocations; i++ ) {
			auto record = &record_list[ i ];

			if ( !record_list[ i ].m_valid )
				continue;

			const auto angle = g_math.calculate_angle( eye_position, player->get_hitbox_position( hitbox_head, record_list[ i ].m_matrix ) );

			if ( float calculated_fov = g_math.calculate_fov( g_ctx.m_cmd->m_view_point, angle ); calculated_fov < current_fov ) {
				current_fov    = calculated_fov;
				closest_record = record;
			}
		}
	}

	g_ctx.m_record = closest_record;

	if ( !closest_record )
		return;

	// https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/game/server/player_lagcompensation.cpp#L287

	g_ctx.m_cmd->m_tick_count = g_math.time_to_ticks( closest_record->m_sim_time + lerp_time( ) );
}

std::optional< n_lagcomp::impl_t::record_t > n_lagcomp::impl_t::oldest_record( const int index )
{
	if ( !g_lagcomp.m_records[ index ] )
		return std::nullopt;

	int last_valid_heap_record   = 0;
	float lowest_simulation_time = FLT_MAX;

	for ( int i = 0; i < g_ctx.m_max_allocations; i++ ) {
		auto current_record = &g_lagcomp.m_records[ index ][ i ];

		if ( !current_record || !current_record->m_valid || current_record->m_sim_time > lowest_simulation_time )
			continue;

		last_valid_heap_record = i;
		lowest_simulation_time = current_record->m_sim_time;
	}

	return std::make_optional( g_lagcomp.m_records[ index ][ last_valid_heap_record ] );
}

// this code should only be ran if aimbot is off, we should never set tickcount without calculating
// the position of the registered lag compensated tick
void n_lagcomp::impl_t::on_create_move_post( )
{
	if ( GET_VARIABLE( g_variables.m_aimbot_enable, bool ) || !GET_VARIABLE( g_variables.m_backtrack_enable, bool ) )
		return;

	float current_fov = 180.f;

	const auto eye_position     = g_ctx.m_local->get_eye_position( false );
	int best_tick               = g_ctx.m_cmd->m_tick_count;
	static int best_logged_tick = 0;

	g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
		if ( !entity || !entity->is_alive( ) || entity->is_dormant( ) || !g_ctx.m_local->is_enemy( entity ) || entity == g_ctx.m_local )
			return;

		const auto index = entity->get_index( );

		if ( const auto record_list = g_lagcomp.m_records[ index ] ) {
			for ( int i = 0; i < g_ctx.m_max_allocations; i++ ) {
				auto record = record_list[ i ];

				if ( !record.m_valid )
					continue;

				for ( int j = 0; j < 19; j++ ) {
					const auto angle = g_math.calculate_angle( eye_position, entity->get_hitbox_position( j, record.m_matrix ) );

					if ( float calculated_fov = g_math.calculate_fov( g_ctx.m_cmd->m_view_point, angle ); calculated_fov < current_fov ) {
						current_fov = calculated_fov;

						best_tick        = g_math.time_to_ticks( record.m_sim_time + lerp_time( ) );
						best_logged_tick = i;
					}
				}
			}
		}
	} );

	if ( !( g_ctx.m_cmd->m_buttons & in_attack ) )
		return;

	g_ctx.m_cmd->m_tick_count = best_tick;
}
