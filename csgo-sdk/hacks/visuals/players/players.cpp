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

		const auto client_renderable = entity->get_client_renderable( );
		if ( !client_renderable )
			return;

		const auto client_unknown = client_renderable->get_client_unknown( );
		if ( !client_unknown )
			return;

		const auto client_networkable = client_unknown->get_client_networkable( );
		if ( !client_networkable )
			return;

		bounding_box_t box{ };
		if ( !entity->get_bounding_box( &box ) )
			return;

		const int index = entity->get_index( );

		player_info_t player_info{ };
		if ( !g_interfaces.m_engine_client->get_player_info( index, &player_info ) )
			return;

		if ( GET_VARIABLE( g_variables.m_players_box, bool ) ) {
			if ( !GET_VARIABLE( g_variables.m_players_box_corner, bool ) )
				g_render.m_draw_data.emplace_back(
					e_draw_type::draw_type_rect,
					std::make_any< rect_draw_object_t >( c_vector_2d( box.m_left, box.m_top ), c_vector_2d( box.m_right, box.m_bottom ),
				                                         GET_VARIABLE( g_variables.m_players_box_color, c_color ).get_u32( ),
				                                         GET_VARIABLE( g_variables.m_players_box_outline_color, c_color ).get_u32( ), false, 0.f,
				                                         ImDrawFlags_::ImDrawFlags_None, 1.f,
				                                         GET_VARIABLE( g_variables.m_players_box_outline, bool )
				                                             ? e_rect_flags::rect_flag_inner_outline | e_rect_flags::rect_flag_outer_outline
				                                             : e_rect_flags::rect_flag_none ) );
			else {
				if ( GET_VARIABLE( g_variables.m_players_box_outline, bool ) ) {
					g_render.corner_rect( box.m_left, box.m_top, box.m_right, box.m_bottom,
					                      c_color( GET_VARIABLE( g_variables.m_players_box_outline_color, c_color ) ).get_u32( ), 2.f );
				}

				g_render.corner_rect( box.m_left, box.m_top, box.m_right, box.m_bottom,
				                      c_color( GET_VARIABLE( g_variables.m_players_box_color, c_color ) ).get_u32( ) );
			}
		}

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

		if ( GET_VARIABLE( g_variables.m_players_skeleton, bool ) ) {
			std::array< matrix3x4_t, 128 > matricies = { };
			memcpy( matricies.data( ), entity->get_cached_bone_data( ).get_elements( ),
			        entity->get_cached_bone_data( ).count( ) * sizeof( matrix3x4_t ) );

			if ( matricies.data( ) ) {
				const auto model = client_renderable->get_model( );
				if ( model ) {
					const auto studio_model = g_interfaces.m_model_info->get_studio_model( model );

					if ( studio_model ) {
						c_vector child_position = { }, parent_position = { };
						c_vector_2d child_screen_position = { }, parent_screen_position = { };

						c_vector upper_direction = c_vector( matricies[ 7 ][ 0 ][ 3 ], matricies[ 7 ][ 1 ][ 3 ], matricies[ 7 ][ 2 ][ 3 ] ) -
						                           c_vector( matricies[ 6 ][ 0 ][ 3 ], matricies[ 6 ][ 1 ][ 3 ], matricies[ 6 ][ 2 ][ 3 ] );
						c_vector breast_bone =
							c_vector( matricies[ 6 ][ 0 ][ 3 ], matricies[ 6 ][ 1 ][ 3 ], matricies[ 6 ][ 2 ][ 3 ] ) + upper_direction * 0.5f;

						for ( int i = 0; i < studio_model->n_bones; i++ ) {
							mstudiobone_t* bone = studio_model->get_bone( i );
							if ( !bone )
								continue;

							if ( bone->m_parent == -1 )
								continue;

							if ( !( bone->m_flags & 0x00000100 /* BONE_USED_BY_HITBOX */ ) )
								continue;

							child_position  = c_vector( matricies[ i ][ 0 ][ 3 ], matricies[ i ][ 1 ][ 3 ], matricies[ i ][ 2 ][ 3 ] );
							parent_position = c_vector( matricies[ bone->m_parent ][ 0 ][ 3 ], matricies[ bone->m_parent ][ 1 ][ 3 ],
							                            matricies[ bone->m_parent ][ 2 ][ 3 ] );

							c_vector delta_child  = child_position - breast_bone;
							c_vector delta_parent = parent_position - breast_bone;

							if ( delta_parent.length( ) < 9.0f && delta_child.length( ) < 9.0f )
								parent_position = breast_bone;

							if ( i == 5 )
								child_position = breast_bone;

							if ( fabs( delta_child.m_z ) < 5.0f && delta_parent.length( ) < 5.0f && delta_child.length( ) < 5.0f || i == 6 )
								continue;

							if ( g_render.world_to_screen( child_position, child_screen_position ) &&
							     g_render.world_to_screen( parent_position, parent_screen_position ) ) {
								g_render.m_draw_data.emplace_back(
									e_draw_type::draw_type_line,
									std::make_any< line_draw_object_t >( child_screen_position, parent_screen_position,
								                                         GET_VARIABLE( g_variables.m_players_skeleton_color, c_color ).get_u32( ),
								                                         1.f ) );
							}
						}
					}
				}
			}
		}
	} );
}
