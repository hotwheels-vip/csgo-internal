#include "../hooks.h"

bool __cdecl n_detoured_functions::glow_effect_spectator( c_base_entity* player, c_base_entity* local, e_glow_style style, c_vector& glow_color,
                                                          float& alpha_start, float& alpha, float& time_start, float& time_target, bool& animate )
{
	const bool should_render = GET_CONFIG_BOOL( variables.m_visuals.m_glow_enable ) && player != local && player->team( ) != local->team( );

	// TODO add functions to check if player is visible and change colors/style depending if so

	auto color = GET_CONFIG_COLOR( variables.m_visuals.m_glow_vis );

	glow_color = { color.base< e_color_type::color_type_r >( ) / 255.f, color.base< e_color_type::color_type_g >( ) / 255.f,
		           color.base< e_color_type::color_type_b >( ) / 255.f };

	alpha = color.base< e_color_type::color_type_a >( ) / 255.f;

	return should_render;
}
