#include "modules.h"

template< class T >
T modules::impl::find_interface( const char* interface_name )
{
	auto get_jmp_address = []( std::uintptr_t address ) -> std::uintptr_t {
		auto jmp_relative = *reinterpret_cast< std::uintptr_t* >( address + 0x1 );

		address += 0x5;
		address += jmp_relative;

		return address;
	};

	auto pointer = reinterpret_cast< std::uintptr_t >( LI_FN( GetProcAddress )( module_handle, _( "CreateInterface" ) ) );

	while ( *reinterpret_cast< std::uint8_t* >( pointer ) != 0xE9 )
		++pointer;

	pointer = get_jmp_address( pointer );

	while ( *reinterpret_cast< std::uint16_t* >( pointer ) != 0x358B )
		++pointer;

	auto node = **reinterpret_cast< interface_node*** >( pointer + 0x2 );

	char buffer_name[ 128 ]{ };

	while ( node ) {
		std::size_t size;

		for ( size = 0; node->name[ size ] != '\0'; ++size )
			buffer_name[ size ] = node->name[ size ];

		buffer_name[ size ] = '\0';

		if ( HASH_BT( buffer_name ) == HASH_BT( interface_name ) )
			break;

		node = node->next;
	}

	if ( !node ) {
		console::print< console::log_level::FATAL >( _( "Find interface node failed [ interface = {} | module = {} ]" ), interface_name,
		                                             module_name );
		return nullptr;
	}

	void* node_address = node->get( );

	if ( !node_address ) {
		console::print< console::log_level::FATAL >( _( "Find interface node->get( ) failed [ interface = {} | module = {} ]" ), interface_name,
		                                             module_name );
		return nullptr;
	}
	return reinterpret_cast< T >( node_address );
}