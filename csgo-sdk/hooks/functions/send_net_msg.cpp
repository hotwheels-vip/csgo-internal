#include "../../game/sdk/classes/c_net_message.h"
#include "../hooks.h"

bool __fastcall n_detoured_functions::send_net_msg( void* ecx, void* edx, c_net_message* message, bool force_reliable, bool voice )
{
	static auto original = g_hooks.m_send_net_msg.get_original< decltype( &n_detoured_functions::send_net_msg ) >( );

	if ( message->get_type( ) == 14 /* FileCRCCheck */ )
		return false;

	return original( ecx, edx, message, force_reliable, voice );
}