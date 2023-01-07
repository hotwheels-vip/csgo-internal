#include "hooks.hh"

#include "../utilities/console/console.hh"
#include "../utilities/modules/modules.hh"

bool hooks::impl::on_attach( )
{
	auto initialise_hook = []( c_detour_hook& detour_class, void* function, void* detour, const char* hook_name ) {
		if ( !detour_class.create( function, detour ) ) {
			g_console.print( xs( "failed to hook {:s} @ {:p}" ), hook_name, function );
			return false;
		} else {
			g_console.print( xs( "{:s} hooked @ {:p}" ), hook_name, function );
			return true;
		}
	};

	if ( MH_Initialize( ) != MH_OK ) {
		throw std::runtime_error( xs( "failed initialize minhook" ) );
		return false;
	}

	static const auto vsnprintf_address = g_modules.get( e_module_names::client )
	                                          .find_pattern( xs( "55 8B EC 51 56 8B 75 0C 8D 45 14 57 8B 7D 08 8B D6 50 51 FF 75 10 8B CF E8 "
	                                                             "? ? ? ? 83 C4 0C 85 C0 78 08 85 F6 7E 0C 3B C6 7C 08 8D 46 FF" ) );

	initialise_hook( this->vsnprintf_detour, vsnprintf_address, &hooks::vsnprintf, xs( "vsnprintf" ) );

	/* TODO ~ refresh entity cache here, or fill the vector upon initialisation HEL ,o bugsgsgs */

	return true;
}

void hooks::impl::on_release( )
{
	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );
	MH_Uninitialize( );
}