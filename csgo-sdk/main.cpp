/*
    use the m_ prefix for members of structs and classes.
    state the reason why you have include a file.

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

#include "globals/includes/includes.h"

#include <thread>

static unsigned long __stdcall on_attach( void* instance )
{
	/* to-do ~ initialise shart here */
	g_console.on_attach( "fart" );

	g_console.print( "initialising module handles" );
	if ( !g_modules.on_attach( ) )
		g_console.print( "failed to initialise module handles" );
	else
		g_console.print( "initialised module handles" );

	while ( !GetAsyncKeyState( VK_END ) )
		std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

	/* to-do ~ uninitialise shart here */
	g_console.on_release( );

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