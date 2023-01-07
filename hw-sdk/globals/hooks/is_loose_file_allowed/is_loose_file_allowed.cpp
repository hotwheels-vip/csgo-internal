#include "is_loose_file_allowed.h"

bool __fastcall hooks::is_loose_file_allowed::is_loose_file_allowed_detour( void* ecx, void* edx )
{
	return true;
}
