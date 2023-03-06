#pragma once
#include "players.h"
#include "../../../game/sdk/classes/c_csgo_hud_radar.h"
#include "../../../game/sdk/includes/includes.h"
#include "../../../globals/includes/includes.h"
#include "../../avatar_cache/avatar_cache.h"
#include "../../entity_cache/entity_cache.h"
#include "dormancy/dormancy.h"

void n_players::impl_t::on_paint_traverse( )
{
	if ( GET_VARIABLE( g_variables.m_players, bool ) )
		this->players( );
}

void n_players::impl_t::players( )
{
	// dormancy-related
	static unsigned long radar_base{ };
	static c_csgo_hud_radar* hud_radar{ };

	[]( ) {
		g_dormancy.think( );

		// gonna be used later on.

		// static auto find_hud_element_ptr = reinterpret_cast< DWORD( __thiscall* )( void*, const char* ) >(
		// 	g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39 77 28" ) );
		//
		// static auto hud_ptr = *reinterpret_cast< DWORD** >( g_modules[ CLIENT_DLL ].find_pattern( "81 25 ? ? ? ? ? ? ? ? 8B 01" ) + 0x2 );
		//
		// radar_base = find_hud_element_ptr( hud_ptr, "CCSGO_HudRadar" );
		// hud_radar  = reinterpret_cast< c_csgo_hud_radar* >( radar_base - 0x14 );
	}( );

	const float delta_time = ( 0.7f / 0.5f ) * g_interfaces.m_global_vars_base->m_frame_time;

	c_angle view_angles = { };
	g_interfaces.m_engine_client->get_view_angles( view_angles );

	const float yaw = deg2rad( view_angles.m_y );

	const auto display_size = ImGui::GetIO( ).DisplaySize;

	g_entity_cache.enumerate( e_enumeration_type::type_players, [ & ]( c_base_entity* entity ) {
		if ( !entity || entity == g_ctx.m_local || !g_ctx.m_local->is_enemy( entity ) || !entity->is_alive( ) )
			return;

		const auto index = entity->get_index( );

		bool valid_dormant       = false;
		const auto backup_flags  = entity->get_flags( );
		const auto backup_origin = entity->get_abs_origin( );

		if ( entity->is_dormant( ) )
			valid_dormant = g_dormancy.adjust_player_sound( entity );
		else
			g_dormancy.m_sound_players[ index ].reset( true, entity->get_abs_origin( ), entity->get_flags( ) );

		if ( entity->is_dormant( ) ) {
			if ( entity->get_abs_origin( ).is_zero( ) )
				this->m_fading_alpha[ index ] = 0;
			else if ( !valid_dormant && this->m_fading_alpha[ index ] > 0.f &&
			          g_interfaces.m_global_vars_base->m_current_time - m_stored_cur_time[ index ] > 2.f )
				this->m_fading_alpha[ index ] -= delta_time;
			else if ( valid_dormant && this->m_fading_alpha[ index ] < 1.f )
				this->m_fading_alpha[ index ] += delta_time;
		} else if ( this->m_fading_alpha[ index ] < 1.f ) {
			m_stored_cur_time[ index ] = g_interfaces.m_global_vars_base->m_current_time;
			this->m_fading_alpha[ index ] += delta_time;
		}
		const auto collideable = entity->get_collideable( );
		if ( !collideable )
			return;

		if ( this->m_fading_alpha[ index ] >= 1.0f )
			this->m_fading_alpha[ index ] = 1.0f;

		if ( this->m_fading_alpha[ index ] <= 0.0f ) {
			this->m_fading_alpha[ index ] = 0.0f;
			return;
		}

		const bool in_view_frustrum = !g_interfaces.m_engine_client->cull_box( collideable->obb_mins( ) + entity->get_abs_origin( ),
		                                                                       collideable->obb_maxs( ) + entity->get_abs_origin( ) );

		const auto client_renderable = entity->get_client_renderable( );
		if ( !client_renderable )
			return;

		const auto client_unknown = client_renderable->get_client_unknown( );
		if ( !client_unknown )
			return;

		const auto client_networkable = client_unknown->get_client_networkable( );
		if ( !client_networkable )
			return;

		[ & ]( ) {
			if ( !GET_VARIABLE( g_variables.m_out_of_fov_arrows, bool ) || in_view_frustrum )
				return;

			const c_vector position_difference = g_ctx.m_local->get_abs_origin( ) - entity->get_abs_origin( );

			float x = std::cos( yaw ) * position_difference.m_y - std::sin( yaw ) * position_difference.m_x;
			float y = std::cos( yaw ) * position_difference.m_x + std::sin( yaw ) * position_difference.m_y;
			if ( const auto len = std::sqrt( x * x + y * y ); len ) {
				x /= len;
				y /= len;
			}

			const float triangle_size          = GET_VARIABLE( g_variables.m_out_of_fov_arrows_size, float );
			const int triangle_screen_position = GET_VARIABLE( g_variables.m_out_of_fov_arrows_distance, int );

			const c_vector_2d position =
				c_vector_2d( display_size.x / 2.f + ( x * triangle_screen_position ), display_size.y / 2.f + ( y * triangle_screen_position ) );

			const c_vector_2d triangle_position        = c_vector_2d( position.m_x + x * 3.f, position.m_y + y * 3.f );
			const c_vector_2d triangle_render_points[] = {
				c_vector_2d( 0.4f * y * triangle_size + triangle_position.m_x, triangle_position.m_y + -0.4f * x * triangle_size ),
				c_vector_2d( triangle_position.m_x + 1.0f * x * triangle_size, triangle_position.m_y + 1.0f * y * triangle_size ),
				c_vector_2d( triangle_position.m_x + -0.4f * y * triangle_size, triangle_position.m_y + 0.4f * x * triangle_size )
			};

			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_triangle,
				std::make_any< triangle_draw_object_t >(
					triangle_render_points[ 0 ], triangle_render_points[ 1 ], triangle_render_points[ 2 ],
					GET_VARIABLE( g_variables.m_out_of_fov_arrows_color, c_color ).get_u32( this->m_fading_alpha[ index ] ),
					e_triangle_flags::triangle_flag_filled ) );
		}( );

		bounding_box_t box{ };
		if ( !entity->get_bounding_box( &box ) )
			return;

		player_info_t player_info{ };
		if ( !g_interfaces.m_engine_client->get_player_info( index, &player_info ) )
			return;

		// fuck you
		std::string hp_text = std::to_string( entity->get_health( ) );
		if ( GET_VARIABLE( g_variables.m_players_health_suffix, bool ) )
			hp_text.append( "hp" );

		const int team = entity->get_team( );

		if ( this->m_backup_player_data[ index ].m_animated_health > entity->get_health( ) )
			this->m_backup_player_data[ index ].m_animated_health -=
				( 100.f * g_interfaces.m_global_vars_base->m_frame_time ) /* NOTE ~ float ~ cannot use the variable delta_time as it is too slow. */;
		else
			this->m_backup_player_data[ index ].m_animated_health = entity->get_health( );

		const float factor = static_cast< float >( this->m_backup_player_data[ index ].m_animated_health ) / entity->get_max_health( );
		const float hue    = ( factor * 120.f ) / 360.f;

		float padding[ e_padding_direction::padding_direction_max ] = { 0.f, 0.f, 0.f, 0.f };

		if ( GET_VARIABLE( g_variables.m_players_box, bool ) ) {
			if ( !GET_VARIABLE( g_variables.m_players_box_corner, bool ) )
				g_render.m_draw_data.emplace_back(
					e_draw_type::draw_type_rect,
					std::make_any< rect_draw_object_t >(
						c_vector_2d( box.m_left, box.m_top ), c_vector_2d( box.m_right, box.m_bottom ),
						GET_VARIABLE( g_variables.m_players_box_color, c_color ).get_u32( this->m_fading_alpha[ index ] ),
						GET_VARIABLE( g_variables.m_players_box_outline_color, c_color ).get_u32( this->m_fading_alpha[ index ] ), false, 0.f,
						ImDrawFlags_::ImDrawFlags_None, 1.f,
						GET_VARIABLE( g_variables.m_players_box_outline, bool )
							? e_rect_flags::rect_flag_inner_outline | e_rect_flags::rect_flag_outer_outline
							: e_rect_flags::rect_flag_none ) );
			else {
				if ( GET_VARIABLE( g_variables.m_players_box_outline, bool ) ) {
					g_render.corner_rect(
						box.m_left, box.m_top, box.m_right, box.m_bottom,
						c_color( GET_VARIABLE( g_variables.m_players_box_outline_color, c_color ) ).get_u32( this->m_fading_alpha[ index ] ), 2.f );
				}

				g_render.corner_rect( box.m_left, box.m_top, box.m_right, box.m_bottom,
				                      c_color( GET_VARIABLE( g_variables.m_players_box_color, c_color ) ).get_u32( this->m_fading_alpha[ index ] ) );
			}
		}

		if ( GET_VARIABLE( g_variables.m_players_name, bool ) ) {
			std::string converted_name = std::string( player_info.m_name ).substr( 0, 24 );

			if ( converted_name.length( ) > 23U )
				converted_name.append( "..." );

			if ( player_info.m_fake_player )
				converted_name.insert( 0, "[bot] " );

			const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
				g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, converted_name.c_str( ) );

			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_text,
				std::make_any< text_draw_object_t >(
					g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
					c_vector_2d( box.m_left + box.m_width * 0.5f - text_size.x * 0.5f, box.m_top - 3 - text_size.y ), converted_name,
					GET_VARIABLE( g_variables.m_players_name_color, c_color ).get_u32( this->m_fading_alpha[ index ] ),
					c_color( 0.f, 0.f, 0.f, 1.f ).get_u32( this->m_fading_alpha[ index ] ), e_text_flags::text_flag_dropshadow ) );

			padding[ e_padding_direction::padding_direction_top ] -= text_size.y - 20;
		}

		if ( GET_VARIABLE( g_variables.m_players_health_text, bool ) && GET_VARIABLE( g_variables.m_players_health_text_style, int ) == 0 ) {
			const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_11 ]->CalcTextSizeA(
				g_render.m_fonts[ e_font_names::font_name_verdana_11 ]->FontSize, FLT_MAX, 0.f, hp_text.c_str( ) );

			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_text,
				std::make_any< text_draw_object_t >( g_render.m_fonts[ e_font_names::font_name_verdana_11 ],
			                                         c_vector_2d( box.m_left + ( box.m_width - text_size.x ) / 2,
			                                                      box.m_bottom + 2 + padding[ e_padding_direction::padding_direction_bottom ] ),
			                                         hp_text, c_color::from_hsb( hue, 1.f, 1.f, 1.f ).get_u32( this->m_fading_alpha[ index ] ),
			                                         ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), text_flag_dropshadow ) );

			padding[ e_padding_direction::padding_direction_bottom ] += ( text_size.y + 1.f );
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
							std::make_any< rect_draw_object_t >(
								c_vector_2d( box.m_left, box.m_bottom + 3.f ), c_vector_2d( box.m_left + box.m_width, box.m_bottom + 5.f ),
								ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ),
								false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f, rect_flag_outer_outline ) );

						g_render.m_draw_data.emplace_back(
							e_draw_type::draw_type_rect,
							std::make_any< rect_draw_object_t >(
								c_vector_2d( box.m_left, box.m_bottom + 3.f ), c_vector_2d( box.m_left + box.m_width * factor, box.m_bottom + 5.f ),
								GET_VARIABLE( g_variables.m_player_ammo_bar_color, c_color ).get_u32( this->m_fading_alpha[ index ] ),
								ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f,
								rect_flag_outer_outline ) );

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
								converted_name, GET_VARIABLE( g_variables.m_weapon_name_color, c_color ).get_u32( this->m_fading_alpha[ index ] ),
								ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), text_flag_dropshadow ) );

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
								text, GET_VARIABLE( g_variables.m_weapon_icon_color, c_color ).get_u32( this->m_fading_alpha[ index ] ),
								ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), text_flag_dropshadow ) );

						padding[ e_padding_direction::padding_direction_bottom ] += text_size.y;
					}
				}
			}
		}

		if ( GET_VARIABLE( g_variables.m_players_avatar, bool ) ) {
			constexpr const static auto avatar_size = 14.f;

			g_render.m_draw_data.emplace_back( e_draw_type::draw_type_texture,
			                                   std::make_any< texture_draw_object_t >(
												   c_vector_2d( box.m_left + ( box.m_width - avatar_size ) * 0.5f,
			                                                    box.m_top - 5 - avatar_size - padding[ e_padding_direction::padding_direction_top ] ),
												   c_vector_2d( avatar_size, avatar_size ), ImColor( 1.f, 1.f, 1.f, this->m_fading_alpha[ index ] ),
												   player_info.m_fake_player
													   ? team == 2 /* terrorist */           ? g_render.m_terrorist_avatar
			                                             : team == 3 /* counter terrorist */ ? g_render.m_counter_terrorist_avatar
			                                                                                 : nullptr
													   : g_avatar_cache[ index ],
												   0.f, ImDrawFlags_::ImDrawFlags_None ) );

			padding[ e_padding_direction::padding_direction_top ] -= avatar_size;
		}

		if ( GET_VARIABLE( g_variables.m_players_health_bar, bool ) ) {
			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_rect,
				std::make_any< rect_draw_object_t >(
					c_vector_2d( box.m_left - 5.f, box.m_bottom - box.m_height ), c_vector_2d( box.m_left - 3.f, box.m_bottom ),
					ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false, 0.f,
					ImDrawFlags_::ImDrawFlags_None, 1.f, e_rect_flags::rect_flag_outer_outline ) );

			g_render.m_draw_data.emplace_back(
				e_draw_type::draw_type_rect,
				std::make_any< rect_draw_object_t >(
					c_vector_2d( box.m_left - 5.f, box.m_bottom - ( box.m_height * factor ) ), c_vector_2d( box.m_left - 3.f, box.m_bottom ),
					GET_VARIABLE( g_variables.m_players_health_bar_custom_color, bool )
						? GET_VARIABLE( g_variables.m_players_health_bar_color, c_color ).get_u32( this->m_fading_alpha[ index ] )
						: c_color::from_hsb( hue, 1.f, 1.f, 1.f ).get_u32( this->m_fading_alpha[ index ] ),
					ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f,
					e_rect_flags::rect_flag_outer_outline ) );

			if ( GET_VARIABLE( g_variables.m_players_health_text, bool ) && GET_VARIABLE( g_variables.m_players_health_text_style, int ) == 1 ) {
				const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_11 ]->CalcTextSizeA(
					g_render.m_fonts[ e_font_names::font_name_verdana_11 ]->FontSize, FLT_MAX, 0.f, hp_text.c_str( ) );

				g_render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >( g_render.m_fonts[ e_font_names::font_name_verdana_11 ],
				                                         c_vector_2d( box.m_left - 5.f - text_size.x, box.m_bottom - ( box.m_height * factor ) ),
				                                         hp_text, c_color( 1.f, 1.f, 1.f ).get_u32( this->m_fading_alpha[ index ] ),
				                                         ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), text_flag_dropshadow ) );
			}
		}

		[ & ]( const bool draw_skeleton ) {
			if ( !draw_skeleton )
				return;

			static auto draw_skeleton_matrix = [ & ]( matrix3x4_t* bone_matrix ) {
				if ( !bone_matrix )
					return;

				const auto model = client_renderable->get_model( );
				if ( model ) {
					const auto studio_model = g_interfaces.m_model_info->get_studio_model( model );

					if ( studio_model ) {
						c_vector child_position = { }, parent_position = { };
						c_vector_2d child_screen_position = { }, parent_screen_position = { };

						c_vector upper_direction = c_vector( bone_matrix[ 7 ][ 0 ][ 3 ], bone_matrix[ 7 ][ 1 ][ 3 ], bone_matrix[ 7 ][ 2 ][ 3 ] ) -
						                           c_vector( bone_matrix[ 6 ][ 0 ][ 3 ], bone_matrix[ 6 ][ 1 ][ 3 ], bone_matrix[ 6 ][ 2 ][ 3 ] );
						c_vector breast_bone =
							c_vector( bone_matrix[ 6 ][ 0 ][ 3 ], bone_matrix[ 6 ][ 1 ][ 3 ], bone_matrix[ 6 ][ 2 ][ 3 ] ) + upper_direction * 0.5f;

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
									std::make_any< line_draw_object_t >(
										child_screen_position, parent_screen_position,
										GET_VARIABLE( g_variables.m_players_skeleton_color, c_color ).get_u32( this->m_fading_alpha[ index ] ),
										1.f ) );
							}
						}
					}
				}
			};

			if ( GET_VARIABLE( g_variables.m_players_skeleton_type, int ) == 1 && GET_VARIABLE( g_variables.m_backtrack_enable, bool ) ) {
				auto record = g_lagcomp.oldest_record( entity->get_index( ) );

				if ( !record.has_value( ) )
					return;

				auto bone_matrix = record.value( ).m_matrix;

				draw_skeleton_matrix( bone_matrix );

			} else {
				matrix3x4_t bone_matrix[ 128 ]{ };

				memcpy( bone_matrix, entity->get_cached_bone_data( ).get_elements( ),
				        entity->get_cached_bone_data( ).count( ) * sizeof( matrix3x4_t ) );

				draw_skeleton_matrix( bone_matrix );
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

					g_render.m_draw_data.emplace_back( e_draw_type::draw_type_rect,
					                                   std::make_any< rect_draw_object_t >( out, c_vector_2d( out.m_x + 1, out.m_y + 1 ),
					                                                                        ImColor( 1.f, 1.f, 1.f, this->m_fading_alpha[ index ] ),
					                                                                        ImColor( ), true, 0.f, ImDrawFlags_::ImDrawFlags_None,
					                                                                        4.f, e_rect_flags::rect_flag_none ) );
				}
			}
		}

		// restore

		if ( entity->is_dormant( ) ) {
			entity->get_flags( ) = backup_flags;
			entity->set_abs_origin( backup_origin );
		}
	} );
}
