#include "scaleform.h"
#include "../../../globals/config/config.h"

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

	g_scaleform.m_uiengine->run_script( g_scaleform.m_hud_panel, ( R"(
var ha = $.GetContextPanel().FindChildTraverse('HudHealthArmor');
var ha_bg = ha.FindChildTraverse('HudHealthArmorBG');
var ha_icons = ha.FindChildrenWithClassTraverse('hud-HA-icon-container');
var ha_texts = ha.FindChildrenWithClassTraverse('hud-HA-text');
var h_bar = ha.FindChildTraverse('HealthBar');
var a_bar = ha.FindChildTraverse('ArmorBar');
var h_bg = ha.FindChildTraverse('HudHealthBG');
var a_bg = ha.FindChildTraverse('HudArmorBG');
var h_icon = ha_icons[0].Children()[0];
var a_icon = ha_icons[1].Children()[1];
var a_h_icon = ha_icons[1].Children()[2];
var ha_main = ha.FindChildrenWithClassTraverse('hud-HA-main')[0];
ha_main.style.marginLeft = '-4px';
ha.FindChildrenWithClassTraverse('hud-HA-health-label')[0].style.S2MixBlendMode = 'SRGBadditive';
ha.FindChildrenWithClassTraverse('hud-HA-armor-label')[0].style.S2MixBlendMode = 'SRGBadditive';
ha.style.marginLeft = '-1px';
ha.style.height = '53px';
ha_bg.style.width = '528px';
ha_bg.style.height = '100%';
ha_bg.style.backgroundColor = 'transparent';
ha_bg.style.flowChildren = 'left';
ha_bg.style.backgroundImage = 'url("https://media.discordapp.net/attachments/729103430441893958/1005692446832861255/big_nr_bg.png")';
ha_bg.style.backgroundSize = '100% 103%';
ha_bg.style.transform = 'scaleX(-1)';
h_bg.style.backgroundImage = 'url("https://media.discordapp.net/attachments/729103430441893958/1005692447134846986/big_r_bg.png")';
h_bg.style.transform = 'scaleX(-1)';
h_bg.style.backgroundSize = '100% 100%';
h_bg.style.backgroundColor = 'transparent';
h_bg.style.opacity = '0'; // make diff opacity and red when in critical
a_bg.style.backgroundColor = 'transparent';
ha_texts[0].style.paddingTop = '0px';
ha_texts[1].style.paddingTop = '-1px';
ha_texts[1].style.marginLeft = '-2px';
ha_texts[1].style.textAlign = 'left';
ha_icons[1].style.marginLeft = '-2px';
h_icon.SetImage('none');
h_icon.style.S2MixBlendMode = 'SRGBadditive';
h_icon.style.washColor = 'transparent';
h_icon.style.backgroundImage = 'url("https://media.discordapp.net/attachments/729103430441893958/1005731011440693309/17.png")';
h_icon.style.backgroundSize = '100% 100%';
h_icon.style.opacity = '1';
h_icon.style.overflow = 'noclip';
h_icon.style.width = '20px';
h_icon.style.height = '20px';
a_icon.SetImage('none');
a_icon.style.S2MixBlendMode = 'SRGBadditive';
a_icon.style.washColor = 'transparent';
a_icon.style.backgroundImage = 'url("https://media.discordapp.net/attachments/729103430441893958/1005731011725889536/19.png")';
a_icon.style.backgroundSize = '100% 100%';
a_icon.style.opacity = '1';
a_icon.style.overflow = 'noclip';
a_icon.style.width = '20px';
a_icon.style.height = '21px';
a_h_icon.style.marginTop = '-4px';
a_h_icon.style.opacity = '0.7';
a_h_icon.SetImage('none');
a_h_icon.style.backgroundImage = 'url("file://{images}/icons/ui/helmet.svg")';
a_h_icon.style.backgroundSize = '100% 100%';
a_h_icon.style.overflow = 'noclip';
a_h_icon.style.width = '19px';
a_h_icon.style.height = '18px';
h_icon.style.washColor = '#ffffff';
a_icon.style.washColor = '#ffffff';
a_h_icon.style.washColor = '#ffffff';
for (var i = 0; i < 2; i++) {
    var ha_label = ha_texts[i].Children()[0];
    ha_label.style.textOverflow = 'noclip';
    ha_label.style.fontSize = '44px';
    ha_label.style.S2MixBlendMode = 'SRGBadditive';
    ha_icons[i].style.paddingTop = '6px';
}
h_bar.style.borderWidth = '0px';
h_bar.style.backgroundColor = 'transparent';
a_bar.style.borderWidth = '0px';
a_bar.style.backgroundColor = 'transparent';
a_bar.style.washColor = '#ffffff';
a_bar.Children()[0].style.washColorFast = '#FFFFFF';
a_bar.Children()[0].style.backgroundColor = '#ffffff';
h_bar.style.backgroundImage = 'url("https://media.discordapp.net/attachments/963223061337886780/990907908575363112/1.png")';
a_bar.style.backgroundImage = 'url("https://media.discordapp.net/attachments/963223061337886780/990907908575363112/1.png")';
h_bar.style.backgroundSize = '100% 100%';
a_bar.style.backgroundSize = '100% 100%';
h_bar.style.x = '106px';
h_bar.style.y = '30px';
a_bar.style.x = '106px';
a_bar.style.y = '30px';
h_bar.style.width = '84px';
h_bar.style.height = '15px';
a_bar.style.width = '84px';
a_bar.style.height = '15px';
h_bar.style.padding = '2px 2px 2px 2px';
a_bar.style.padding = '2px 2px 2px 2px';
        )" ),
	                                    ( "panorama/layout/hud/hud.xml" ), 8, 10, false, false );
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
            m_icon.SetImage('https://media.discordapp.net/attachments/727360448969572403/990509782391722034/1.png');
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
