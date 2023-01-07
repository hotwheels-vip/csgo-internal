#include "calculate_view.h"
#include "../../ctx/ctx.h"

void __fastcall hooks::calculate_view::calculate_view_detour( std::uintptr_t ecx, std::uintptr_t edx, math::vec3& eye_origin, math::vec3& eye_angles,
                                                              float& zn, float& zf, float& fov )
{
	auto player = reinterpret_cast< sdk::c_cs_player* >( ecx );

	if ( player != g_ctx.local )
		return hooks::calculate_view_hook.call_original< void >( ecx, edx, eye_origin, eye_angles, zn, zf, fov );

	const bool new_anim_state = player->get< bool >( 0x9B14 );

	player->set( 0x9B14, false );

	hooks::calculate_view_hook.call_original< void >( ecx, edx, eye_origin, eye_angles, zn, zf, fov );

	player->set( 0x9B14, new_anim_state );
}
