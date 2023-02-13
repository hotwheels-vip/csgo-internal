#include "indicators.h"

#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

void n_indicators::impl_t::on_paint_traverse( )
{
	const bool on_ground = g_ctx.m_local->get_flags( ) & e_flags::fl_onground;

	if ( GET_VARIABLE( g_variables.m_velocity_indicator, bool ) )
		this->velocity( on_ground );

	if ( GET_VARIABLE( g_variables.m_stamina_indicator, bool ) )
		this->stamina( on_ground );
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
		this->m_indicator_data.m_last_stamina = stamina;
		this->m_indicator_data.m_tick_prev_stamina = g_interfaces.m_global_vars_base->m_tick_count;
	}

	this->m_indicator_data.m_last_on_ground_stamina = on_ground;
}