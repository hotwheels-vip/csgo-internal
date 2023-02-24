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

		float padding[ e_padding_direction::padding_direction_max ] = { 0.f, 0.f, 0.f, 0.f };

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

		const auto active_weapon = reinterpret_cast< c_base_entity* >(
			g_interfaces.m_client_entity_list->get_client_entity_from_handle( entity->get_active_weapon_handle( ) ) );
		if ( active_weapon )
			this->m_backup_player_data[ index ].m_active_weapon = active_weapon;

		if ( this->m_backup_player_data[ index ].m_active_weapon ) {
			const auto item_definition_index = this->m_backup_player_data[ index ].m_active_weapon->get_item_definition_index( );
			const auto weapon_data           = g_interfaces.m_weapon_system->get_weapon_data( item_definition_index );

			if ( weapon_data )
				this->m_backup_player_data[ index ].m_weapon_data = weapon_data;

			if ( this->m_backup_player_data[ index ].m_weapon_data ) {
				[ & ]( ) {
					if ( GET_VARIABLE( g_variables.m_player_ammo_bar, bool ) ) {
						if ( !this->m_backup_player_data[ index ].m_weapon_data->is_gun( ) )
							return;

						const float ammo = static_cast< float >( this->m_backup_player_data[ index ].m_active_weapon->get_ammo( ) );

						if ( this->m_backup_player_data[ index ].m_ammo > ammo )
							this->m_backup_player_data[ index ].m_ammo -= ( 2.f * g_interfaces.m_global_vars_base->m_frame_time );
						else
							this->m_backup_player_data[ index ].m_ammo = ammo;

						const int max_ammo = this->m_backup_player_data[ index ].m_weapon_data->m_max_clip1;

						float factor = this->m_backup_player_data[ index ].m_ammo / static_cast< float >( max_ammo );
						factor       = std::clamp< float >( factor, 0.f, 1.f );

						g_render.m_draw_data.emplace_back(
							e_draw_type::draw_type_rect,
							std::make_any< rect_draw_object_t >( c_vector_2d( box.m_left, box.m_bottom + 3.f ),
						                                         c_vector_2d( box.m_left + box.m_width, box.m_bottom + 5.f ),
						                                         ImColor( 0.f, 0.f, 0.f, 1.f ), ImColor( 0.f, 0.f, 0.f, 1.f ), false, 0.f,
						                                         ImDrawFlags_::ImDrawFlags_None, 1.f, rect_flag_outer_outline ) );

						g_render.m_draw_data.emplace_back(
							e_draw_type::draw_type_rect,
							std::make_any< rect_draw_object_t >(
								c_vector_2d( box.m_left, box.m_bottom + 3.f ), c_vector_2d( box.m_left + box.m_width * factor, box.m_bottom + 5.f ),
								GET_VARIABLE( g_variables.m_player_ammo_bar_color, c_color ).get_u32( 1.f ), ImColor( 0.f, 0.f, 0.f, 1.f ), false,
								0.f, ImDrawFlags_::ImDrawFlags_None, 1.f, rect_flag_outer_outline ) );

						padding[ e_padding_direction::padding_direction_bottom ] += 6.f;
					};
				}( );

				[ & ]( ) {
					if ( GET_VARIABLE( g_variables.m_weapon_name, bool ) ) {
						const auto localized_name = g_interfaces.m_localize->find( this->m_backup_player_data[ index ].m_weapon_data->m_hud_name );

						std::wstring w = localized_name;
						if ( w.empty( ) )
							return;

						std::transform( w.begin( ), w.end( ), w.begin( ), ::towlower );

						const std::string converted_name( w.begin( ), w.end( ) );
						if ( converted_name.empty( ) )
							return;

						const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
							g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, converted_name.c_str( ) );

						g_render.m_draw_data.emplace_back(
							e_draw_type::draw_type_text,
							std::make_any< text_draw_object_t >(
								g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
								c_vector_2d( box.m_left + ( box.m_width - text_size.x ) / 2,
						                     box.m_bottom + 2 + padding[ e_padding_direction::padding_direction_bottom ] ),
								converted_name, GET_VARIABLE( g_variables.m_weapon_name_color, c_color ).get_u32( 1.f ),
								ImColor( 0.f, 0.f, 0.f, 1.f ), text_flag_dropshadow ) );

						padding[ e_padding_direction::padding_direction_bottom ] += ( text_size.y + 1.f );
					}
				}( );

				if ( GET_VARIABLE( g_variables.m_weapon_icon, bool ) ) {
					if ( !( item_definition_index == e_item_definition_index::weapon_shield ||
					        item_definition_index == e_item_definition_index::weapon_breachcharge ||
					        item_definition_index == e_item_definition_index::weapon_bumpmine ) ) {
						const auto text      = reinterpret_cast< const char* >( g_utilities.get_weapon_icon( item_definition_index ) );
						const auto text_size = g_render.m_fonts[ e_font_names::font_name_icon_12 ]->CalcTextSizeA(
							g_render.m_fonts[ e_font_names::font_name_icon_12 ]->FontSize, FLT_MAX, 0.f, text );

						g_render.m_draw_data.emplace_back(
							e_draw_type::draw_type_text,
							std::make_any< text_draw_object_t >(
								g_render.m_fonts[ e_font_names::font_name_icon_12 ],
								c_vector_2d( box.m_left + ( box.m_width - text_size.x ) / 2,
						                     box.m_bottom + 2 + padding[ e_padding_direction::padding_direction_bottom ] ),
								text, GET_VARIABLE( g_variables.m_weapon_icon_color, c_color ).get_u32( 1.f ), ImColor( 0.f, 0.f, 0.f, 1.f ),
								text_flag_dropshadow ) );
					}
				}
			}
		}

		if ( GET_VARIABLE( g_variables.m_players_health_bar, bool ) ) {
			if ( this->m_backup_player_data[ index ].m_health > entity->get_health( ) )
				this->m_backup_player_data[ index ].m_health -=
					( 100.f *
				      g_interfaces.m_global_vars_base->m_frame_time ) /* NOTE ~ float ~ cannot use the variable delta_time as it is too slow. */;
			else
				this->m_backup_player_data[ index ].m_health = entity->get_health( );

			const float factor = static_cast< float >( this->m_backup_player_data[ index ].m_health ) / entity->get_max_health( );
			const float hue    = ( factor * 120.f ) / 360.f;

			g_render.m_draw_data.emplace_back( e_draw_type::draw_type_rect,
			                                   std::make_any< rect_draw_object_t >( c_vector_2d( box.m_left - 5.f, box.m_bottom - box.m_height ),
			                                                                        c_vector_2d( box.m_left - 3.f, box.m_bottom ),
			                                                                        ImColor( 0.f, 0.f, 0.f, 1.f ), ImColor( 0.f, 0.f, 0.f, 1.f ),
			                                                                        false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f,
			                                                                        e_rect_flags::rect_flag_outer_outline ) );

			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_rect,
				std::make_any< rect_draw_object_t >(
					c_vector_2d( box.m_left - 5.f, box.m_bottom - ( box.m_height * factor ) ), c_vector_2d( box.m_left - 3.f, box.m_bottom ),
					GET_VARIABLE( g_variables.m_players_health_bar_custom_color, bool )
						? GET_VARIABLE( g_variables.m_players_health_bar_color, c_color ).get_u32( 1.f )
						: c_color::from_hsb( hue, 1.f, 1.f, 1.f ).get_u32( ),
					ImColor( 0.f, 0.f, 0.f, 1.f ), false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f, e_rect_flags::rect_flag_outer_outline ) );
		}

		[ & ]( const bool draw_skeleton ) {
			if ( !draw_skeleton )
				return;

			if ( GET_VARIABLE( g_variables.m_players_skeleton_type, int ) == 1 && GET_VARIABLE( g_variables.m_backtrack_enable, bool ) ) {
				auto record = g_lagcomp.oldest_record( entity->get_index( ) );

				if ( !record.has_value( ) )
					return;

				auto bone_matrix = record.value( ).m_matrix;

				if ( bone_matrix ) {
					const auto model = client_renderable->get_model( );
					if ( model ) {
						const auto studio_model = g_interfaces.m_model_info->get_studio_model( model );

						if ( studio_model ) {
							c_vector child_position = { }, parent_position = { };
							c_vector_2d child_screen_position = { }, parent_screen_position = { };

							c_vector upper_direction =
								c_vector( bone_matrix[ 7 ][ 0 ][ 3 ], bone_matrix[ 7 ][ 1 ][ 3 ], bone_matrix[ 7 ][ 2 ][ 3 ] ) -
								c_vector( bone_matrix[ 6 ][ 0 ][ 3 ], bone_matrix[ 6 ][ 1 ][ 3 ], bone_matrix[ 6 ][ 2 ][ 3 ] );
							c_vector breast_bone = c_vector( bone_matrix[ 6 ][ 0 ][ 3 ], bone_matrix[ 6 ][ 1 ][ 3 ], bone_matrix[ 6 ][ 2 ][ 3 ] ) +
							                       upper_direction * 0.5f;

							for ( int i = 0; i < studio_model->n_bones; i++ ) {
								mstudiobone_t* bone = studio_model->get_bone( i );
								if ( !bone )
									continue;

								if ( bone->m_parent == -1 )
									continue;

								if ( !( bone->m_flags & 0x00000100 /* BONE_USED_BY_HITBOX */ ) )
									continue;

								child_position  = c_vector( bone_matrix[ i ][ 0 ][ 3 ], bone_matrix[ i ][ 1 ][ 3 ], bone_matrix[ i ][ 2 ][ 3 ] );
								parent_position = c_vector( bone_matrix[ bone->m_parent ][ 0 ][ 3 ], bone_matrix[ bone->m_parent ][ 1 ][ 3 ],
								                            bone_matrix[ bone->m_parent ][ 2 ][ 3 ] );

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

			} else {
				matrix3x4_t bone_matrix[ 128 ]{ };

				memcpy( bone_matrix, entity->get_cached_bone_data( ).get_elements( ),
				        entity->get_cached_bone_data( ).count( ) * sizeof( matrix3x4_t ) );

				if ( bone_matrix ) {
					const auto model = client_renderable->get_model( );
					if ( model ) {
						const auto studio_model = g_interfaces.m_model_info->get_studio_model( model );

						if ( studio_model ) {
							c_vector child_position = { }, parent_position = { };
							c_vector_2d child_screen_position = { }, parent_screen_position = { };

							c_vector upper_direction =
								c_vector( bone_matrix[ 7 ][ 0 ][ 3 ], bone_matrix[ 7 ][ 1 ][ 3 ], bone_matrix[ 7 ][ 2 ][ 3 ] ) -
								c_vector( bone_matrix[ 6 ][ 0 ][ 3 ], bone_matrix[ 6 ][ 1 ][ 3 ], bone_matrix[ 6 ][ 2 ][ 3 ] );
							c_vector breast_bone = c_vector( bone_matrix[ 6 ][ 0 ][ 3 ], bone_matrix[ 6 ][ 1 ][ 3 ], bone_matrix[ 6 ][ 2 ][ 3 ] ) +
							                       upper_direction * 0.5f;

							for ( int i = 0; i < studio_model->n_bones; i++ ) {
								mstudiobone_t* bone = studio_model->get_bone( i );
								if ( !bone )
									continue;

								if ( bone->m_parent == -1 )
									continue;

								if ( !( bone->m_flags & 0x00000100 /* BONE_USED_BY_HITBOX */ ) )
									continue;

								child_position  = c_vector( bone_matrix[ i ][ 0 ][ 3 ], bone_matrix[ i ][ 1 ][ 3 ], bone_matrix[ i ][ 2 ][ 3 ] );
								parent_position = c_vector( bone_matrix[ bone->m_parent ][ 0 ][ 3 ], bone_matrix[ bone->m_parent ][ 1 ][ 3 ],
								                            bone_matrix[ bone->m_parent ][ 2 ][ 3 ] );

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
		}( GET_VARIABLE( g_variables.m_players_skeleton, bool ) );

		if ( GET_VARIABLE( g_variables.m_players_backtrack_trail, bool ) ) {
			if ( const auto record_list = g_lagcomp.m_records[ index ] ) {
				for ( int i = 0; i < g_ctx.m_max_allocations; i++ ) {
					auto record = record_list[ i ];

					if ( !record.m_valid )
						continue;

					c_vector_2d out{ };

					if ( !g_render.world_to_screen( entity->get_hitbox_position( hitbox_head, record.m_matrix ), out ) )
						continue;

					g_render.m_draw_data.emplace_back(
						e_draw_type::draw_type_rect,
						std::make_any< rect_draw_object_t >( out, c_vector_2d( out.m_x + 1, out.m_y + 1 ), ImColor( 1.f, 1.f, 1.f, 1.f ), ImColor( ),
					                                         true, 0.f, ImDrawFlags_::ImDrawFlags_None, 4.f, e_rect_flags::rect_flag_none ) );
				}
			}
		}
	} );
}
