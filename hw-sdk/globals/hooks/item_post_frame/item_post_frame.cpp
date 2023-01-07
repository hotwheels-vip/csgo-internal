#include <string.h>

#include "../../../hacks/prediction/prediction.h"
#include "../../../utils/convars/convars.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../../utils/keybinds/keybinds.h"
#include "../../ctx/ctx.h"
#include "item_post_frame.h"

void __fastcall hooks::item_post_frame::item_post_frame_detour( void* self, void* edx )
{
	static auto prediction_return_address = g_client_dll.pattern_scan( _( "F6 87 ? ? ? ? ? 74 ? C7 87 ? ? ? ? ? ? ? ? 83 BF" ) ).as< void* >( );

	if ( _ReturnAddress( ) == prediction_return_address && g_ctx.running_post_think )
		return;

	return item_post_frame_hook.call_original< void >( self, edx );
}
