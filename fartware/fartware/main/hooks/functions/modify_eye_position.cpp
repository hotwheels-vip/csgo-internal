#include "../hooks.h"

void __fastcall n_detoured_functions::modify_eye_position( c_anim_state* anim_state, void* edx, c_vector& input_eye_pos )
{
	static auto original = hooks.modify_eye_position.get_original< void( __thiscall* )( void*, std::reference_wrapper< const c_vector > ) >( );

	auto entity = anim_state->player;

	if ( !entity )
		return original( anim_state, std::cref( input_eye_pos ) );

	static auto calc_view_return_address =
		reinterpret_cast< void* >( memory.m_modules[ e_module_names::client ].find_pattern( ( "8B ? ? ? ? ? 30 ? ? ? ? C0 ? 50 " ) ) );

	if ( _ReturnAddress( ) == calc_view_return_address )
		return;

	if ( globals.m_local && entity != globals.m_local )
		original( anim_state, std::cref( input_eye_pos ) );
}
