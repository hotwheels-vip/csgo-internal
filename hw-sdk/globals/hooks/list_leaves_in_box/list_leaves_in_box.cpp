#include "list_leaves_in_box.h"

int __fastcall hooks::list_leaves_in_box::list_leaves_in_box_detour( void* ecx, void* edx, math::vec3& mins, math::vec3& maxs, unsigned short* list,
                                                                     int list_max )
{
	static auto return_address = g_client_dll.pattern_scan( _( "89 44 24 ? EB ? C7 44 24 ? ? ? ? ? 8B 45" ) ).as< std::uintptr_t >( );

	if ( reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ) != return_address )
		return list_leaves_in_box_hook.call_original< int >( ecx, edx, mins, maxs, list, list_max );

	auto info = *reinterpret_cast< sdk::renderable_info** >( reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14 );

	if ( !info || !info->renderable )
		return list_leaves_in_box_hook.call_original< int >( ecx, edx, mins, maxs, list, list_max );

	auto base_entity = info->renderable->get_i_client_unkown( )->get_base_entity( );

	if ( !base_entity || !base_entity->is_player( ) )
		return list_leaves_in_box_hook.call_original< int >( ecx, edx, mins, maxs, list, list_max );

	info->flags &= ~0x100;
	info->flags_alt |= 0xC0;

	static const math::vec3 new_mins = { -16384.f, -16384.f, -16384.f };
	static const math::vec3 new_maxs = { 16384.f, 16384.f, 16384.f };

	return list_leaves_in_box_hook.call_original< int >( ecx, edx, new_mins, new_maxs, list, list_max );
}
