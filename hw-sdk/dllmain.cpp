#include "globals/includes/includes.h"

BOOL APIENTRY DllMain( HMODULE hmodule, DWORD ul_reason_for_call, LPVOID lpReserved )
{
	switch ( ul_reason_for_call ) {
	case DLL_PROCESS_ATTACH:
		LI_FN( DisableThreadLibraryCalls )( hmodule );

		return utils::cheat_create_thread( hotwheels::init, hmodule );
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
};
