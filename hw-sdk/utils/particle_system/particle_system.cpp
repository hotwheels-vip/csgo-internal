#include "particle_system.h"

using is_effect_chached_type = bool( __thiscall* )( void*, const char* );
static is_effect_chached_type is_effect_cached{ };

using find_string_index_type = void( __thiscall* )( void*, int*, const char* );
static find_string_index_type find_string_index;

using set_control_point_type = void( __thiscall* )( sdk::c_effect*, int, math::vec3* );
static set_control_point_type set_control_point;

static void* particle_system_manager;

inline void* play_effect{ };

void* particle_system::impl::get_particle_system_index( const char* system_name )
{
	void* system{ };

	if ( is_effect_cached( particle_system_manager, system_name ) ) {
		int index{ };

		find_string_index( reinterpret_cast< void* >( reinterpret_cast< std::uintptr_t >( particle_system_manager ) + 0x14 ), &index, system_name );

		short casted_index = static_cast< short >( index );

		if ( casted_index == -1 )
			return 0;

		system = *reinterpret_cast< void** >( *reinterpret_cast< std::uintptr_t* >( particle_system_manager ) + ( 0x4 * casted_index ) );
	}

	return system;
}

__declspec( naked ) sdk::c_effect* particle_system::impl::create_effect( void* system, math::vec3* origin )
{
	__asm
	{
		push ebp
		mov ebp, esp
		push 0x0FFFFFFFF
		push 0
		mov edx, system
		push 0
		push origin
		call play_effect
		add esp, 0x10
		pop ebp
		retn
	}
}

void particle_system::impl::run( math::vec3 origin, const char* effect_name )
{
	if ( !g_ctx.local )
		return; // shew

	void* system = get_particle_system_index( effect_name );
	auto effect  = create_effect( system, &origin );
	__asm add esp, 8;

	effect->origin( ) = origin;

	set_control_point( effect, 0, &origin );
	set_control_point( effect, 1, &origin );
}

bool particle_system::impl::init( )
{
	play_effect             = g_client_dll.pattern_scan( _( "55 8B EC 83 EC ? 53 56 8B F2 89 75" ) ).as< void* >( );
	is_effect_cached        = g_client_dll.pattern_scan( _( "55 8B EC 83 EC ? 53 56 57 8B 7D ? 8D 59 ? 85 FF" ) ).as< is_effect_chached_type >( );
	find_string_index       = g_client_dll.pattern_scan( _( "55 8B EC 83 EC ? 53 8B 5D ? 57 8B F9 89 7D ? 85 DB" ) ).as< find_string_index_type >( );
	particle_system_manager = **g_client_dll.pattern_scan( _( "55 8B EC 51 56 8B 35 ? ? ? ? 8B CE" ) ).add( 0x7 ).as< void*** >( );
	set_control_point       = g_client_dll.pattern_scan( _( "55 8B EC 53 8B 5D ? 56 8B F1 F6 86" ) ).as< set_control_point_type >( );

	return true;
}
