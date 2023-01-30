#include "players.h"
#include "../../../hooks/hooks.h"
#include "../../../includes.h"
#include "../../../source_engine/enumerations/e_item_definition_index.h"
#include "../../../source_engine/enumerations/e_weapon_type.h"
#include "../../../utilities/mathematics/mathematics.h"
#include "../../entities/avatars.h"
#include "../../entities/entities.h"

void players_t::on_paint_traverse( )
{
	if ( !render.m_initialised )
		return;

	if ( !globals.m_local )
		return;

	const float delta_time = ( 1.f / 0.4f ) * memory.m_globals->m_frame_time;

	/* TODO ~ float ~ store this shart globally */
	c_angle view_angles = { };
	interfaces.m_engine->get_view_angles( view_angles );

	const float yaw = DEG2RAD( view_angles.m_y );

	const auto display_size = ImGui::GetIO( ).DisplaySize;

	entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
		if ( !entity || globals.m_local == entity || entity->team( ) == globals.m_local->team( ) )
			return;

		const auto collideable = entity->collideable( );
		if ( !collideable )
			return;

		const int index = entity->index( );

		entity->is_dormant( ) || !entity->is_alive( ) ? this->m_fading_alpha[ index ] -= delta_time : this->m_fading_alpha[ index ] += delta_time;

		if ( this->m_fading_alpha[ index ] >= 1.0f )
			this->m_fading_alpha[ index ] = 1.0f;

		if ( this->m_fading_alpha[ index ] <= 0.0f ) {
			this->m_fading_alpha[ index ] = 0.0f;
			return;
		}

		const bool in_view_frustrum =
			!interfaces.m_engine->cull_box( collideable->obb_mins( ) + entity->origin( ), collideable->obb_maxs( ) + entity->origin( ) );

		auto out_of_fov_arrows = [ & ]( ) {
			if ( !GET_CONFIG_BOOL( variables.m_visuals.m_out_of_fov_arrows ) || in_view_frustrum )
				return;

			const c_vector position_difference = globals.m_local->origin( ) - entity->origin( );

			float x = std::cos( yaw ) * position_difference.m_y - std::sin( yaw ) * position_difference.m_x;
			float y = std::cos( yaw ) * position_difference.m_x + std::sin( yaw ) * position_difference.m_y;
			if ( const auto len = std::sqrt( x * x + y * y ); len ) {
				x /= len;
				y /= len;
			}

			const float triangle_size          = GET_CONFIG_FLOAT( variables.m_visuals.m_out_of_fov_arrows_size );
			const int triangle_screen_position = GET_CONFIG_INT( variables.m_visuals.m_out_of_fov_arrows_distance );

			const c_vector_2d position = c_vector_2d( ImGui::GetIO( ).DisplaySize.x / 2.f + ( x * triangle_screen_position ),
			                                          ImGui::GetIO( ).DisplaySize.y / 2.f + ( y * triangle_screen_position ) );

			const c_vector_2d triangle_position        = c_vector_2d( position.m_x + x * 3.f, position.m_y + y * 3.f );
			const c_vector_2d triangle_render_points[] = {
				c_vector_2d( 0.4f * y * triangle_size + triangle_position.m_x, triangle_position.m_y + -0.4f * x * triangle_size ),
				c_vector_2d( triangle_position.m_x + 1.0f * x * triangle_size, triangle_position.m_y + 1.0f * y * triangle_size ),
				c_vector_2d( triangle_position.m_x + -0.4f * y * triangle_size, triangle_position.m_y + 0.4f * x * triangle_size )
			};

			render.m_draw_data.emplace_back(
				e_draw_type::draw_type_triangle,
				std::make_any< triangle_draw_object_t >(
					triangle_render_points[ 0 ], triangle_render_points[ 1 ], triangle_render_points[ 2 ],
					GET_CONFIG_COLOR( variables.m_visuals.m_out_of_fov_arrows_color ).get_u32( this->m_fading_alpha[ index ] ),
					e_triangle_render_flags::triangle_render_flag_filled ) );
		};
		out_of_fov_arrows( );

		bounding_box_t bbox = { };
		if ( !entity->get_bounding_box( &bbox ) )
			return;

		float padding[ e_padding_direction::padding_direction_max ] = { 0.f, 0.f, 0.f, 0.f };

		player_info_t player_info = { };
		if ( !interfaces.m_engine->get_player_info( index, &player_info ) )
			return;

		const auto health = entity->health( );
		const auto team   = entity->team( );

		/* TODO ~ move these lambdas into a separate function if you want breh */
		auto bounding_box = [ & ]( ) {
			if ( GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box ) ) {
				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_rect,
					std::make_any< rect_draw_object_t >(
						c_vector_2d( bbox.m_left, bbox.m_top ), c_vector_2d( bbox.m_right, bbox.m_bottom ),
						GET_CONFIG_COLOR( variables.m_visuals.m_bounding_box_color ).get_u32( this->m_fading_alpha[ index ] ),
						ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f,
						GET_CONFIG_BOOL( variables.m_visuals.m_bounding_box_outline )
							? e_rect_render_flags::rect_render_flag_inner_outline | e_rect_render_flags::rect_render_flag_outer_outline
							: e_rect_render_flags::rect_render_flag_none ) );
			}
		};
		bounding_box( );

		auto player_name = [ & ]( ) {
			if ( GET_CONFIG_BOOL( variables.m_visuals.m_player_name ) ) {
				std::string converted_name = player_info.m_name;
				if ( converted_name.length( ) > 24U )
					converted_name = converted_name.substr( 0U, 24U ).append( "..." );

				if ( player_info.m_fake_player )
					converted_name.insert( 0, ( "[bot] " ) );

				const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, converted_name.c_str( ) );

				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
						c_vector_2d( bbox.m_left + bbox.m_width * 0.5f - text_size.x * 0.5f, bbox.m_top - 3 - text_size.y ), converted_name,
						GET_CONFIG_COLOR( variables.m_visuals.m_player_name_color ).get_u32( this->m_fading_alpha[ index ] ),
						ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), e_text_render_flags::text_render_flag_dropshadow ) );

				padding[ e_padding_direction::padding_direction_top ] += text_size.y;
			}
		};
		player_name( );

		const auto active_weapon =
			reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity_from_handle( entity->active_weapon_handle( ) ) );
		if ( active_weapon )
			this->m_backup_player_data[ index ].m_active_weapon = active_weapon;

		if ( this->m_backup_player_data[ index ].m_active_weapon ) {
			const auto item_definition_index = this->m_backup_player_data[ index ].m_active_weapon->item_definition_index( );
			const auto weapon_data           = memory.m_weapon_system->get_weapon_data( item_definition_index );
			if ( weapon_data )
				this->m_backup_player_data[ index ].m_weapon_data = weapon_data;

			if ( this->m_backup_player_data[ index ].m_weapon_data ) {
				auto player_ammo_bar = [ & ]( ) {
					if ( !GET_CONFIG_BOOL( variables.m_visuals.m_weapon_ammo_bar ) )
						return;

					if ( !this->m_backup_player_data[ index ].m_weapon_data->is_gun( ) )
						return;

					const float ammo = static_cast< float >( this->m_backup_player_data[ index ].m_active_weapon->ammo( ) );

					if ( this->m_backup_player_data[ index ].m_ammo > ammo )
						this->m_backup_player_data[ index ].m_ammo -=
							( 2.f * memory.m_globals->m_frame_time ) /* NOTE ~ float ~ cannot use the variable delta_time as it is too slow. */;
					else
						this->m_backup_player_data[ index ].m_ammo = ammo;

					const int max_ammo = this->m_backup_player_data[ index ].m_weapon_data->m_max_clip1;

					float factor = this->m_backup_player_data[ index ].m_ammo / static_cast< float >( max_ammo );
					factor       = std::clamp< float >( factor, 0.f, 1.f ); /* if this is not here, it will cause the width to be really big :3 */

					render.m_draw_data.emplace_back(
						e_draw_type::draw_type_rect,
						std::make_any< rect_draw_object_t >(
							c_vector_2d( bbox.m_left, bbox.m_bottom + 3.f ), c_vector_2d( bbox.m_left + bbox.m_width, bbox.m_bottom + 5.f ),
							ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false,
							0.f, ImDrawFlags_::ImDrawFlags_None, 1.f, e_rect_render_flags::rect_render_flag_outer_outline ) );

					render.m_draw_data.emplace_back(
						e_draw_type::draw_type_rect,
						std::make_any< rect_draw_object_t >(
							c_vector_2d( bbox.m_left, bbox.m_bottom + 3.f ), c_vector_2d( bbox.m_left + bbox.m_width * factor, bbox.m_bottom + 5.f ),
							GET_CONFIG_COLOR( variables.m_visuals.m_weapon_ammo_bar_color ).get_u32( this->m_fading_alpha[ index ] ),
							ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f,
							e_rect_render_flags::rect_render_flag_outer_outline ) );

					padding[ e_padding_direction::padding_direction_bottom ] += 6.f;
				};
				player_ammo_bar( );

				auto active_weapons = [ & ]( ) {
					if ( GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_name ) ) {
						const auto localized_name = interfaces.m_localize->find( this->m_backup_player_data[ index ].m_weapon_data->m_hud_name );

						const std::wstring w = localized_name;
						if ( w.empty( ) )
							return;

						const std::string converted_name( w.begin( ), w.end( ) );
						if ( converted_name.empty( ) )
							return;

						const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
							render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, converted_name.c_str( ) );

						render.m_draw_data.emplace_back(
							e_draw_type::draw_type_text,
							std::make_any< text_draw_object_t >(
								render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
								c_vector_2d( bbox.m_left + ( bbox.m_width - text_size.x ) / 2,
						                     bbox.m_bottom + 2 + padding[ e_padding_direction::padding_direction_bottom ] ),
								converted_name,
								GET_CONFIG_COLOR( variables.m_visuals.m_active_weapon_name_color ).get_u32( this->m_fading_alpha[ index ] ),
								ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), e_text_render_flags::text_render_flag_dropshadow ) );

						padding[ e_padding_direction::padding_direction_bottom ] += ( text_size.y + 1.f );
					}

					if ( GET_CONFIG_BOOL( variables.m_visuals.m_active_weapon_icon ) ) {
						if ( !( item_definition_index == e_item_definition_index::weapon_shield ||
						        item_definition_index == e_item_definition_index::weapon_breachcharge ||
						        item_definition_index == e_item_definition_index::weapon_bumpmine ) ) {
							const auto text      = reinterpret_cast< const char* >( utilities.get_weapon_icon( item_definition_index ) );
							const auto text_size = render.m_fonts[ e_font_names::font_name_icon_12 ]->CalcTextSizeA(
								render.m_fonts[ e_font_names::font_name_icon_12 ]->FontSize, FLT_MAX, 0.f, text );

							render.m_draw_data.emplace_back(
								e_draw_type::draw_type_text,
								std::make_any< text_draw_object_t >(
									render.m_fonts[ e_font_names::font_name_icon_12 ],
									c_vector_2d( bbox.m_left + ( bbox.m_width - text_size.x ) / 2,
							                     bbox.m_bottom + 2 + padding[ e_padding_direction::padding_direction_bottom ] ),
									text, GET_CONFIG_COLOR( variables.m_visuals.m_active_weapon_icon_color ).get_u32( this->m_fading_alpha[ index ] ),
									ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), e_text_render_flags::text_render_flag_dropshadow ) );
						}
					}
				};
				active_weapons( );
			}
		}

		auto player_health_bar = [ & ]( ) {
			if ( !GET_CONFIG_BOOL( variables.m_visuals.m_player_health_bar ) )
				return;

			if ( this->m_backup_player_data[ index ].m_health > health )
				this->m_backup_player_data[ index ].m_health -=
					( 100.f * memory.m_globals->m_frame_time ) /* NOTE ~ float ~ cannot use the variable delta_time as it is too slow. */;
			else
				this->m_backup_player_data[ index ].m_health = health;

			const float factor = static_cast< float >( this->m_backup_player_data[ index ].m_health ) / max_health;
			const float hue    = ( factor * 120.f ) / 360.f;

			render.m_draw_data.emplace_back(
				e_draw_type::draw_type_rect,
				std::make_any< rect_draw_object_t >(
					c_vector_2d( bbox.m_left - 5.f, bbox.m_bottom - bbox.m_height ), c_vector_2d( bbox.m_left - 3.f, bbox.m_bottom ),
					ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false, 0.f,
					ImDrawFlags_::ImDrawFlags_None, 1.f, e_rect_render_flags::rect_render_flag_outer_outline ) );

			render.m_draw_data.emplace_back(
				e_draw_type::draw_type_rect,
				std::make_any< rect_draw_object_t >(
					c_vector_2d( bbox.m_left - 5.f, bbox.m_bottom - ( bbox.m_height * factor ) ), c_vector_2d( bbox.m_left - 3.f, bbox.m_bottom ),
					GET_CONFIG_BOOL( variables.m_visuals.m_custom_color_health_bar )
						? GET_CONFIG_COLOR( variables.m_visuals.m_player_health_bar_color ).get_u32( this->m_fading_alpha[ index ] )
						: c_color::from_hsb( hue, 1.f, 1.f, this->m_fading_alpha[ index ] ).get_u32( ),
					ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), false, 0.f, ImDrawFlags_::ImDrawFlags_None, 1.f,
					e_rect_render_flags::rect_render_flag_outer_outline ) );
		};
		player_health_bar( );

		auto player_flags = [ & ]( ) {
			if ( config.get< std::vector< bool > >( variables.m_visuals.m_player_flags )[ e_player_flags::player_flag_money ] ) {
				const auto text = std::to_string( entity->money( ) ).insert( 0U, ( "$" ) );

				const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, text.c_str( ) );

				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
						c_vector_2d( bbox.m_right + 2.f, bbox.m_top - 3.f + padding[ e_padding_direction::padding_direction_right ] ), text,
						ImColor( 1.f, 1.f, 1.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ),
						e_text_render_flags::text_render_flag_dropshadow ) );

				padding[ e_padding_direction::padding_direction_right ] += text_size.y;
			}

			if ( config.get< std::vector< bool > >( variables.m_visuals.m_player_flags )[ e_player_flags::player_flag_armor ] ) {
				if ( !( entity->armor( ) > 0 ) )
					return;

				const auto text = entity->helmet( ) ? ( "hk" ) : ( "k" );

				const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, text );

				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
						c_vector_2d( bbox.m_right + 2.f, bbox.m_top - 3.f + padding[ e_padding_direction::padding_direction_right ] ), text,
						ImColor( 1.f, 1.f, 1.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ),
						e_text_render_flags::text_render_flag_dropshadow ) );

				padding[ e_padding_direction::padding_direction_right ] += text_size.y;
			}
			if ( config.get< std::vector< bool > >( variables.m_visuals.m_player_flags )[ e_player_flags::player_flag_reloading ] ) {
				if ( !entity->reloading( ) )
					return;

				constexpr static auto text = "reloading";

				const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, text );

				// red text
				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
						c_vector_2d( bbox.m_right + 2.f, bbox.m_top - 3.f + padding[ e_padding_direction::padding_direction_right ] ), text,
						ImColor( 1.f, 0.f, 0.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ),
						e_text_render_flags::text_render_flag_dropshadow ) );

				padding[ e_padding_direction::padding_direction_right ] += text_size.y;
			}
			if ( config.get< std::vector< bool > >( variables.m_visuals.m_player_flags )[ e_player_flags::player_flag_bomb ] ) {
				if ( !entity->has_bomb( ) )
					return;

				constexpr static auto text = "bomb";

				const auto text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, text );

				// red text
				render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
						c_vector_2d( bbox.m_right + 2.f, bbox.m_top - 3.f + padding[ e_padding_direction::padding_direction_right ] ), text,
						ImColor( 1.f, 1.f, 1.f, this->m_fading_alpha[ index ] ), ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ),
						e_text_render_flags::text_render_flag_dropshadow ) );

				padding[ e_padding_direction::padding_direction_right ] += text_size.y;
			}
		};
		player_flags( );

		auto player_avatar = [ & ]( ) {
			if ( !GET_CONFIG_BOOL( variables.m_visuals.m_player_avatar ) )
				return;

			constexpr auto avatar_size = 14.f;

			render.m_draw_data.emplace_back( e_draw_type::draw_type_texture,
			                                 std::make_any< texture_draw_object_t >(
												 c_vector_2d( bbox.m_left + ( bbox.m_width - avatar_size ) * 0.5f,
			                                                  bbox.m_top - 5 - avatar_size - padding[ e_padding_direction::padding_direction_top ] ),
												 c_vector_2d( avatar_size, avatar_size ), ImColor( 1.f, 1.f, 1.f, this->m_fading_alpha[ index ] ),
												 player_info.m_fake_player ? team == 2 /* terrorist */           ? render.m_terrorist_avatar
			                                                                 : team == 3 /* counter terrorist */ ? render.m_counter_terrorist_avatar
			                                                                                                     : nullptr
																		   : avatar_cache.find( index ),
												 0.f, ImDrawFlags_::ImDrawFlags_None ) );

			/* white outline, i personally don't thinker it looks good :p */
			/*render.m_draw_data.emplace_back(
			    e_draw_type::draw_type_rect,
			    std::make_any< rect_draw_object_t >(
			        c_vector_2d( bbox.m_left + ( bbox.m_width - avatar_size ) * 0.5f,
			                     bbox.m_top - 5 - avatar_size - padding[ e_padding_direction::padding_direction_top ] ),
			        c_vector_2d( bbox.m_left + ( bbox.m_width - avatar_size ) * 0.5f + avatar_size,
			                     bbox.m_top - 5 - avatar_size - padding[ e_padding_direction::padding_direction_top ] + avatar_size ),
			        ImColor( 0.f, 0.f, 0.f, this->m_fading_alpha[ index ] ),
			        GET_CONFIG_COLOR( variables.m_visuals.m_player_avatar_color ).get_u32( this->m_fading_alpha[ index ] ), false, 0.f,
			        ImDrawFlags_::ImDrawFlags_None, 1.f, e_rect_render_flags::rect_render_flag_outer_outline ) );*/
		};
		player_avatar( );
	} );
}

/*
        * @note: materials info:
        1	shaders: https://developer.valvesoftware.com/wiki/Category:Shaders
        -		parameters: https://developer.valvesoftware.com/wiki/Category:List_of_Shader_Parameters
        2	proxies: https://developer.valvesoftware.com/wiki/Material_proxies
        -		list: https://developer.valvesoftware.com/wiki/List_Of_Material_Proxies
        *
        * use "mat_texture_list 1" command to see full materials list
*/

void players_t::on_draw_model_execute( int ecx, int edx, void* context, void* state, model_render_info_t* info, matrix3x4_t* bone_to_world )
{
	static auto original = hooks.draw_model_execute.get_original< decltype( &n_detoured_functions::draw_model_execute ) >( );

	const auto entity = reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity( info->m_entity_index ) );
	if ( !entity || !entity->is_player( ) || !entity->is_alive( ) || entity == globals.m_local || entity->team( ) == globals.m_local->team( ) )
		return;

	constexpr auto override_material = [ & ]( c_material* material, const c_color& color, bool ignorez = false, bool wireframe = false,
	                                          bool is_overlay = false ) -> void {
		if ( is_overlay ) {
			bool found              = false;
			const auto material_var = material->find_var( "$envmaptint", &found );
			if ( found ) {
				material_var->set_vector( color.base< e_color_type::color_type_r >( ), color.base< e_color_type::color_type_g >( ),
				                          color.base< e_color_type::color_type_b >( ) );
			}
		}

		material->color_modulate( color.base< e_color_type::color_type_r >( ), color.base< e_color_type::color_type_g >( ),
		                          color.base< e_color_type::color_type_b >( ) );
		material->alpha_modulate( color.base< e_color_type::color_type_a >( ) );

		material->set_material_var_flag( e_material_var_flags::material_var_flag_ignorez, ignorez );
		material->set_material_var_flag( e_material_var_flags::material_var_flag_wireframe, wireframe );

		interfaces.m_model_render->forced_material_override( material );
	};

	const auto render_chams_layer = [ & ]( const c_chams_settings& chams_settings ) -> void {
		if ( chams_settings.m_render_original_model )
			original( ecx, edx, context, state, info, bone_to_world );

		if ( chams_settings.m_enable_invisible ) {
			override_material( this->m_base_materials[ chams_settings.m_invisible_material ], chams_settings.m_invisible_color, true );
			original( ecx, edx, context, state, info, bone_to_world );

			if ( !chams_settings.m_enable_visible ) {
				interfaces.m_model_render->forced_material_override( nullptr );
				original( ecx, edx, context, state, info, bone_to_world );
			}
		}

		if ( chams_settings.m_enable_invisible_overlay ) {
			override_material( this->m_overlay_materials[ chams_settings.m_invisible_overlay_material ], chams_settings.m_invisible_overlay_color,
			                   true );
			original( ecx, edx, context, state, info, bone_to_world );

			if ( !chams_settings.m_enable_visible_overlay ) {
				interfaces.m_model_render->forced_material_override( nullptr );
				original( ecx, edx, context, state, info, bone_to_world );
			}
		}

		if ( chams_settings.m_enable_visible ) {
			override_material( this->m_base_materials[ chams_settings.m_visible_material ], chams_settings.m_visible_color );
			original( ecx, edx, context, state, info, bone_to_world );
		}

		if ( chams_settings.m_enable_visible_overlay ) {
			override_material( this->m_overlay_materials[ chams_settings.m_visible_overlay_material ], chams_settings.m_visible_overlay_color );
			original( ecx, edx, context, state, info, bone_to_world );
		}
	};

	const auto get_base_material = [ & ]( int material_index ) -> c_material* {
		switch ( material_index ) {
		case 0: {
			return this->m_base_materials[ e_base_material_name::base_material_name_flat ];
			break;
		}
		case 1: {
			return this->m_base_materials[ e_base_material_name::base_material_name_textured ];
			break;
		}
		}
	};

	render_chams_layer( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_one ) );

	render_chams_layer( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_two ) );

	render_chams_layer( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_three ) );

	render_chams_layer( GET_CONFIG_CHAMS( variables.m_visuals.m_chams_layer_four ) );
}

void players_t::on_end_scene( )
{
	const ImColor accent_color = ImGui::GetColorU32( ImGuiCol_::ImGuiCol_Accent );

	std::vector< spectator_data_t > spectator_data{ };

	if ( !globals.m_local || !interfaces.m_engine->is_in_game( ) || !GET_CONFIG_BOOL( variables.m_visuals.m_spectators_list ) ) {
		if ( !spectator_data.empty( ) )
			spectator_data.clear( );

		return;
	}

	entity_cache.enumerate( [ & ]( c_base_entity* entity ) {
		if ( !entity || entity->is_alive( ) || entity->is_dormant( ) )
			return;

		const auto entity_index = entity->index( );
		const auto entity_team  = entity->team( );

		const auto spectated_player = reinterpret_cast< c_base_entity* >(
			interfaces.m_client_entity_list->get_client_entity_from_handle( entity->get_observer_target_handle( ) ) );
		if ( !spectated_player || !spectated_player->is_alive( ) )
			return;

		const int spectated_player_index = spectated_player->index( );

		player_info_t spectating_info{ }, spectated_info{ };
		interfaces.m_engine->get_player_info( entity_index, &spectating_info );
		interfaces.m_engine->get_player_info( spectated_player_index, &spectated_info );

		if ( spectating_info.m_is_hltv )
			return;

		spectator_data.push_back( { std::format( ( "{} -> {}" ), std::string( spectating_info.m_name ).substr( 0, 24 ),
		                                         std::string( spectated_info.m_name ).substr( 0, 24 ) ),

		                            spectating_info.m_fake_player ? entity_team == 2 /* terrorist */           ? render.m_terrorist_avatar
		                                                            : entity_team == 3 /* counter terrorist */ ? render.m_counter_terrorist_avatar
		                                                                                                       : nullptr
		                                                          : avatar_cache.find( entity_index ),
		                            spectated_player == globals.m_local
		                                ? GET_CONFIG_COLOR( variables.m_visuals.m_spectators_list_text_color_one )
		                                : GET_CONFIG_COLOR( variables.m_visuals.m_spectators_list_text_color_two ) } );
	} );

	constexpr auto background_height = 25.f;

	constexpr auto title_text  = "spectators";
	const auto title_text_size = render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
		render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, title_text );

	ImAnimationHelper spectators_list_ui_animation = ImAnimationHelper( ImHashStr( "hotwheels-spectators-list-ui" ), ImGui::GetIO( ).DeltaTime );
	spectators_list_ui_animation.Update( 2.f, !( spectator_data.empty( ) ) ? 2.f : -2.f );

	// if alpha too low dont run rest of the code
	if ( spectators_list_ui_animation.AnimationData->second < 0.02f )
		return;

	ImGui::PushStyleVar( ImGuiStyleVar_::ImGuiStyleVar_Alpha, spectators_list_ui_animation.AnimationData->second );

	ImGui::SetNextWindowSizeConstraints( ImVec2( title_text_size.x + 20.f, title_text_size.y + 5.f ), ImVec2( FLT_MAX, FLT_MAX ) );
	ImGui::Begin( ( "hotwheels-spectators-list-ui" ), 0,
	              ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar |
	                  ImGuiWindowFlags_::ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_::ImGuiWindowFlags_AlwaysAutoResize );
	{
		const auto window = ImGui::GetCurrentWindow( );

		const auto draw_list = window->DrawList;

		const auto size     = window->Size;
		const auto position = window->Pos;

		[ & ]( ) {
			/* render background */
			ImGui::PushClipRect( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + background_height ), false );
			draw_list->AddRectFilled( ImVec2( position.x, position.y ), ImVec2( position.x + size.x, position.y + background_height ),
			                          ImColor( 25 / 255.f, 25 / 255.f, 25 / 255.f, spectators_list_ui_animation.AnimationData->second ),
			                          ImGui::GetStyle( ).WindowRounding, ImDrawFlags_RoundCornersTop );
			ImGui::PopClipRect( );

			/* render gradient */
			RenderFadedGradientLine( draw_list, ImVec2( position.x, position.y + background_height - 1.f ), ImVec2( size.x, 1.f ),
			                         ImColor( accent_color.Value.x, accent_color.Value.y, accent_color.Value.z ) );

			draw_list->AddText(
				render.m_fonts[ e_font_names::font_name_verdana_bd_11 ], render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize,
				ImVec2( position.x + ( ( size.x - title_text_size.x ) / 2.f ), position.y + ( ( background_height - title_text_size.y ) / 2.f ) ),
				ImColor( 1.f, 1.f, 1.f, spectators_list_ui_animation.AnimationData->second ), title_text );

			/* render outline */
			ImGui::PushClipRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
			                     false );
			draw_list->AddRect( ImVec2( position.x + 1.f, position.y + 1.f ), ImVec2( position.x + size.x - 1.f, position.y + size.y - 1.f ),
			                    ImColor( 50 / 255.f, 50 / 255.f, 50 / 255.f, spectators_list_ui_animation.AnimationData->second ),
			                    ImGui::GetStyle( ).WindowRounding );
			ImGui::PopClipRect( );
		}( );

		ImGui::SetCursorPosY( 30.f );

		for ( const auto& data : spectator_data ) {
			ImGui::Image( data.m_avatar, ImVec2( 14, 14 ), ImVec2( 0, 0 ), ImVec2( 1, 1 ) );
			ImGui::SameLine( );
			ImGui::TextColored( data.m_color.get_vec4( ), data.m_text.c_str( ) );
		}
	}
	ImGui::End( );

	ImGui::PopStyleVar( );
}

bool players_t::on_attach( )
{
	constexpr auto create_material = [ & ]( const char* material_name, const char* shader, const char* material ) {
		c_key_values* key_values = new c_key_values( shader );
		key_values->load_from_buffer( material_name, material );
		return interfaces.m_material_system->create_material( material_name, key_values );
	};

	if ( ( this->m_base_materials[ e_base_material_name::base_material_name_flat ] =
	           interfaces.m_material_system->find_material( "debug/debugdrawflat" ) ) == nullptr )
		return false;

	if ( !( this->m_base_materials[ e_base_material_name::base_material_name_flat ]->is_error_material( ) ) )
		this->m_base_materials[ e_base_material_name::base_material_name_flat ]->increment_reference_count( );

	if ( ( this->m_base_materials[ e_base_material_name::base_material_name_textured ] =
	           interfaces.m_material_system->find_material( "debug/debugambientcube" ) ) == nullptr )
		return false;

	if ( !( this->m_base_materials[ e_base_material_name::base_material_name_textured ]->is_error_material( ) ) )
		this->m_base_materials[ e_base_material_name::base_material_name_textured ]->increment_reference_count( );

	if ( ( this->m_overlay_materials[ e_overlay_material_name::overlay_material_name_snow ] =
	           create_material( "snow-material", "VertexLitGeneric", R"("VertexLitGeneric" {
"$basetexture"                "dev/snowfield"
                "$additive"                    "1"
                "$envmap"                    "editor/cube_vertigo"
                "$envmaptint"                "[0 0.5 0.55]"
                "$envmapfresnel"            "1"
                "$envmapfresnelminmaxexp"   "[0.00005 0.6 6]"
                "$alpha"                    "1"
 
                Proxies
                {
                    TextureScroll
                    {
                        "texturescrollvar"            "$baseTextureTransform"
                        "texturescrollrate"            "0.15"
                        "texturescrollangle"        "270"
                    }
                }
			})" ) ) == nullptr )
		return false;

	if ( !( this->m_overlay_materials[ e_overlay_material_name::overlay_material_name_snow ]->is_error_material( ) ) )
		this->m_overlay_materials[ e_overlay_material_name::overlay_material_name_snow ]->increment_reference_count( );

	return true;
}
