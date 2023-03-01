#include "../../game/sdk/includes/includes.h"
#include "../../globals/globals.h"
#include "../../globals/interfaces/interfaces.h"
#include "../../utilities/console/console.h"
#include "../hooks.h"
#include <fstream>

/* can be used to a world material changer */
c_material* __fastcall n_detoured_functions::find_material( void* ecx, void* edx, const char* material_name, const char* texture_group, bool complain,
                                                            const char* complain_prefix )
{
	static auto original = g_hooks.m_find_material.get_original< decltype( &n_detoured_functions::find_material ) >( );

	// if ( strstr( material_name, "dev/glow_color" ) )
	// 	g_console.print( material_name );

	return original( ecx, edx, material_name, texture_group, complain, complain_prefix );
}
