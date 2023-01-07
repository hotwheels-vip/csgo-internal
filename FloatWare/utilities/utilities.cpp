#include "utilities.hh"
#include "structs/pe32.hh"

#include <intrin.h>

void* utilities::impl::get_module_handle( const std::string_view module_name )
{
	const _PEB32* peb_data = reinterpret_cast< _PEB32* >( __readfsdword( 0x30 ) );

	if ( module_name.empty( ) )
		return peb_data->ImageBaseAddress;

	const std::wstring converted_module_name( module_name.begin( ), module_name.end( ) );

	for ( LIST_ENTRY* list_entry = peb_data->Ldr->InLoadOrderModuleList.Flink; list_entry != &peb_data->Ldr->InLoadOrderModuleList;
	      list_entry             = list_entry->Flink ) {
		const _LDR_DATA_TABLE_ENTRY* entry = CONTAINING_RECORD( list_entry, _LDR_DATA_TABLE_ENTRY, InLoadOrderLinks );

		if ( entry->BaseDllName.Buffer && converted_module_name.compare( entry->BaseDllName.Buffer ) == 0 )
			return entry->DllBase;
	}

	return nullptr;
}
