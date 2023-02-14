#pragma once
#include "players.h"
#include "../../../game/sdk/includes/includes.h"
#include "../../../globals/includes/includes.h"
#include "../../entity_cache/entity_cache.h"

void n_players::impl_t::on_paint_traverse( )
{
	if ( GET_VARIABLE( g_variables.m_players, bool ) )
		this->players( );
}

void n_players::impl_t::players( )
{
	g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
		if ( !entity || entity->is_dormant( ) || !entity->is_alive( ) || entity == g_ctx.m_local || !g_ctx.m_local->is_enemy( entity ) ||
		     entity->is_dormant( ) )
			return;

		bounding_box_t box{ };
		if ( !entity->get_bounding_box( &box ) )
			return;

		const int index = entity->get_index( );

		player_info_t player_info{ };
		if ( !g_interfaces.m_engine_client->get_player_info( index, &player_info ) )
			return;

		if ( GET_VARIABLE( g_variables.m_players_box, bool ) )
			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_rect,
				std::make_any< rect_draw_object_t >( c_vector_2d( box.m_left, box.m_top ), c_vector_2d( box.m_right, box.m_bottom ),
			                                         GET_VARIABLE( g_variables.m_players_box_color, c_color ).get_u32( ),
			                                         GET_VARIABLE( g_variables.m_players_box_outline_color, c_color ).get_u32( ), false, 0.f,
			                                         ImDrawFlags_::ImDrawFlags_None, 1.f,
			                                         e_rect_flags::rect_flag_inner_outline | e_rect_flags::rect_flag_outer_outline ) );

		if ( GET_VARIABLE( g_variables.m_players_name, bool ) ) {
			std::string converted_name = player_info.m_name;
			if ( converted_name.length( ) > 24U )
				converted_name = converted_name.substr( 0U, 24U ).append( "..." );

			if ( player_info.m_fake_player )
				converted_name.insert( 0, "[bot] " );

			const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, converted_name.c_str( ) );

			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_text,
				std::make_any< text_draw_object_t >( g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
			                                         c_vector_2d( box.m_left + box.m_width * 0.5f - text_size.x * 0.5f, box.m_top - 3 - text_size.y ),
			                                         converted_name, GET_VARIABLE( g_variables.m_players_name_color, c_color ).get_u32( ),
			                                         c_color( 0.f, 0.f, 0.f, 1.f ).get_u32( ), e_text_flags::text_flag_dropshadow ) );
		}
	} );
}
