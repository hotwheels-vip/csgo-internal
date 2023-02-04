#include "utilities.h"

/* used ~ std::thread (*/
#include <thread>

int n_utilities::impl_t::create_thread( unsigned long __stdcall function( void* ), void* parameter )
{
	/* void* handle = LI_FN( CreateThread )( nullptr, 0, function, parameter, 0, nullptr );
	if ( !handle )
		return 0;

	LI_FN( CloseHandle )( handle ); */

	std::thread( function, parameter ).detach( );

	return 1;
}