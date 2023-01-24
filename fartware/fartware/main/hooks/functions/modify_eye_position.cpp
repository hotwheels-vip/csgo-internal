#include "../hooks.h"

void __fastcall n_detoured_functions::modify_eye_position( c_anim_state* anim_state, void* edx, c_vector& input_eye_pos )
{
	static auto original = hooks.modify_eye_position.get_original< void( __thiscall* )( void*, std::reference_wrapper< const c_vector > ) >( );

	if ( !anim_state )
		return original( anim_state, input_eye_pos );

	static auto calc_view_return_address =
		reinterpret_cast< void* >( memory.m_modules[ e_module_names::client ].find_pattern( ( "8B ? ? ? ? ? 30 ? ? ? ? C0 ? 50 " ) ) );

	if ( _ReturnAddress( ) == calc_view_return_address )
		return;

	return anim_state->modify_eye_position( input_eye_pos );
}
