#include "../includes.h"

#include "memory/pe32.h"

#include "../source_engine/enumerations/e_item_definition_index.h"
#include <intrin.h>

void* utilities_t::get_module_handle( const std::string_view module_name )
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

void* utilities_t::get_export_address( const void* module_handle, const std::string_view export_name )
{
	const std::uint8_t* address                  = static_cast< const std::uint8_t* >( module_handle );
	const IMAGE_DOS_HEADER* dos_header           = static_cast< const IMAGE_DOS_HEADER* >( module_handle );
	const IMAGE_NT_HEADERS* nt_headers           = reinterpret_cast< const IMAGE_NT_HEADERS* >( address + dos_header->e_lfanew );
	const IMAGE_OPTIONAL_HEADER* optional_header = &nt_headers->OptionalHeader;

	const std::uintptr_t export_size    = optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].Size;
	const std::uintptr_t export_address = optional_header->DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT ].VirtualAddress;

	if ( export_size <= 0U )
		return nullptr;

	const IMAGE_EXPORT_DIRECTORY* export_directory = reinterpret_cast< const IMAGE_EXPORT_DIRECTORY* >( address + export_address );
	const std::uintptr_t* names_rva                = reinterpret_cast< const std::uintptr_t* >( address + export_directory->AddressOfNames );
	const std::uintptr_t* functions_rva            = reinterpret_cast< const std::uintptr_t* >( address + export_directory->AddressOfFunctions );
	const std::uint16_t* name_ordinals             = reinterpret_cast< const std::uint16_t* >( address + export_directory->AddressOfNameOrdinals );

	std::uintptr_t right = export_directory->NumberOfNames;
	std::uintptr_t left  = 0;

	while ( right != left ) {
		const std::uintptr_t middle = left + ( ( right - left ) >> 1U );
		const int result            = export_name.compare( reinterpret_cast< const char* >( address + names_rva[ middle ] ) );

		if ( result == 0 )
			return const_cast< void* >( static_cast< const void* >( address + functions_rva[ name_ordinals[ middle ] ] ) );

		if ( result > 0 )
			left = middle;
		else
			right = middle;
	}

	return nullptr;
}

const int utilities_t::get_fps( )
{
	static auto float_fps = 1.0f;
	float_fps             = 0.9f * float_fps + 0.1f * memory.m_globals->m_abs_frame_time;

	return float_fps != 0.0f ? static_cast< int >( 1 / float_fps ) : 0;
}

const char8_t* utilities_t::get_weapon_icon( short item_definition_index )
{
	switch ( item_definition_index ) {
	case weapon_deagle:
		return u8"\uE001";
	case weapon_elite:
		return u8"\uE002";
	case weapon_fiveseven:
		return u8"\uE003";
	case weapon_glock:
		return u8"\uE004";
	case weapon_ak47:
		return u8"\uE007";
	case weapon_aug:
		return u8"\uE008";
	case weapon_awp:
		return u8"\uE009";
	case weapon_famas:
		return u8"\uE00A";
	case weapon_g3sg1:
		return u8"\uE00B";
	case weapon_galilar:
		return u8"\uE00D";
	case weapon_m249:
		return u8"\uE00E";
	case weapon_m4a1:
		return u8"\uE010";
	case weapon_mac10:
		return u8"\uE011";
	case weapon_p90:
		return u8"\uE013";
	case weapon_mp5sd:
		return u8"\uE017";
	case weapon_ump45:
		return u8"\uE018";
	case weapon_xm1014:
		return u8"\uE019";
	case weapon_bizon:
		return u8"\uE01A";
	case weapon_mag7:
		return u8"\uE01B";
	case weapon_negev:
		return u8"\uE01C";
	case weapon_sawedoff:
		return u8"\uE01D";
	case weapon_tec9:
		return u8"\uE01E";
	case weapon_taser:
		return u8"\uE01F";
	case weapon_hkp2000:
		return u8"\uE020";
	case weapon_mp7:
		return u8"\uE021";
	case weapon_mp9:
		return u8"\uE022";
	case weapon_nova:
		return u8"\uE023";
	case weapon_p250:
		return u8"\uE024";
	case weapon_scar20:
		return u8"\uE026";
	case weapon_sg556:
		return u8"\uE027";
	case weapon_ssg08:
		return u8"\uE028";
	case weapon_knife:
		return u8"\uE02A";
	case weapon_flashbang:
		return u8"\uE02B";
	case weapon_hegrenade:
		return u8"\uE02C";
	case weapon_smokegrenade:
		return u8"\uE02D";
	case weapon_molotov:
		[[fallthrough]];
	case weapon_firebomb:
		return u8"\uE02E";
	case weapon_decoy:
		[[fallthrough]];
	case weapon_diversion:
		return u8"\uE02F";
	case weapon_incgrenade:
		return u8"\uE030";
	case weapon_c4:
		return u8"\uE031";
	case weapon_healthshot:
		return u8"\uE039";
	case weapon_knife_gg:
		[[fallthrough]];
	case weapon_knife_t:
		return u8"\uE03B";
	case weapon_m4a1_silencer:
		return u8"\uE03C";
	case weapon_usp_silencer:
		return u8"\uE03D";
	case weapon_cz75a:
		return u8"\uE03F";
	case weapon_revolver:
		return u8"\uE040";
	case weapon_tagrenade:
		return u8"\uE044";
	case weapon_fists:
		return u8"\uE045";
	case weapon_tablet:
		return u8"\uE048";
	case weapon_melee:
		return u8"\uE04A";
	case weapon_axe:
		return u8"\uE04B";
	case weapon_hammer:
		return u8"\uE04C";
	case weapon_spanner:
		return u8"\uE04E";
	case weapon_knife_bayonet:
		return u8"\uE1F4";
	case weapon_knife_css:
		return u8"\uE1F7";
	case weapon_knife_flip:
		return u8"\uE1F9";
	case weapon_knife_gut:
		return u8"\uE1FA";
	case weapon_knife_karambit:
		return u8"\uE1FB";
	case weapon_knife_m9_bayonet:
		return u8"\uE1FC";
	case weapon_knife_tactical:
		return u8"\uE1FD";
	case weapon_knife_falchion:
		return u8"\uE200";
	case weapon_knife_survival_bowie:
		return u8"\uE202";
	case weapon_knife_butterfly:
		return u8"\uE203";
	case weapon_knife_push:
		return u8"\uE204";
	case weapon_knife_cord:
		return u8"\uE205";
	case weapon_knife_canis:
		return u8"\uE206";
	case weapon_knife_ursus:
		return u8"\uE207";
	case weapon_knife_gypsy_jackknife:
		return u8"\uE208";
	case weapon_knife_outdoor:
		return u8"\uE209";
	case weapon_knife_stiletto:
		return u8"\uE20A";
	case weapon_knife_widowmaker:
		return u8"\uE20B";
	case weapon_knife_skeleton:
		return u8"\uE20D";
	default:
		return u8"\u003F";
	}
}
