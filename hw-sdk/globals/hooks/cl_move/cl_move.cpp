#include <string.h>

#include "../../../globals/ctx/ctx.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../../utils/keybinds/keybinds.h"
#include "cl_move.h"

static int charged = 0;

void __cdecl hooks::cl_move::cl_move_detour( float accumulated_extra_samples, bool final_tick )
{
	// auto can_charge = []( ) -> bool {
	//	return false;
	//
	//	bool sure = false;
	//
	//	if ( g_ctx.local )
	//		sure = true;
	//
	//	return sure;
	// };
	//
	// auto release = [ accumulated_extra_samples, final_tick ]( ) -> void {
	//	shifting_tb = true;
	//
	//	for ( int i = 0; i < charged; i++ ) {
	//		hooks::cl_move_hook.call_original< void >( accumulated_extra_samples, final_tick );
	//
	//		send_packet = i == 1;
	//	}
	//
	//	charged = 0;
	//
	//	shifting_tb = false;
	// };
	//
	// if ( charged < 16 && can_charge( ) ) {
	//	charged++;
	//	return;
	// }
	//
	// hooks::cl_move_hook.call_original< void >( accumulated_extra_samples, final_tick );
	//
	// if ( g_input.key_state< input::KEY_DOWN >( VK_HOME ) && charged == 16 )
	//	release( );
}
