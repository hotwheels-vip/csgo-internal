#include "utilities.h"

#include "../game/sdk/includes/includes.h"
#include "../globals/includes/includes.h"

/* used ~ std::thread (*/
#include <thread>

int n_utilities::impl_t::create_thread( unsigned long __stdcall function( void* ), void* parameter )
{
	std::thread( function, parameter ).detach( );

	return 1;
}

int n_utilities::impl_t::get_average_fps( ImGuiIO io )
{
	static int average_fps = static_cast< int >( io.Framerate + 0.5f );

	if ( static float last_checked = g_interfaces.m_global_vars_base->m_real_time;
	     fabs( last_checked - g_interfaces.m_global_vars_base->m_real_time ) > 2.0f ) {
		average_fps  = static_cast< int >( io.Framerate + 0.5f );
		last_checked = g_interfaces.m_global_vars_base->m_real_time;
	}
	return average_fps;
}

const char8_t* n_utilities::impl_t::get_weapon_icon( short item_definition_index )
{
	if ( item_definition_index == weapon_none )
		return u8"";

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
		g_console.print(
			std::vformat( "! get_weapon_icon failed to find icon index: {} !", std::make_format_args( item_definition_index ) ).c_str( ) );

		return u8"";
	}
}

bool n_utilities::impl_t::is_weapon_valid( )
{
	auto weapon_handle = g_ctx.m_local->get_active_weapon_handle( );
	if ( !weapon_handle )
		return false;

	const auto active_weapon =
		reinterpret_cast< c_base_entity* >( g_interfaces.m_client_entity_list->get_client_entity_from_handle( weapon_handle ) );

	if ( !active_weapon )
		return false;

	const auto definition_index = active_weapon->get_item_definition_index( );

	return definition_index > 0 && !( definition_index >= 41 && definition_index <= 59 ) && !( definition_index >= 68 );
}
