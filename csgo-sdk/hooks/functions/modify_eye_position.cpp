#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

void __fastcall n_detoured_functions::modify_eye_position( c_animation_state* anim_state, void* edx, c_vector& input_eye_pos )
{
	static auto original = g_hooks.m_modify_eye_position.get_original< void( __thiscall* )( void*, std::reference_wrapper< const c_vector > ) >( );

	if ( !g_ctx.m_local || !anim_state )
		return original( anim_state, input_eye_pos );

	static auto calc_view_return_address = reinterpret_cast< void* >( g_modules[ CLIENT_DLL ].find_pattern( "8B ? ? ? ? ? 30 ? ? ? ? C0 ? 50 " ) );

	if ( _ReturnAddress( ) == calc_view_return_address )
		return;

	return anim_state->m_entity->modify_eye_position( anim_state, &input_eye_pos );
}
