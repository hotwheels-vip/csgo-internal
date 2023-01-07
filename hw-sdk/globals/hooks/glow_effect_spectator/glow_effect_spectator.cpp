#include "glow_effect_spectator.h"

bool __cdecl hooks::glow_effect_spectator::glow_effect_spectator_detour( sdk::c_cs_player* player, sdk::c_cs_player* local, sdk::glow_style& style,
                                                                         math::vec3& glow_color, float& alpha_start, float& alpha, float& time_start,
                                                                         float& time_target, bool& animate )
{
	/*glow_color = { 1.f, 0.f, 0.f };
	alpha /= 2.f;
	return player != local && player->is_enemy( local );*/

	return glow_effect_spectator_hook.call_original< bool >( player, local, style, glow_color, alpha_start, alpha, time_start, time_target, animate );
}
