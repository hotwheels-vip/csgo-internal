#include "aimbot.h"

#include "../../../game/sdk/enums/flow.h"

#include "../../../globals/ctx/ctx.h"

#include "../../../utils/convars/convars.h"
#include "../../../utils/entity_list/entity_list.h"

#include "../../../utils/particle_system/particle_system.h"
#include "../../logging/logging.h"
#include "../lagcomp/lagcomp.h"

/* todo - coffin - add autowall (look at qo0, or reverse it since it isnt perfect)*/

sdk::c_cs_player* aimbot::impl::find_closest( )
{
	float closest_fov                = 180.f;
	sdk::c_cs_player* closest_player = nullptr;

	for ( auto& player_info : g_entity_list.players ) {
		auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

		if ( !player_info.m_valid || !player )
			continue;

		if ( auto player_fov = math::get_fov( g_ctx.cmd->view_angles, g_ctx.local->eye_position( ), player->eye_position( ) );
		     player_fov < closest_fov ) {
			closest_fov    = player_fov;
			closest_player = player;
		}
	}

	return closest_player;
}

float aimbot::impl::hitchance( sdk::c_cs_player* player, math::vec3 angles, sdk::hitgroup hit_group )
{
	math::vec3 start = g_ctx.local->eye_position( ), end{ }, forward{ }, right{ }, up{ }, direction{ }, weapon_spread{ };

	float innaccuracy = g_ctx.weapon->inaccuracy( ), spread = g_ctx.weapon->spread( );

	int total_hits{ };

	math::angle_vectors( angles, &forward, &right, &up );

	auto weapon_data = g_ctx.weapon->get_weapon_data( );

	for ( int iterator = 0; iterator < 256; iterator++ ) {
		weapon_spread = g_ctx.weapon->spread_vector( iterator, innaccuracy, spread );
		direction     = ( forward + ( right * weapon_spread.x ) + ( up * weapon_spread.y ) ).normalized( );
		end           = start + ( direction * weapon_data->range );

		if ( g_ctx.local->can_see_player_ignore_walls( player, hit_group, start, end ) )
			total_hits++;
	}

	return ( total_hits / 255.f ) * 100.f;
}

void aimbot::impl::run( )
{
	if ( !g_ctx.cmd->buttons.has( sdk::buttons::IN_ATTACK ) )
		return;

	if ( !g_ctx.local || !g_ctx.weapon )
		return;

	auto entity = find_closest( );

	if ( !entity )
		return;

	if ( !g_ctx.local->can_see_player( entity ) )
		return;

	g_lagcomp.backtrack_player( entity );

	math::vec3 forward_to_head = g_ctx.local->eye_position( ) - entity->hitbox_position( sdk::HITGROUP_HEAD );
	math::vec3 angles_to_head;

	math::vector_angles( forward_to_head, angles_to_head );

	float head_hitchance = hitchance( entity, angles_to_head, sdk::HITGROUP_HEAD );

	if ( g_ctx.record ) {
		forward_to_head = g_ctx.local->eye_position( ) - entity->hitbox_position( sdk::HITGROUP_HEAD, g_ctx.record->bone_matrix );
		math::vector_angles( forward_to_head, angles_to_head );
	}

	static auto recoil_scale = g_convars[ _( "weapon_recoil_scale" ) ];
	auto recoil_angle        = g_ctx.local->aim_punch_angle( ) * ( recoil_scale->get_float( ) * -1.f );

	angles_to_head += recoil_angle;

	angles_to_head = angles_to_head.normalize( );

	if ( g_ctx.weapon->can_shoot_primary( ) ) {
		if ( g_ctx.record ) {
			auto out = console::format( _( "hit player {} [ tick: {} | health: {} | chance: {} | fall: {} ]" ), entity->name( ),
			                            sdk::time_to_ticks( g_ctx.record->simulation_time + g_lagcomp.lerp_time( ) ), entity->health( ),
			                            ( int )head_hitchance, ( int )entity->velocity( ).z );
			g_log.print( out );
		}
		g_interfaces.engine->set_view_angles( angles_to_head );

		/* todo - coffin - do sendpacket stuff on shot etc for a more psilent effect */
		g_ctx.cmd->view_angles = angles_to_head;
	}
}
