#include "scaleform.h"
#include "../../../globals/config/config.h"
#include "../../../globals/includes/includes.h"
#include <fstream>
#include <iosfwd>

constexpr const char* deathnotices =
#include "files/deathnotices.js"
	;

constexpr const char* alerts =
#include "files/alerts.js"
	;

constexpr const char* teamcount_avatar =
#include "files/teamcount_avatar.js"
	;

constexpr const char* teamcount_score =
#include "files/teamcount_score.js"
	;

constexpr const char* weapon_select =
#include "files/weapon_select.js"
	;

constexpr const char* health_and_ammo =
#include "files/health_and_ammo.js"
	;

c_uipanel* n_scaleform::impl_t::panorama_functions_t::get_panel_from( c_uipanel* panel, uint32_t _hash )
{
	auto itr       = panel;
	c_uipanel* ret = nullptr;

	while ( itr && g_scaleform.m_uiengine->is_valid_panel_ptr( itr ) ) {
		auto hash = HASH_RT( itr->get_id( ) );
		if ( hash == _hash ) {
			ret = itr;
			break;
		}
		itr = itr->get_parent( );
	}

	return ret;
}

c_uipanel* n_scaleform::impl_t::panorama_functions_t::get_panel( uint32_t _hash )
{
	auto panel = g_scaleform.m_uiengine->get_last_dispatched_event_target_panel( );
	return get_panel_from( panel, _hash );
}

#pragma region elements
void n_scaleform::impl_t::panorama_elements_t::do_radar( )
{
	if ( !g_scaleform.m_should_force_update && g_scaleform.m_last_hud_color == g_scaleform.m_curr_hud_color &&
	     g_scaleform.m_last_hud_opacity == g_scaleform.m_curr_hud_opacity )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel,
	                                    std::format( R"(
            var radar = $.GetContextPanel().FindChildTraverse('HudRadar');
            var radar_content = radar.FindChildTraverse('Radar');
            var radar_label = radar.FindChildTraverse('DashboardLabel');
            var label_container = radar_label.GetParent();
            var radar_old_round = radar.FindChildTraverse('Radar__Round--Border');
            var radar_old_square = radar.FindChildTraverse('Radar__Square--Border');
            radar.style.marginTop = '2px';
            radar_content.style.marginTop = '-4px';
            radar_label.style.S2MixBlendMode = 'SRGBadditive';
            radar_label.style.opacity = '0.75';
            radar_label.style.fontWeight = 'medium';
            radar_label.style.letterSpacing = '0.5px';
            radar_label.style.fontSize = '27px';
            radar_label.style.margin = '4px 0px 0px 16px';
            radar_label.style.textOverflow = 'noclip';
            label_container.style.backgroundColor = '#FF000000';
            label_container.style.marginTop = '-2px';
            label_container.style.width = '276px';
            label_container.style.height = '46px';
            label_container.style.backgroundImage = 'url("https://media.discordapp.net/attachments/729103430441893958/1005692313047150642/radar_bg.png")';
            label_container.style.backgroundSize = '100% 100%';
            radar_old_round.style.borderRadius = '50% / 50%';
            radar_old_round.style.borderWidth = '2px';
            radar_old_round.style.borderStyle = 'solid';
            radar_old_round.style.borderColor = '{0}';
            radar_old_round.style.opacity = '0.25';
            radar_old_round.style.S2MixBlendMode = 'SRGBadditive';
            radar_old_square.style.borderRadius = '0px';
            radar_old_square.style.borderWidth = '2px';
            radar_old_square.style.borderStyle = 'solid';
            radar_old_square.style.borderColor = '{1}';
            radar_old_square.style.opacity = '0.25';
            radar_old_square.style.S2MixBlendMode = 'SRGBadditive';
        )",
	                                                 g_scaleform.m_hud_color_hex, g_scaleform.m_hud_color_hex )
	                                        .c_str( ),
	                                    ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );
}

void n_scaleform::impl_t::panorama_elements_t::do_healtharmor( )
{
	if ( !g_scaleform.m_should_force_update && g_scaleform.m_last_hud_color == g_scaleform.m_curr_hud_color )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, health_and_ammo, ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );
}

void n_scaleform::impl_t::panorama_elements_t::do_weaponpanel( )
{
	// make bg visible
	if ( !g_scaleform.m_weapon_panel_bg->is_visible( ) )
		g_scaleform.m_weapon_panel_bg->set_visible( true );

	if ( !g_scaleform.m_should_force_update )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, ( R"(
            var wpn = $.GetContextPanel().FindChildTraverse('HudWeaponPanel');
            var wpn_bg = wpn.FindChildTraverse('WeaponPanelBottomBG');
            var wpn_gradient = wpn.FindChildTraverse('WeaponPanelBottomBGGradient');
            var wpn_solid = wpn.FindChildTraverse('WeaponPanelBottomBGSolid');
            var bullet_positions = [35, 27, 19, 11, 3];
            for (var i = 1; i < 6; i++) {
                 var bullet = $.GetContextPanel().FindChildrenWithClassTraverse('AmmoAnim__BulletIcon--Pos' + i)[0];
                 bullet.style.x = bullet_positions[i - 1] + 'px';
                 bullet.style.y = '24px';
            }
            $.GetContextPanel().FindChildTraverse('AmmoTextClip').style.S2MixBlendMode = 'SRGBadditive';
            $.GetContextPanel().FindChildTraverse('HudWeaponSelection').style.marginBottom = '15px';
            wpn.style.marginRight = '0px';
            wpn.style.paddingRight = '-2px';
            wpn_bg.style.backgroundColor = 'transparent';
            wpn_bg.style.backgroundImage = 'url("https://media.discordapp.net/attachments/729103430441893958/1005692266964328498/weapon_panel_bg.png")';
            wpn_bg.style.backgroundSize = '100% 100%';
            wpn_solid.style.backgroundColor = 'transparent';
            wpn_gradient.style.backgroundColor = 'transparent';
        )" ),
	                                    ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );
}

void n_scaleform::impl_t::panorama_elements_t::do_weaponselection( )
{
	static int last_holding_weapon{ };

	if ( !g_ctx.m_local )
		return;

	auto weapon_handle = g_ctx.m_local->get_active_weapon_handle( );
	if ( !weapon_handle )
		return;

	const auto active_weapon =
		reinterpret_cast< c_base_entity* >( g_interfaces.m_client_entity_list->get_client_entity_from_handle( weapon_handle ) );

	if ( !active_weapon )
		return;

	if ( !g_scaleform.m_should_force_update && last_holding_weapon == active_weapon->get_item_definition_index( ) )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, weapon_select, ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );

	last_holding_weapon = active_weapon->get_item_definition_index( );
}

void n_scaleform::impl_t::panorama_elements_t::do_moneypanel( )
{
	if ( !g_scaleform.m_should_force_update )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, ( R"(
            var m_p = $.GetContextPanel().FindChildTraverse('HudMoney');
            var m_bg = m_p.Children()[0];
            var m_txt = m_p.Children()[1];
            var is_behind = m_txt.Children()[2].paneltype == 'Image';
            var m_txt_lbl = m_txt.Children()[is_behind ? 3 : 2];
            var m_icon = m_txt.Children()[is_behind ? 2 : 4];
            var gi_icons = $.GetContextPanel().FindChildTraverse('HudGameIcons');
            gi_icons.style.marginTop = '33px';
            if (!is_behind)
               m_txt.MoveChildBefore(m_icon, m_txt_lbl);
            m_p.style.marginTop = '0px';
            m_p.style.height = '39px';
            m_p.style.opacity = '1.0';
            m_bg.style.marginLeft = '0px';
            m_bg.style.backgroundColor = 'transparent';
            m_bg.style.backgroundImage = 'url("https://media.discordapp.net/attachments/729103430441893958/1005692403581194301/money_bg.png")';
            m_bg.style.backgroundSize = '100% 100%';
            m_bg.style.width = '257px';
            m_txt_lbl.style.S2MixBlendMode = 'SRGBadditive';
            m_txt_lbl.style.textAlign = 'right';
            m_txt_lbl.style.width = '102px';
            m_txt_lbl.style.marginTop = '1px';
            m_txt_lbl.style.marginLeft = '0px';
            m_txt_lbl.style.letterSpacing = '0px';
            m_txt_lbl.style.fontSize = '33px';
            m_icon.style.S2MixBlendMode = 'SRGBadditive';
            m_icon.style.width = '32px';
            m_icon.style.height = '32px';
            m_icon.style.marginTop = '4px';
            m_icon.style.marginLeft = '14px';
            m_icon.style.washColor = '#FFFFFF';
            m_icon.SetImage('https://cdn.discordapp.com/attachments/755851390496145548/1078756711239798784/buyicon.png');
        )" ),
	                                    ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );
}

void n_scaleform::impl_t::panorama_elements_t::do_winpanel( )
{
	if ( !g_scaleform.m_should_force_update && !g_scaleform.m_should_update_winpanel )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, R"(
var win_root = $.GetContextPanel().FindChildTraverse('HudWinPanel');
var win_team = win_root.FindChildrenWithClassTraverse('WinPanelTeam')[0];
var win_team_bg = win_team.FindChildrenWithClassTraverse('TeamBG__MainBG')[0];
var win_title = win_team.FindChildTraverse('Title');
var win_mvp = win_root.FindChildTraverse('MVP');
var win_mvp_content = win_mvp.GetParent();
var win_mvp_avatar = win_mvp.FindChildTraverse('MVPAvatar');
var win_mvp_star = win_mvp.FindChildrenWithClassTraverse('MVP__WinnerStar')[0];
var win_mvp_text = win_mvp.FindChildTraverse('MVPWinnerName');
var win_funfact = win_root.FindChildTraverse('Funfact');
var win_funfact_text = win_root.FindChildTraverse('FunFactText');
var did_ctwin = $.GetContextPanel().FindChildrenWithClassTraverse('WinPanelRoot--CTWin')[0] != undefined;
var did_twin = $.GetContextPanel().FindChildrenWithClassTraverse('WinPanelRoot--TWin')[0] != undefined;
var win_icon = win_team.FindChildrenWithClassTraverse('TeamBG__DefaultLogo')[0];
var win_fg = win_team.FindChildrenWithClassTraverse('TeamFG')[0];
win_root.style.y = '111px';
win_root.style.width = '818px';
win_root.FindChildrenWithClassTraverse('WinPanelRow--Main')[0].style.height = '125px';
win_root.style.height = '500px';
win_team.style.width = '100%';
win_team.style.height = '100%';
win_title.style.fontSize = '38px';
win_title.style.fontWeight = 'bold';
win_title.style.S2MixBlendMode = 'SRGBadditive';
win_title.style.color = '#999999';
win_title.style.letterSpacing = '0px';
win_mvp_content.style.align = 'left bottom';
win_mvp_content.style.marginLeft = '21px';
win_mvp.style.horizontalAlign = 'left';
win_mvp_avatar.style.width = '60px';
win_mvp_avatar.style.height = '62px';
win_mvp_avatar.style.boxShadow = '#000000 0px 0px 4px 2px';
win_mvp_avatar.style.padding = '0px 0px 0px 0px'
win_mvp_avatar.style.backgroundColor = 'gradient(linear, 0% 0%, 0% 100%, from(#959595), to(#353535))';
win_mvp_text.style.color = '#666666';
win_mvp_text.style.S2MixBlendMode = 'SRGBadditive';
win_mvp_text.style.marginTop = '7px';
win_mvp_text.style.fontWeight = 'bold';
win_mvp_text.style.fontSize = '17.75px';
win_mvp_text.style.letterSpacing = '0px';
win_mvp_star.style.washColor = '#FBB03B';
win_mvp_star.style.marginTop = '4px';
win_mvp_star.style.marginLeft = '3px';
win_mvp_star.style.height = '19px';
win_mvp_star.style.width = '18px';
win_team_bg.style.width = did_twin ? '100.4%' : '100%';
win_team_bg.style.height = '101.75%';
win_team_bg.style.opacity = did_twin ? '0.975' : '0.995';
win_team_bg.style.backgroundColor = 'transparent';
win_team_bg.style.backgroundImage = did_twin ? 'url("https://media.discordapp.net/attachments/983151109721960479/1045530761241051167/50.png")' :
    did_ctwin ? 'url("https://media.discordapp.net/attachments/729103430441893958/1045534098615189585/winpanel_ct.png")' : 
    'url("https://media.discordapp.net/attachments/729103430441893958/1045534098615189585/winpanel_ct.png")';
win_team_bg.style.backgroundSize = did_twin ? '99.65% 97%' : '100.1% 100%';
win_funfact.style.y = '200px';
win_funfact.style.backgroundColor = '#000000AE';
win_funfact.style.marginTop = '2px';
win_funfact.style.marginLeft = '0px';
win_funfact.style.marginRight = '0px';
win_funfact.style.padding = '6px 6px 8px 6px';
win_funfact.style.color = '#FFFFFFBB';
win_funfact.style.width = '99.9%';
win_funfact_text.style.backgroundColor = 'transparent';
win_funfact_text.style.color = '#FFFFFFAE';
win_funfact_text.style.padding = '0px 0px 0px 0px';
win_funfact_text.style.fontSize = '18px';
win_funfact_text.style.fontWeight = 'bold';
win_funfact_text.style.S2MixBlendMode = 'SRGBadditive';
win_funfact_text.style.letterSpacing = '0px';
win_icon.style.opacity = '0.30';
win_icon.style.verticalAlign = 'bottom';
win_icon.style.horizontalAlign = 'right';
win_icon.style.width = '69px';
win_icon.style.marginTop = '51px';
win_icon.style.marginRight = '20px';
win_icon.style.marginBottom = '0px';
win_team.FindChildrenWithClassTraverse('TeamBG__hrBot')[0].style.opacity = '0';
win_team.FindChildrenWithClassTraverse('TeamBG__hrTop')[0].style.opacity = '0';
win_team.FindChildrenWithClassTraverse('TeamBG__hrMid')[0].style.opacity = '0';
win_team.FindChildTraverse('TeamLogo').style.opacity = '0';
function set_funfact() {
    win_funfact.style.transition = 'position 0.20s linear 0.0s';
    win_funfact.style.y = '125px';
}
function white_fade_in() {
    win_fg.style.transition = 'background-color 0.10s linear 0.0s';
    win_mvp_star.style.transition = 'wash-color 0.10s linear 0.0s';
    win_mvp_star.style.washColor = '#FFFFFF';
    win_fg.style.backgroundColor = '#FFFFFF';
}
function white_fade_out() {
    win_fg.style.transition = 'background-color 0.20s linear 0.0s';
    win_mvp_star.style.transition = 'wash-color 0.20s linear 0.0s';
    win_mvp_star.style.washColor = '#FBB03B';
    win_fg.style.backgroundColor = 'transparent';
}
$.Schedule(0.20, white_fade_in);
$.Schedule(0.30, white_fade_out);
$.Schedule(1.00, set_funfact);
        )",
	                                    ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );

	// do not spam update after round_end lolz
	g_scaleform.m_should_update_winpanel = false;
}

void n_scaleform::impl_t::panorama_elements_t::do_deathnotices( )
{
	if ( !g_scaleform.m_should_force_update && !g_scaleform.m_should_update_deathnotices )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, deathnotices, "panorama/layout/hud/hud.xml", 8, 10, false, false );

	g_scaleform.m_should_update_deathnotices = false;
}

void n_scaleform::impl_t::panorama_elements_t::do_teamcounter_score( )
{
	if ( !g_scaleform.m_should_force_update )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, teamcount_score, ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );
}

void n_scaleform::impl_t::panorama_elements_t::do_alert_text( )
{
	if ( !g_scaleform.m_should_force_update )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, alerts, ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );
}

void n_scaleform::impl_t::panorama_elements_t::do_teamcounter( )
{
	if ( !g_scaleform.m_should_force_update && !g_scaleform.m_should_update_teamcount_avatar )
		return;

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, teamcount_avatar, ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );

	g_scaleform.m_should_update_teamcount_avatar = false;
}
#pragma endregion

bool n_scaleform::impl_t::panorama_elements_t::get_panels( bool do_non_cm_panels )
{
	// get interfaces
	if ( !g_scaleform.m_uiengine ) {
		g_scaleform.m_uiengine = g_interfaces.m_panorama->access_ui_engine( );
		return g_scaleform.m_uiengine;
	}

	// get panels
	if ( !do_non_cm_panels ) {
		if ( !g_scaleform.m_hud_panel )
			g_scaleform.m_hud_panel = g_scaleform.panorama_functions.get_panel( HASH_BT( "CSGOHud" ) );

		if ( g_scaleform.m_hud_panel && !g_scaleform.m_weapon_panel )
			g_scaleform.m_weapon_panel = g_scaleform.m_hud_panel->find_child_traverse( ( "HudWeaponPanel" ) );

		if ( g_scaleform.m_weapon_panel && !g_scaleform.m_weapon_panel_bg )
			g_scaleform.m_weapon_panel_bg = g_scaleform.m_weapon_panel->find_child_traverse( ( "WeaponPanelBottomBG" ) );

		if ( g_scaleform.m_hud_panel && !g_scaleform.m_health_armor_panel )
			g_scaleform.m_health_armor_panel = g_scaleform.m_hud_panel->find_child_traverse( ( "HudHealthArmor" ) );

		if ( g_scaleform.m_health_armor_panel && !g_scaleform.m_health_small_bg )
			g_scaleform.m_health_small_bg = g_scaleform.m_health_armor_panel->find_child_traverse( ( "HudHealthBG" ) );

		return g_scaleform.m_hud_panel && g_scaleform.m_weapon_panel_bg;
	} else {
		// do non cm panels == true
	}

	return false;
}

void n_scaleform::impl_t::panorama_elements_t::modify_all( )
{
	if ( !GET_VARIABLE( g_variables.m_scaleform, bool ) )
		return;

	// local vars
	g_scaleform.m_curr_hud_color   = g_convars[ HASH_BT( "cl_hud_color" ) ]->get_int( );
	g_scaleform.m_curr_hud_opacity = g_convars[ HASH_BT( "cl_hud_background_alpha" ) ]->get_float( );

	if ( g_scaleform.m_should_force_update )
		g_convars[ HASH_BT( "cl_showloadout" ) ]->set_value( 1 );

	// update the current hud color value
	g_scaleform.m_hud_color_hex = g_scaleform.m_curr_hud_color == 0    ? ( "#e8e8e8" )
	                              : g_scaleform.m_curr_hud_color == 1  ? ( "#ffffff" )
	                              : g_scaleform.m_curr_hud_color == 2  ? ( "#96c8ff" )
	                              : g_scaleform.m_curr_hud_color == 3  ? ( "#356eff" )
	                              : g_scaleform.m_curr_hud_color == 4  ? ( "#c864ff" )
	                              : g_scaleform.m_curr_hud_color == 5  ? ( "#ff2924" )
	                              : g_scaleform.m_curr_hud_color == 6  ? ( "#ff7124" )
	                              : g_scaleform.m_curr_hud_color == 7  ? ( "#fff724" )
	                              : g_scaleform.m_curr_hud_color == 8  ? ( "#3eff24" )
	                              : g_scaleform.m_curr_hud_color == 9  ? ( "#24ff90" )
	                              : g_scaleform.m_curr_hud_color == 10 ? ( "#ff7999" )
	                                                                   : ( "#d0d198" ); // 11

	// update the current hud alpha hex value
	g_scaleform.m_hud_alpha_hex = g_scaleform.m_curr_hud_opacity;

	if ( !g_scaleform.m_hud_panel )
		return;

	/* elements */
	do_radar( );
	do_healtharmor( );
	do_weaponpanel( );
	do_weaponselection( );
	do_moneypanel( );
	do_winpanel( );
	do_alert_text( );
	do_deathnotices( );
	do_teamcounter( );
	do_teamcounter_score( );
	/* elements */

	// don't update multiple times
	if ( g_scaleform.m_should_force_update )
		g_scaleform.m_should_force_update = false;

	// global update shart
	g_scaleform.m_last_hud_color   = g_scaleform.m_curr_hud_color;
	g_scaleform.m_last_hud_opacity = g_scaleform.m_curr_hud_opacity;
}

void n_scaleform::impl_t::on_level_init( )
{
	// modify elements
	g_scaleform.panorama_elements.get_panels( false );
	g_scaleform.m_should_force_update = true;
	g_scaleform.panorama_elements.modify_all( );
}

void n_scaleform::impl_t::on_createmove( )
{
	g_scaleform.panorama_elements.get_panels( false );
	g_scaleform.panorama_elements.modify_all( );
}

#include "../../../utilities/data/data.h"

constexpr static uint64_t hash_data( const char* data, size_t len )
{
	uint64_t hash = 0x543C730D;

	for ( size_t i = 0; i < len; ++i ) {
		hash ^= data[ i ];
		hash *= 0x1000931;
	}

	return hash;
}

bool n_scaleform::impl_t::get_replacement_icon( const char* name, const uint8_t*& data, size_t& len, int& w, int& h )
{
	uint64_t hash = hash_data( name, strlen( name ) );
	switch ( hash ) {
	case ct_data< hash_data( icon_m4a1_silencer_name, sizeof( icon_m4a1_silencer_name ) - 1 ) >::value:
		data = icon_m4a1_silencer;
		len  = sizeof( icon_m4a1_silencer );
		w    = icon_m4a1_silencer_w;
		h    = icon_m4a1_silencer_h;
		return true;
		break;
	case ct_data< hash_data( icon_m4a1_name, sizeof( icon_m4a1_name ) - 1 ) >::value:
		data = icon_m4a1;
		len  = sizeof( icon_m4a1 );
		w    = icon_m4a1_w;
		h    = icon_m4a1_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_tactical_name, sizeof( icon_knife_tactical_name ) - 1 ) >::value:
		data = icon_knife_tactical;
		len  = sizeof( icon_knife_tactical );
		w    = icon_knife_tactical_w;
		h    = icon_knife_tactical_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_t_name, sizeof( icon_knife_t_name ) - 1 ) >::value:
		data = icon_knife_t;
		len  = sizeof( icon_knife_t );
		w    = icon_knife_t_w;
		h    = icon_knife_t_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_survival_bowie_name, sizeof( icon_knife_survival_bowie_name ) - 1 ) >::value:
		data = icon_knife_survival_bowie;
		len  = sizeof( icon_knife_survival_bowie );
		w    = icon_knife_survival_bowie_w;
		h    = icon_knife_survival_bowie_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_push_name, sizeof( icon_knife_push_name ) - 1 ) >::value:
		data = icon_knife_push;
		len  = sizeof( icon_knife_push );
		w    = icon_knife_push_w;
		h    = icon_knife_push_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_m9_bayonet_name, sizeof( icon_knife_m9_bayonet_name ) - 1 ) >::value:
		data = icon_knife_m9_bayonet;
		len  = sizeof( icon_knife_m9_bayonet );
		w    = icon_knife_m9_bayonet_w;
		h    = icon_knife_m9_bayonet_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_karambit_name, sizeof( icon_knife_karambit_name ) - 1 ) >::value:
		data = icon_knife_karambit;
		len  = sizeof( icon_knife_karambit );
		w    = icon_knife_karambit_w;
		h    = icon_knife_karambit_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_gut_name, sizeof( icon_knife_gut_name ) - 1 ) >::value:
		data = icon_knife_gut;
		len  = sizeof( icon_knife_gut );
		w    = icon_knife_gut_w;
		h    = icon_knife_gut_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_flip_name, sizeof( icon_knife_flip_name ) - 1 ) >::value:
		data = icon_knife_flip;
		len  = sizeof( icon_knife_flip );
		w    = icon_knife_flip_w;
		h    = icon_knife_flip_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_name, sizeof( icon_knife_name ) - 1 ) >::value:
		data = icon_knife;
		len  = sizeof( icon_knife );
		w    = icon_knife_w;
		h    = icon_knife_h;
		return true;
		break;
	case ct_data< hash_data( icon_knife_butterfly_name, sizeof( icon_knife_butterfly_name ) - 1 ) >::value:
		data = icon_knife_butterfly;
		len  = sizeof( icon_knife_butterfly );
		w    = icon_knife_butterfly_w;
		h    = icon_knife_butterfly_h;
		return true;
		break;
	case ct_data< hash_data( icon_bayonet_name, sizeof( icon_bayonet_name ) - 1 ) >::value:
		data = icon_bayonet;
		len  = sizeof( icon_bayonet );
		w    = icon_bayonet_w;
		h    = icon_bayonet_h;
		return true;
		break;
	case ct_data< hash_data( icon_incgrenade_name, sizeof( icon_incgrenade_name ) - 1 ) >::value:
		data = icon_incgrenade;
		len  = sizeof( icon_incgrenade );
		w    = icon_incgrenade_w;
		h    = icon_incgrenade_h;
		return true;
		break;
	case ct_data< hash_data( icon_hkp2000_name, sizeof( icon_hkp2000_name ) - 1 ) >::value:
		data = icon_hkp2000;
		len  = sizeof( icon_hkp2000 );
		w    = icon_hkp2000_w;
		h    = icon_hkp2000_h;
		return true;
		break;
	case ct_data< hash_data( icon_hegrenade_name, sizeof( icon_hegrenade_name ) - 1 ) >::value:
		data = icon_hegrenade;
		len  = sizeof( icon_hegrenade );
		w    = icon_hegrenade_w;
		h    = icon_hegrenade_h;
		return true;
		break;
	case ct_data< hash_data( icon_flashbang_name, sizeof( icon_flashbang_name ) - 1 ) >::value:
		data = icon_flashbang;
		len  = sizeof( icon_flashbang );
		w    = icon_flashbang_w;
		h    = icon_flashbang_h;
		return true;
		break;
	case ct_data< hash_data( icon_elite_name, sizeof( icon_elite_name ) - 1 ) >::value:
		data = icon_elite;
		len  = sizeof( icon_elite );
		w    = icon_elite_w;
		h    = icon_elite_h;
		return true;
		break;
	case ct_data< hash_data( icon_decoy_name, sizeof( icon_decoy_name ) - 1 ) >::value:
		data = icon_decoy;
		len  = sizeof( icon_decoy );
		w    = icon_decoy_w;
		h    = icon_decoy_h;
		return true;
		break;
	case ct_data< hash_data( icon_deagle_name, sizeof( icon_deagle_name ) - 1 ) >::value:
		data = icon_deagle;
		len  = sizeof( icon_deagle );
		w    = icon_deagle_w;
		h    = icon_deagle_h;
		return true;
		break;
	case ct_data< hash_data( icon_awp_name, sizeof( icon_awp_name ) - 1 ) >::value:
		data = icon_awp;
		len  = sizeof( icon_awp );
		w    = 1; // icon_awp_w;
		h    = 1; // icon_awp_h;
		return true;
		break;
	case ct_data< hash_data( icon_ak47_name, sizeof( icon_ak47_name ) - 1 ) >::value:
		data = icon_ak47;
		len  = sizeof( icon_ak47 );
		w    = icon_ak47_w;
		h    = icon_ak47_h;
		return true;
		break;
	case ct_data< hash_data( icon_ssg08_name, sizeof( icon_ssg08_name ) - 1 ) >::value:
		data = icon_ssg08;
		len  = sizeof( icon_ssg08 );
		w    = icon_ssg08_w;
		h    = icon_ssg08_h;
		return true;
		break;
	case ct_data< hash_data( icon_smokegrenade_name, sizeof( icon_smokegrenade_name ) - 1 ) >::value:
		data = icon_smokegrenade;
		len  = sizeof( icon_smokegrenade );
		w    = icon_smokegrenade_w;
		h    = icon_smokegrenade_h;
		return true;
		break;
	case ct_data< hash_data( icon_molotov_name, sizeof( icon_molotov_name ) - 1 ) >::value:
		data = icon_molotov;
		len  = sizeof( icon_molotov );
		w    = icon_molotov_w;
		h    = icon_molotov_h;
		return true;
		break;
	case ct_data< hash_data( icon_mag7_name, sizeof( icon_mag7_name ) - 1 ) >::value:
		data = icon_mag7;
		len  = sizeof( icon_mag7 );
		w    = icon_mag7_w;
		h    = icon_mag7_h;
		return true;
		break;
	case ct_data< hash_data( icon_m4a1_silencer_off_name, sizeof( icon_m4a1_silencer_off_name ) - 1 ) >::value:
		data = icon_m4a1_silencer_off;
		len  = sizeof( icon_m4a1_silencer_off );
		w    = icon_m4a1_silencer_off_w;
		h    = icon_m4a1_silencer_off_h;
		return true;
		break;
	}

	return false;
}
