#include "modules.h"
#include "../../globals/includes/includes.h"
#include "../memory/structs/pe32.h"

#include <unordered_map>

std::unordered_map< unsigned int, void* > dumped_modules = { };

bool n_modules::impl_t::on_attach( )
{
	const _PEB32* peb_data = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) );

	for ( LIST_ENTRY* list_entry = peb_data->Ldr->InLoadOrderModuleList.Flink; list_entry != &peb_data->Ldr->InLoadOrderModuleList;
	      list_entry             = list_entry->Flink ) {
		const _LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD( list_entry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

		if ( entry->BaseDllName.Buffer ) {
			const auto converted_name = std::wstring( entry->BaseDllName.Buffer );

			if ( !( converted_name.empty( ) ) )
				dumped_modules[ HASH_RT( std::string( converted_name.begin( ), converted_name.end( ) ).c_str( ) ) ] = entry->DllBase;
		}
	}

	return !( dumped_modules.empty( ) );
}

void* n_modules::impl_t::operator[]( unsigned int hash )
{
	return dumped_modules[ hash ];
}