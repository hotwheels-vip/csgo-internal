/*
    use the m_ prefix for members of structs and classes.
    state the reason why you have include a file.

    macros UPPERCASE regar.ds.

    to-do example -
        to-do ~ blah blah blah

    note example -
        note ~ blah blah blah

    xref example
        xref @ "module.dll" "bytes"

    declarations
        enum example
            enum e_fart
            {
                fart_one,
                fart_two
                fart_max
            };

        struct example
            struct fart_t
            {
                int m_member{};
            };

            inline fart_t g_fart = {};

        namespace example
            namespace n_fart
            {

            };

        class example
            class c_fart
            {
            public:
                int get_fart()
                {
                    return this->m_fart;
                }
            private:
                int m_fart{};
            };
*/

/* He's a big chungus
He's a big chunky boi
Such a big bun, yes
We are so overjoyed
To have a big chunky boi
A big and wonderful chungus such as he
Oh such as he
He's a big chungus
He's a big chunky boi
Such a big bun, yes
We are so overjoyed
To have a big chunky boi
A big and wonderful chungus such as he
Oh, such as he
Huh
Buns come in all shapes and all sizes
This one has so many surprises
I've never seen a giant quite like him
There's no one like him
Nobody like chungus
Huh
Get the game for PS4
For a limited time
Huh
Don't miss what it has in store
You're running out of time
Play the game of the year
The game with that colossal boi
He's a big chungus
He's a big chunky boi
Such a big bun, yes
We are so overjoyed
To have a big chunky boi
A big and wonderful chungus such as he
Oh, such as he
Huh
Buns come in all shapes and all sizes
This one has so many surprises
I've never seen a giant quite like him
There's no one like him
Nobody like chungus
Huh */

#include "game/sdk/includes/includes.h"
#include "globals/fonts/fonts.h"
#include "globals/includes/includes.h"

static unsigned long __stdcall on_attach( void* instance )
{
	g_console.on_attach( "fart" );

	g_console.print( "initialising module handles" );
	if ( !g_modules.on_attach( ) )
		g_console.print< n_console::log_level::WARNING >( "failed to initialise module handles" );
	else
		g_console.print( "initialised module handles" );

	g_console.print( "initialising interfaces" );
	if ( !g_interfaces.on_attach( ) )
		g_console.print< n_console::log_level::WARNING >( "failed to initialise interfaces" );
	else
		g_console.print( "initialised interfaces" );

	g_console.print( "initialising netvar manager" );
	if ( !g_netvars.on_attach( ) )
		g_console.print< n_console::log_level::WARNING >( "failed to initialise netvar manager" );
	else
		g_console.print( "initialised netvar manager" );

	g_console.print( "initialising convars" );
	if ( !g_convars.on_attach( ) )
		g_console.print< n_console::log_level::WARNING >( "failed to initialise convars" );
	else
		g_console.print( "initialised convars" );

	g_console.print( "initialising input system" );
	if ( !g_input.on_attach( ) )
		g_console.print< n_console::log_level::WARNING >( "failed to initialise input system" );
	else
		g_console.print( "initialised hooks" );

	g_console.print( "initialising config system" );
	if ( !g_config.on_attach( ) )
		g_console.print< n_console::log_level::WARNING >( "failed to initialise config system" );
	else
		g_console.print( "initialised config system" );

	g_console.print( "initialising hooks" );
	if ( !g_hooks.on_attach( ) )
		g_console.print< n_console::log_level::WARNING >( "failed to initialise hooks" );
	else
		g_console.print( "initialised hooks" );

	LI_FN( GetWindowsDirectoryA )( g_ctx.m_windows_directory, 64 );
	g_console.print( std::vformat( "windows directory - {:s}", std::make_format_args( g_ctx.m_windows_directory ) ).c_str( ) );

	g_fonts.on_attach( );
	g_console.print( "initialised font file vector" );

	while ( !g_input.is_key_down( VK_END ) && !g_input.is_key_down( VK_DELETE ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	g_ctx.m_unloading = true;

	g_hooks.on_release( );

	g_input.on_release( );

	g_render.on_release( );

	g_console.on_release( );

	g_interfaces.m_client_state->m_delta_tick = -1;

	LI_FN( FreeLibrary )( static_cast< HMODULE >( instance ) );

	return 0;
}

int __stdcall DllMain( void* instance, unsigned long reason_for_call, void* reserved )
{
	switch ( reason_for_call ) {
	case DLL_PROCESS_ATTACH: {
		LI_FN( DisableThreadLibraryCalls )( reinterpret_cast< HMODULE >( instance ) );

		return g_utilities.create_thread( on_attach, instance );
	}
	}

	return TRUE;
}
