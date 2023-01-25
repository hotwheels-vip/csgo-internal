#include "indicators.h"
#include "../../../../dependencies/imgui/imgui.h"
#include "../../../convars/convars.h"
#include "../../../includes.h"
#include "../../../logging/logging.h"
#include "../../../source_engine/enumerations/e_flags.h"
#include "../../../source_engine/enumerations/e_item_definition_index.h"
#include "../../../source_engine/enumerations/e_move_types.h"
#include "../../prediction/prediction.h"
#include "../movement.h"

void indicators_t::on_create_move_pre( )
{
	// we only check these cause we run before our sanity checks in create move.
	// we also run this function while player isnt alive.

	if ( !globals.m_local || !globals.m_cmd || !interfaces.m_engine->is_in_game( ) ||
	     globals.m_local->get_observer_mode( ) == c_base_entity::obs_mode_t::obs_mode_deathcam ) {
		if ( !velocity_history.empty( ) )
			velocity_history.clear( );
		return;
	}

	const int speed = static_cast< int >( std::clamp( std::roundf( globals.m_local->velocity( ).length_2d( ) ), 0.f, 450.f ) );

	velocity_data m_data{ };

	m_data.m_velocity   = speed;
	m_data.m_jumpbugged = indicators.detection.m_jumpbugged;
	m_data.m_edgebugged = indicators.detection.m_edgebugged;

	velocity_history.push_back( m_data );
	while ( velocity_history.size( ) > 300u )
		velocity_history.erase( velocity_history.begin( ) );
}

void indicators_t::on_create_move_post( )
{
	// detections
	[ & ]( ) {
		if ( utilities.is_in< int >( globals.m_local->move_type( ), invalid_move_types ) ||
		     utilities.is_in< int >( prediction.m_data.m_movetype, invalid_move_types ) || globals.m_local->flags( ) & fl_onground ||
		     prediction.m_data.m_flags & fl_onground ) {
			indicators.detection.reset( );
			return;
		}

		// edgebug detection
		[]( ) {
			if ( prediction.m_data.m_velocity.m_z > 0 || static_cast< int >( roundf( globals.m_local->velocity( ).m_z ) ) > 0.f ||
			     round( globals.m_local->velocity( ).m_z ) == 0 ) {
				indicators.detection.m_edgebugged = false;
				return;
			}

			if ( prediction.m_data.m_velocity.m_z < -6.f && globals.m_local->velocity( ).m_z > prediction.m_data.m_velocity.m_z &&
			     globals.m_local->velocity( ).m_z < -6.f && prediction.m_data.m_origin.m_z > globals.m_local->abs_origin( ).m_z ) {
				const float before_detection_pred = globals.m_local->velocity( ).m_z;
				const auto gravity                = convars.find( fnv1a::hash_const( "sv_gravity" ) )->get_float( );

				if ( std::floor( prediction.m_data.m_velocity.m_z ) < -7 && std::floor( before_detection_pred ) == -7 &&
				     globals.m_local->velocity( ).length_2d( ) >= prediction.m_data.m_velocity.length_2d( ) ) {
					indicators.detection.m_edgebugged = true;
				} else {
					prediction.begin( globals.m_cmd );
					prediction.end( );

					float own_prediction   = roundf( ( -gravity * memory.m_globals->m_interval_per_tick ) + before_detection_pred );
					float rounded_velocity = roundf( globals.m_local->velocity( ).m_z );

					if ( own_prediction == rounded_velocity ) {
						indicators.detection.m_edgebugged = true;
					} else {
						indicators.detection.m_edgebugged = false;
					}

					prediction.restore_entity_to_predicted_frame( interfaces.m_prediction->m_commands_predicted - 1 );

					prediction.begin( globals.m_cmd );
					prediction.end( );
				}
			} else
				indicators.detection.m_edgebugged = false;
		}( );

		[]( ) {
			// jumpbug detection
			if ( globals.m_local->velocity( ).m_z > prediction.m_data.m_velocity.m_z && !indicators.detection.m_edgebugged &&
			     !movement.m_pixelsurf_data.m_in_pixel_surf ) {
				indicators.detection.m_jumpbugged = true;
			} else
				indicators.detection.m_jumpbugged = false;
		}( );
	}( );
}

void indicators_t::on_paint_traverse( )
{
	if ( !render.m_initialised )
		return;

	/* TODO ~ move to a struct of somesort */
	static int last_velocity            = 0;
	static int tick_prev_velocity       = 0;
	static int take_off_velocity        = 0;
	static float take_off_time_velocity = 0.f;
	static bool last_on_ground_velocity = false;

	static float last_stamina          = 0.f;
	static float tick_prev_stamina     = 0.f;
	static float take_off_stamina      = 0.f;
	static float take_off_time_stamina = 0.f;
	static bool last_on_ground_stamina = false;

	constexpr auto reset_data = [ & ]( ) {
		last_velocity           = 0;
		tick_prev_velocity      = 0;
		take_off_velocity       = 0;
		take_off_time_velocity  = 0.f;
		last_on_ground_velocity = false;

		last_stamina           = 0.f;
		tick_prev_stamina      = 0.f;
		take_off_stamina       = 0.f;
		take_off_time_stamina  = 0.f;
		last_on_ground_stamina = false;
	};

	if ( !globals.m_local || !interfaces.m_engine->is_in_game( ) ) {
		reset_data( );
		return;
	}

	const bool on_ground = ( globals.m_local->flags( ) & e_flags::fl_onground );
	// velocity graph
	[ & ]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_graph ) )
			return;

		// dont draw graph if not enough history
		if ( velocity_history.size( ) < 5 )
			return;

		// temporary, please dont make it SUPER customizable, no need for 6 different sliders.
		const int graph_width  = globals.m_display_size.x / 5.5;
		const int graph_height = 70;
		const int graph_center = graph_width / 2;
		const int graph_x      = globals.m_display_size.x / 2;
		const int graph_ratio  = globals.m_display_size.y / 1.2;

		for ( std::size_t data{ 0u }; data != velocity_history.size( ) - 1; data++ ) {
			if ( ( data + 1 ) > velocity_history.size( ) )
				continue;

			const auto current = velocity_history[ data ], next = velocity_history[ data + 1 ];

			const bool edgebugged = current.m_edgebugged, jumpbugged = current.m_jumpbugged;

			const int cur_x  = ( data / 300.f ) * graph_width + graph_x - graph_center,
					  last_x = ( ( data - 1 ) / 300.f ) * graph_width + graph_x - graph_center,
					  cur_y  = std::sqrt( ( ( ( next.m_velocity / 400.f ) * graph_height ) - graph_ratio ) *
			                              ( ( ( next.m_velocity / 400.f ) * graph_height ) - graph_ratio ) ),
					  last_y = std::sqrt( ( ( ( current.m_velocity / 400.f ) * graph_height ) - graph_ratio ) *
			                              ( ( ( current.m_velocity / 400.f ) * graph_height ) - graph_ratio ) );

			c_unsigned_char_color graph_color = c_unsigned_char_color( 255.f, 255.f, 255.f, 255.f );

			for ( int i = 0; i < 45; i++ ) {
				if ( data == i )
					graph_color.a = 5 * i;
				if ( data == ( velocity_history.size( ) - i ) )
					graph_color.a = 5 * i;
			}

			if ( data != 0 ) {
				// render main line

				ImColor cur_color         = ImColor( 1.f, 1.f, 1.f, std::clamp( ( float )graph_color.a, 0.f, 255.f ) / 255.f );
				ImColor cur_color_outline = ImColor( 0.f, 0.f, 0.f, std::clamp( ( float )graph_color.a, 0.f, 255.f ) / 255.f );

				render.m_draw_data.emplace_back( e_draw_type::draw_type_line,
				                                 std::make_any< line_object_t >( ImVec2( last_x, last_y ), ImVec2( cur_x, cur_y ), cur_color, 1.f ) );

				if ( jumpbugged )
					render.m_draw_data.emplace_back(
						e_draw_type::draw_type_text,
						std::make_any< text_draw_object_t >( render.m_fonts[ e_font_names::font_name_verdana_11 ], c_vector_2d( last_x, last_y + 4 ),
					                                         "jb", cur_color, cur_color_outline, e_text_render_flags::text_render_flag_dropshadow ) );

				if ( edgebugged )
					render.m_draw_data.emplace_back(
						e_draw_type::draw_type_text,
						std::make_any< text_draw_object_t >( render.m_fonts[ e_font_names::font_name_verdana_11 ], c_vector_2d( last_x, last_y + 4 ),
					                                         "eb", cur_color, cur_color_outline, e_text_render_flags::text_render_flag_dropshadow ) );
			}
		}
	}( );
	// velocity indicator
	[ & ]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator ) )
			return;

		const int velocity = static_cast< int >( std::round( globals.m_local->velocity( ).length_2d( ) ) );
		if ( last_on_ground_velocity && !on_ground ) {
			take_off_velocity      = velocity;
			take_off_time_velocity = memory.m_globals->m_current_time + 2.f;
		}

		const float alpha = globals.m_local->velocity( ).length_2d( ) >= 310.f ? 310.f : globals.m_local->velocity( ).length_2d( ) / 310.f;

		const ImColor blended_color =
			ImColor::Blend( GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color1 ).get_u32( ),
		                    GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color2 ).get_u32( ), alpha );

		const ImColor speed_based_color =
			velocity == last_velocity
				? GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color3 )
					  .get_u32( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_fade_alpha ) ? alpha : 1.f )
			: velocity < last_velocity
				? GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color4 )
					  .get_u32( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_fade_alpha ) ? alpha : 1.f )
				: GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_velocity_indicator_color5 )
					  .get_u32( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_fade_alpha ) ? alpha : 1.f );

		const bool should_draw_take_off = ( !on_ground || ( take_off_time_velocity > memory.m_globals->m_current_time ) ) &&
		                                  ( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_show_pre_speed ) );

		const std::string text = std::vformat( should_draw_take_off ? "{:d} ({:d})" : "{:d}", std::make_format_args( velocity, take_off_velocity ) );

		const auto text_size = render.m_fonts[ e_font_names::font_name_indicator_29 ]->CalcTextSizeA(
			render.m_fonts[ e_font_names::font_name_indicator_29 ]->FontSize, FLT_MAX, 0.f, text.c_str( ) );

		render.m_draw_data.emplace_back(
			e_draw_type::draw_type_text,
			std::make_any< text_draw_object_t >(
				render.m_fonts[ e_font_names::font_name_indicator_29 ],
				c_vector_2d( ( globals.m_display_size.x - text_size.x ) / 2.f,
		                     globals.m_display_size.y - GET_CONFIG_INT( variables.m_movement.m_indicators.m_velocity_indicator_position ) ),
				text,
				GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_custom_color )
					? ImColor( blended_color.Value.x, blended_color.Value.y, blended_color.Value.z,
		                       blended_color.Value.w * GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_fade_alpha ) ? alpha
		                                                                                                                                    : 1.f )
					: ImColor( speed_based_color.Value.x, speed_based_color.Value.y, speed_based_color.Value.z, speed_based_color.Value.w ),
				ImColor( 0.f, 0.f, 0.f, GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_velocity_indicator_fade_alpha ) ? alpha : 1.f ),
				e_text_render_flags::text_render_flag_dropshadow ) );

		if ( tick_prev_velocity + 5 < memory.m_globals->m_tick_count ) {
			last_velocity      = velocity;
			tick_prev_velocity = memory.m_globals->m_tick_count;
		}

		last_on_ground_velocity = on_ground;
	}( );

	// stamina indicator
	[ & ]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator ) )
			return;

		const float stamina = globals.m_local->stamina( );
		if ( last_on_ground_stamina && !on_ground ) {
			take_off_stamina      = stamina;
			take_off_time_stamina = memory.m_globals->m_current_time + 2.f;
		}

		const float alpha = stamina >= 40.f ? 40.f : stamina / 40.f;

		const ImColor blended_color =
			ImColor::Blend( GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_stamina_indicator_color1 ).get_u32( ),
		                    GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_stamina_indicator_color2 ).get_u32( ), alpha );

		const bool should_draw_take_off = ( !on_ground || ( take_off_time_stamina > memory.m_globals->m_current_time ) ) &&
		                                  ( GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator_show_pre_speed ) );

		const std::string text =
			std::vformat( should_draw_take_off ? "{:.1f} ({:.1f})" : "{:.1f}", std::make_format_args( stamina, take_off_stamina ) );

		const auto text_size = render.m_fonts[ e_font_names::font_name_indicator_29 ]->CalcTextSizeA(
			render.m_fonts[ e_font_names::font_name_indicator_29 ]->FontSize, FLT_MAX, 0.f, text.c_str( ) );

		render.m_draw_data.emplace_back(
			e_draw_type::draw_type_text,
			std::make_any< text_draw_object_t >(
				render.m_fonts[ e_font_names::font_name_indicator_29 ],
				c_vector_2d( ( globals.m_display_size.x - text_size.x ) / 2.f,
		                     globals.m_display_size.y - GET_CONFIG_INT( variables.m_movement.m_indicators.m_stamina_indicator_position ) ),
				text,
				ImColor( blended_color.Value.x, blended_color.Value.y, blended_color.Value.z,
		                 blended_color.Value.w * GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator_fade_alpha ) ? alpha : 1.f ),
				ImColor( 0.f, 0.f, 0.f, GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_stamina_indicator_fade_alpha ) ? alpha : 1.f ),
				e_text_render_flags::text_render_flag_dropshadow ) );

		if ( tick_prev_stamina + 5 < memory.m_globals->m_tick_count ) {
			last_stamina      = stamina;
			tick_prev_stamina = memory.m_globals->m_tick_count;
		}

		last_on_ground_stamina = on_ground;
	}( );

	// movement indicators
	[ & ]( ) {
		if ( !GET_CONFIG_BOOL( variables.m_movement.m_indicators.m_sub_indicators_enable ) )
			return;

		float offset = 0.f;

		const auto render_indicator = [ & ]( const char* indicator_name, const c_color& color, bool active ) {
			ImAnimationHelper indicator_animation = ImAnimationHelper( fnv1a::hash( indicator_name ), ImGui::GetIO( ).DeltaTime );
			indicator_animation.Update( 2.f, active ? 2.f : -2.f );

			if ( indicator_animation.AnimationData->second <= 0.f )
				return;

			const auto text_size = render.m_fonts[ e_font_names::font_name_indicator_29 ]->CalcTextSizeA(
				render.m_fonts[ e_font_names::font_name_indicator_29 ]->FontSize, FLT_MAX, 0.f, indicator_name );

			render.m_draw_data.emplace_back(
				e_draw_type::draw_type_text,
				std::make_any< text_draw_object_t >(
					render.m_fonts[ e_font_names::font_name_indicator_29 ],
					c_vector_2d( ( globals.m_display_size.x - text_size.x ) / 2.f,
			                     globals.m_display_size.y - offset - GET_CONFIG_INT( variables.m_movement.m_indicators.m_sub_indicators_position ) ),
					indicator_name, color.get_u32( indicator_animation.AnimationData->second ),
					ImColor( 0.f, 0.f, 0.f, color.base< e_color_type::color_type_a >( ) * indicator_animation.AnimationData->second ),
					e_text_render_flags::text_render_flag_dropshadow ) );

			offset -= ( text_size.y + 2 ) * indicator_animation.AnimationData->second;
		};

		if ( GET_CONFIG_BOOL( variables.m_movement.m_edge_bug ) &&
		     config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators )[ e_keybind_indicators::key_eb ] )
			render_indicator( "eb",
			                  movement.m_edgebug_data.m_will_edgebug ? GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color_success )
			                                                         : GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
			                  input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_edge_bug_key ) ) );

		if ( GET_CONFIG_BOOL( variables.m_movement.m_pixel_surf ) &&
		     config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators )[ e_keybind_indicators::key_ps ] )
			render_indicator( "ps",
			                  movement.m_pixelsurf_data.m_predicted_succesful || movement.m_pixelsurf_data.m_in_pixel_surf
			                      ? GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color_success )
			                      : GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
			                  input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_pixel_surf_key ) ) );

		// only render ej text when not ljing, else just render lj
		if ( GET_CONFIG_BOOL( variables.m_movement.m_edge_jump ) && !input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_long_jump_key ) ) &&
		     config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators )[ e_keybind_indicators::key_ej ] )
			render_indicator( "ej", GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
			                  input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_edge_jump_key ) ) );

		if ( GET_CONFIG_BOOL( variables.m_movement.m_long_jump ) &&
		     config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators )[ e_keybind_indicators::key_lj ] )
			render_indicator( "lj", GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
			                  input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_long_jump_key ) ) );

		if ( GET_CONFIG_BOOL( variables.m_movement.m_delay_hop ) &&
		     config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators )[ e_keybind_indicators::key_dh ] )
			render_indicator( "dh", GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
			                  input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_delay_hop_key ) ) );

		if ( GET_CONFIG_BOOL( variables.m_movement.m_mini_jump ) &&
		     config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators )[ e_keybind_indicators::key_mj ] )
			render_indicator( "mj", GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
			                  input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_mini_jump_key ) ) );

		if ( GET_CONFIG_BOOL( variables.m_movement.m_jump_bug ) &&
		     config.get< std::vector< bool > >( variables.m_movement.m_indicators.m_sub_indicators )[ e_keybind_indicators::key_jb ] )
			render_indicator( "jb", GET_CONFIG_COLOR( variables.m_movement.m_indicators.m_keybind_color ),
			                  input.check_input( &GET_CONFIG_BIND( variables.m_movement.m_jump_bug_key ) ) );
	}( );

	// sniper crosshair
	[ & ]( const bool can_draw_sniper_xhair ) {
		if ( !can_draw_sniper_xhair )
			return;

		if ( !globals.m_local->is_alive( ) )
			return;

		auto weapon_handle = globals.m_local->active_weapon_handle( );
		if ( !weapon_handle )
			return;

		const auto active_weapon =
			reinterpret_cast< c_base_entity* >( interfaces.m_client_entity_list->get_client_entity_from_handle( weapon_handle ) );

		if ( !active_weapon )
			return;

		auto definition_index = active_weapon->item_definition_index( );

		if ( utilities.is_in< short >( definition_index, { e_item_definition_index::weapon_awp, e_item_definition_index::weapon_ssg08,
		                                                   e_item_definition_index::weapon_scar20, e_item_definition_index::weapon_g3sg1 } ) ) {
			render.m_draw_data.emplace_back(
				e_draw_type::draw_type_rect,
				std::make_any< rect_draw_object_t >( c_vector_2d( globals.m_display_size.x / 2 - 1, globals.m_display_size.y / 2 - 1 ),
			                                         c_vector_2d( globals.m_display_size.x / 2 + 1, globals.m_display_size.y / 2 + 1 ),
			                                         ImColor( 1.f, 1.f, 1.f, 1.f ), ImColor( 0.f, 0.f, 0.f, 0.f ) ) );
		}
	}( GET_CONFIG_BOOL( variables.m_visuals.m_sniper_crosshair ) );
}

void indicators_t::detection_data::reset( )
{
	indicators.detection.m_edgebugged = false;
	indicators.detection.m_jumpbugged = false;
}
