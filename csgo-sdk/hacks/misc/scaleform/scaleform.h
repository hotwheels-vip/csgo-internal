#pragma once
#include "../../../game/sdk/includes/includes.h"
#include "../../../globals/includes/includes.h"

#include <cstdint>

// creds patoke

namespace n_scaleform
{
	struct impl_t {
		struct panorama_functions_t {
			c_uipanel* get_panel_from( c_uipanel* panel, uint32_t _hash );
			c_uipanel* get_panel( uint32_t _hash );
		} panorama_functions;

		struct panorama_elements_t {
			bool get_panels( bool do_non_cm_panels );
			void modify_all( );

			/* modify */
			void do_radar( );
			void do_healtharmor( );
			void do_weaponpanel( );
			void do_weaponselection( );
			void do_moneypanel( );
			void do_teamcounter( );
			void do_winpanel( );
			void do_deathnotices( );
			void do_teamcounter_score( );
			void do_alert_text( );
		} panorama_elements;

		void on_level_init( );
		void on_createmove( );

		c_uipanel* m_hud_panel;
		c_uipanel* m_menu_panel;
		c_uipanel* m_weapon_panel;
		c_uipanel* m_weapon_panel_bg;
		c_uipanel* m_health_armor_panel;
		c_uipanel* m_health_small_bg;

		const char* m_hud_color_hex;
		float m_hud_alpha_hex;
		c_ui_engine* m_uiengine;

		bool m_should_force_update            = false;
		bool m_should_update_winpanel         = false;
		bool m_should_update_deathnotices     = false;
		bool m_should_update_teamcount_avatar = false;

		int m_curr_hud_color{ };
		float m_curr_hud_opacity{ };
		int m_last_hud_color{ };
		float m_last_hud_opacity{ };
	};
} // namespace n_scaleform

inline n_scaleform::impl_t g_scaleform{ };
