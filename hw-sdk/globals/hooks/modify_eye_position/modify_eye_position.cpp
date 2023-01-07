#include "modify_eye_position.h"

void __fastcall hooks::modify_eye_position::modify_eye_position_detour( sdk::c_anim_state* anim_state, void* edx, math::vec3& input_eye_pos )
{
	static auto calc_view_retadd = g_client_dll.pattern_scan( _( "8B ? ? ? ? ? 30 ? ? ? ? C0 ? 50" ) ).as< std::uintptr_t >( );

	if ( reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ) == calc_view_retadd )
		return;

	return anim_state->modify_eye_position( input_eye_pos );
}
