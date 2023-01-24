#include "../../menu/menu.h"
#include "../hooks.h"

bool __cdecl n_detoured_functions::glow_effect_spectator( c_base_entity* player, c_base_entity* local, glow_style_t& style, c_vector& glow_color,
                                                          float& alpha_start, float& alpha, float& time_start, float& time_target, bool& animate )
{
	auto m_color = GET_CONFIG_COLOR( variables.m_visuals.m_glow_color );

	glow_color = { m_color.base< color_type_r >( ), m_color.base< color_type_g >( ), m_color.base< color_type_b >( ) };
	alpha      = m_color.base< color_type_a >( );
	return player != local && player->team( ) != local->team( );
}
