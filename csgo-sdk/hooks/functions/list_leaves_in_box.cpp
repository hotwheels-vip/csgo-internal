#include "../../game/sdk/includes/includes.h"
#include "../../game/sdk/structs/renderable_info_t.h"
#include "../../globals/includes/includes.h"
#include "../hooks.h"

int __fastcall n_detoured_functions::list_leaves_in_box( void* ecx, void* edx, const c_vector& mins, const c_vector& maxs, unsigned short* list,
                                                         int list_max )
{
	static auto return_address =
		reinterpret_cast< std::uintptr_t >( g_modules[ CLIENT_DLL ].find_pattern( "89 44 24 ? EB ? C7 44 24 ? ? ? ? ? 8B 45" ) );
	static auto original = g_hooks.m_list_leaves_in_box.get_original< decltype( &n_detoured_functions::list_leaves_in_box ) >( );

	// TODO - replace with enable player chams variable
	// only disable model oclusion if any chams are on.

	if ( !GET_VARIABLE( g_variables.m_player_lag_chams, bool ) || reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ) != return_address )
		return original( ecx, edx, mins, maxs, list, list_max );

	auto info = *reinterpret_cast< renderable_info_t** >( reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14 );

	if ( !info || !info->m_renderable )
		return original( ecx, edx, mins, maxs, list, list_max );

	auto base_entity = info->m_renderable->get_client_unknown( )->get_base_entity( );

	if ( !base_entity || !base_entity->is_player( ) )
		return original( ecx, edx, mins, maxs, list, list_max );

	info->m_flags &= ~0x100;
	info->m_flags_alt |= 0xC0;

	static const c_vector new_mins = { MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT };
	static const c_vector new_maxs = { MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT };

	return original( ecx, edx, new_mins, new_maxs, list, list_max );
}
