#include "indicators.h"
#include "../../../../dependencies/imgui/imgui.h"
#include "../../../convars/convars.h"
#include "../../../includes.h"
#include "../../../source_engine/enumerations/e_flags.h"
#include "../../../source_engine/enumerations/e_move_types.h"
#include "../../prediction/prediction.h"
#include "../movement.h"

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

	if ( !globals.m_local || !globals.m_local->is_alive( ) ) {
		reset_data( );
		return;
	}

	const bool on_ground = ( globals.m_local->flags( ) & e_flags::fl_onground );

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

		const std::string text =
			 std::vformat( should_draw_take_off ? "{:d} ({:d})" : "{:d}", std::make_format_args( velocity, take_off_velocity ) );

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

		/* TODO ~ change indicator colors when predicted or what ever u get it like blarity. . , .. . . . LOL XD dxdxdx :3 Sussuy Among Su */

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
}
