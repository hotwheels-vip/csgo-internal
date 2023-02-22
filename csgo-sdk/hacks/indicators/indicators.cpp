#include "indicators.h"

#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../movement/movement.h"

void n_indicators::impl_t::on_paint_traverse( )
{
	const bool on_ground = g_ctx.m_local->get_flags( ) & e_flags::fl_onground;

	if ( GET_VARIABLE( g_variables.m_velocity_indicator, bool ) )
		this->velocity( on_ground );

	if ( GET_VARIABLE( g_variables.m_stamina_indicator, bool ) )
		this->stamina( on_ground );

	if ( GET_VARIABLE( g_variables.m_key_indicators_enable, bool ) )
		this->keybind_indicators( );

	if ( GET_VARIABLE( g_variables.m_sniper_crosshair, bool ) )
		this->sniper_crosshair( );
}

void n_indicators::impl_t::sniper_crosshair( )
{
	if ( !g_ctx.m_local->is_alive( ) )
		return;

	auto weapon_handle = g_ctx.m_local->get_weapon_handle( );
	if ( !weapon_handle )
		return;

	const auto active_weapon =
		reinterpret_cast< c_base_entity* >( g_interfaces.m_client_entity_list->get_client_entity_from_handle( weapon_handle ) );

	if ( !active_weapon )
		return;

	if ( g_ctx.m_local->is_scoped( ) )
		return;

	auto definition_index = active_weapon->get_item_definition_index( );

	if ( g_utilities.is_in< short >( definition_index, { e_item_definition_index::weapon_awp, e_item_definition_index::weapon_ssg08,
	                                                     e_item_definition_index::weapon_scar20, e_item_definition_index::weapon_g3sg1 } ) ) {
		g_render.m_draw_data.emplace_back( e_draw_type::draw_type_rect,
		                                   std::make_any< rect_draw_object_t >( c_vector_2d( g_ctx.m_width / 2 - 1, g_ctx.m_height / 2 - 1 ),
		                                                                        c_vector_2d( g_ctx.m_width / 2 + 1, g_ctx.m_height / 2 + 1 ),
		                                                                        ImColor( 1.f, 1.f, 1.f, 1.f ), ImColor( 0.f, 0.f, 0.f, 0.f ), true,
		                                                                        0.f, 0, 2.f ) );
	}
}

void n_indicators::impl_t::keybind_indicators( )
{
	float offset = 0.f;

	constexpr auto render_indicator = [ & ]( const char* indicator_name, const c_color& color, const bool active ) {
		ImAnimationHelper indicator_animation = ImAnimationHelper( fnv1a::hash( indicator_name ), ImGui::GetIO( ).DeltaTime );
		indicator_animation.Update( 2.f, active ? 2.f : -2.f );

		if ( indicator_animation.AnimationData->second <= 0.f )
			return;

		const auto text_size = g_render.m_fonts[ e_font_names::font_name_indicator_29 ]->CalcTextSizeA(
			g_render.m_fonts[ e_font_names::font_name_indicator_29 ]->FontSize, FLT_MAX, 0.f, indicator_name );

		g_render.m_draw_data.emplace_back(
			e_draw_type::draw_type_text,
			std::make_any< text_draw_object_t >(
				g_render.m_fonts[ e_font_names::font_name_indicator_29 ],
				c_vector_2d( ( g_ctx.m_width - text_size.x ) / 2.f,
		                     g_ctx.m_height - offset - GET_VARIABLE( g_variables.m_key_indicators_position, int ) ),

				indicator_name, color.get_u32( indicator_animation.AnimationData->second ),
				ImColor( 0.f, 0.f, 0.f, color.base< e_color_type::color_type_a >( ) * indicator_animation.AnimationData->second ),
				e_text_flags::text_flag_dropshadow ) );

		offset -= ( text_size.y + 2 ) * indicator_animation.AnimationData->second;
	};

	if ( GET_VARIABLE( g_variables.m_edge_bug, bool ) &&
	     g_config.get< std::vector< bool > >( g_variables.m_key_indicators )[ e_keybind_indicators::key_eb ] )
		render_indicator( "eb",
		                  g_movement.m_edgebug_data.m_will_edgebug ? GET_VARIABLE( g_variables.m_key_color_success, c_color )
		                                                           : GET_VARIABLE( g_variables.m_key_color, c_color ),
		                  g_input.check_input( &GET_VARIABLE( g_variables.m_edge_bug_key, key_bind_t ) ) );

	if ( GET_VARIABLE( g_variables.m_pixel_surf, bool ) &&
	     g_config.get< std::vector< bool > >( g_variables.m_key_indicators )[ e_keybind_indicators::key_ps ] )
		render_indicator( "ps",
		                  g_movement.m_pixelsurf_data.m_predicted_succesful || g_movement.m_pixelsurf_data.m_in_pixel_surf
		                      ? GET_VARIABLE( g_variables.m_key_color_success, c_color )
		                      : GET_VARIABLE( g_variables.m_key_color, c_color ),
		                  g_input.check_input( &GET_VARIABLE( g_variables.m_pixel_surf_key, key_bind_t ) ) );

	if ( GET_VARIABLE( g_variables.m_edge_jump, bool ) &&
	     g_config.get< std::vector< bool > >( g_variables.m_key_indicators )[ e_keybind_indicators::key_ej ] )
		render_indicator( "ej", GET_VARIABLE( g_variables.m_key_color, c_color ),
		                  g_input.check_input( &GET_VARIABLE( g_variables.m_edge_jump_key, key_bind_t ) ) );

	if ( GET_VARIABLE( g_variables.m_long_jump, bool ) &&
	     g_config.get< std::vector< bool > >( g_variables.m_key_indicators )[ e_keybind_indicators::key_lj ] )
		render_indicator( "lj", GET_VARIABLE( g_variables.m_key_color, c_color ),
		                  g_input.check_input( &GET_VARIABLE( g_variables.m_long_jump_key, key_bind_t ) ) );

	// TODO: re-add delayhop

	/* if ( GET_CONFIG_BOOL( g_variables.m_delay_hop ) &&
	     config.get< std::vector< bool > >( g_variables.m_key_indicators )[ e_keybind_indicators::key_dh ] )
	    render_indicator( "dh", GET_VARIABLE( g_variables.m_key_color, c_color ),
	                      g_input.check_input( &GET_VARIABLE( g_variables.m_delay_hop_key ) ) );*/

	if ( GET_VARIABLE( g_variables.m_mini_jump, bool ) &&
	     g_config.get< std::vector< bool > >( g_variables.m_key_indicators )[ e_keybind_indicators::key_mj ] )
		render_indicator( "mj", GET_VARIABLE( g_variables.m_key_color, c_color ),
		                  g_input.check_input( &GET_VARIABLE( g_variables.m_mini_jump_key, key_bind_t ) ) );

	if ( GET_VARIABLE( g_variables.m_jump_bug, bool ) &&
	     g_config.get< std::vector< bool > >( g_variables.m_key_indicators )[ e_keybind_indicators::key_jb ] )
		render_indicator( "jb", GET_VARIABLE( g_variables.m_key_color, c_color ),
		                  g_input.check_input( &GET_VARIABLE( g_variables.m_jump_bug_key, key_bind_t ) ) );
}

void n_indicators::impl_t::velocity( const bool on_ground )
{
	const int velocity = static_cast< int >( std::round( g_ctx.m_local->get_velocity( ).length_2d( ) ) );
	if ( this->m_indicator_data.m_last_on_ground_velocity && !on_ground ) {
		this->m_indicator_data.m_take_off_velocity      = velocity;
		this->m_indicator_data.m_take_off_time_velocity = g_interfaces.m_global_vars_base->m_current_time + 2.f;
	}

	const float alpha = g_ctx.m_local->get_velocity( ).length_2d( ) >= 310.f ? 310.f : g_ctx.m_local->get_velocity( ).length_2d( ) / 310.f;

	const ImColor blended_color = ImColor::Blend( GET_VARIABLE( g_variables.m_velocity_indicator_color1, c_color ).get_u32( ),
	                                              GET_VARIABLE( g_variables.m_velocity_indicator_color2, c_color ).get_u32( ), alpha );

	const ImColor speed_based_color = velocity == this->m_indicator_data.m_last_velocity
	                                      ? GET_VARIABLE( g_variables.m_velocity_indicator_color3, c_color )
	                                            .get_u32( GET_VARIABLE( g_variables.m_velocity_indicator_fade_alpha, bool ) ? alpha : 1.f )
	                                  : velocity < this->m_indicator_data.m_last_velocity
	                                      ? GET_VARIABLE( g_variables.m_velocity_indicator_color4, c_color )
	                                            .get_u32( GET_VARIABLE( g_variables.m_velocity_indicator_fade_alpha, bool ) ? alpha : 1.f )
	                                      : GET_VARIABLE( g_variables.m_velocity_indicator_color5, c_color )
	                                            .get_u32( GET_VARIABLE( g_variables.m_velocity_indicator_fade_alpha, bool ) ? alpha : 1.f );

	const bool should_draw_take_off =
		( !on_ground || ( this->m_indicator_data.m_take_off_time_velocity > g_interfaces.m_global_vars_base->m_current_time ) ) &&
		( GET_VARIABLE( g_variables.m_velocity_indicator_show_pre_speed, bool ) );

	const std::string text =
		std::vformat( should_draw_take_off ? "{:d} ({:d})" : "{:d}", std::make_format_args( velocity, this->m_indicator_data.m_take_off_velocity ) );

	const auto text_size = g_render.m_fonts[ e_font_names::font_name_indicator_29 ]->CalcTextSizeA(
		g_render.m_fonts[ e_font_names::font_name_indicator_29 ]->FontSize, FLT_MAX, 0.f, text.c_str( ) );

	g_render.m_draw_data.emplace_back(
		e_draw_type::draw_type_text,
		std::make_any< text_draw_object_t >(
			g_render.m_fonts[ e_font_names::font_name_indicator_29 ],
			c_vector_2d( ( g_ctx.m_width - text_size.x ) / 2.f,
	                     g_ctx.m_height * ( 1.f - GET_VARIABLE( g_variables.m_velocity_indicator_padding, int ) / ( 100.f ) ) ),
			text,
			GET_VARIABLE( g_variables.m_velocity_indicator_custom_color, bool )
				? ImColor( blended_color.Value.x, blended_color.Value.y, blended_color.Value.z,
	                       blended_color.Value.w * GET_VARIABLE( g_variables.m_velocity_indicator_fade_alpha, bool ) ? alpha : 1.f )
				: ImColor( speed_based_color.Value.x, speed_based_color.Value.y, speed_based_color.Value.z, speed_based_color.Value.w ),
			ImColor( 0.f, 0.f, 0.f, GET_VARIABLE( g_variables.m_velocity_indicator_fade_alpha, bool ) ? alpha : 1.f ),
			e_text_flags::text_flag_dropshadow ) );

	if ( this->m_indicator_data.m_tick_prev_velocity + 5 < g_interfaces.m_global_vars_base->m_tick_count ) {
		this->m_indicator_data.m_last_velocity      = velocity;
		this->m_indicator_data.m_tick_prev_velocity = g_interfaces.m_global_vars_base->m_tick_count;
	}

	this->m_indicator_data.m_last_on_ground_velocity = on_ground;
}

void n_indicators::impl_t::stamina( const bool on_ground )
{
	const float stamina = g_ctx.m_local->get_stamina( );
	if ( this->m_indicator_data.m_last_on_ground_stamina && !on_ground ) {
		this->m_indicator_data.m_take_off_stamina      = stamina;
		this->m_indicator_data.m_take_off_time_stamina = g_interfaces.m_global_vars_base->m_current_time + 2.f;
	}

	const float alpha = stamina >= 40.f ? 40.f : stamina / 40.f;

	const ImColor blended_color = ImColor::Blend( GET_VARIABLE( g_variables.m_stamina_indicator_color1, c_color ).get_u32( ),
	                                              GET_VARIABLE( g_variables.m_stamina_indicator_color2, c_color ).get_u32( ), alpha );

	const bool should_draw_take_off =
		( !on_ground || ( this->m_indicator_data.m_take_off_time_stamina > g_interfaces.m_global_vars_base->m_current_time ) ) &&
		( GET_VARIABLE( g_variables.m_stamina_indicator_show_pre_speed, bool ) );

	const std::string text = std::vformat( should_draw_take_off ? "{:.1f} ({:.1f})" : "{:.1f}",
	                                       std::make_format_args( stamina, this->m_indicator_data.m_take_off_stamina ) );

	const auto text_size = g_render.m_fonts[ e_font_names::font_name_indicator_29 ]->CalcTextSizeA(
		g_render.m_fonts[ e_font_names::font_name_indicator_29 ]->FontSize, FLT_MAX, 0.f, text.c_str( ) );

	g_render.m_draw_data.emplace_back(
		e_draw_type::draw_type_text,
		std::make_any< text_draw_object_t >(
			g_render.m_fonts[ e_font_names::font_name_indicator_29 ],
			c_vector_2d( ( g_ctx.m_width - text_size.x ) / 2.f,
	                     g_ctx.m_height * ( 1.f - GET_VARIABLE( g_variables.m_stamina_indicator_padding, int ) / ( 100.f ) ) ),
			text,
			ImColor( blended_color.Value.x, blended_color.Value.y, blended_color.Value.z,
	                 blended_color.Value.w * GET_VARIABLE( g_variables.m_stamina_indicator_fade_alpha, bool ) ? alpha : 1.f ),
			ImColor( 0.f, 0.f, 0.f, GET_VARIABLE( g_variables.m_stamina_indicator_fade_alpha, bool ) ? alpha : 1.f ),
			e_text_flags::text_flag_dropshadow ) );

	if ( this->m_indicator_data.m_tick_prev_stamina + 5 < g_interfaces.m_global_vars_base->m_tick_count ) {
		this->m_indicator_data.m_last_stamina      = stamina;
		this->m_indicator_data.m_tick_prev_stamina = g_interfaces.m_global_vars_base->m_tick_count;
	}

	this->m_indicator_data.m_last_on_ground_stamina = on_ground;
}
