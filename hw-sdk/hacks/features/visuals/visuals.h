#pragma once

#include "../../../game/sdk/classes/c_cs_player.h"
#include "../../../game/sdk/classes/i_collideable.h"
#include "../../../game/sdk/enums/hitgroup.h"
#include "../../../game/sdk/include.h"
#include "../../../globals/includes/includes.h"
#include "../../../utils/console/console.h"
#include "../../../utils/entity_list/entity_list.h"
#include "../../../utils/renderer/renderer.h"

#include <deque>

namespace visuals
{
	// Welcome to the great wall of esp. I have designed this as a nice object oriented way to
	// Do esp or visuals. If you want to create a new object follow the esp_box struct exactly.
	// Make a render function, all helper functions must be private! If they use rgfl matrix or
	// Use any function that relies i_collidable update them in paint like I do with esp_box rgfl.
	// This is an issue with CS:GO and multicore rendering and I plan on having more than 2 FPS.

	enum class esp_location : int
	{
		LOCATION_TOP = 0,
		LOCATION_LEFT,
		LOCATION_BOTTOM,
		LOCATION_RIGHT
	};

	struct esp_text {
	public:
		void render( math::box box, float& offset_x, float& offset_y );

		std::string m_text{ };

		color m_color{ };

		esp_location m_location{ };

		LPD3DXFONT m_font{ };

		font_flags m_flags{ };
	};

	struct esp_title : public esp_text {
	public:
		void render( math::box box, float& offset_x, float& offset_y );
	};

	struct esp_bar : public esp_text {
	public:
		void render( math::box box, float& offset_x, float& offset_y );

		float m_width{ };

		color m_color_from{ };
		color m_color_to{ };

		float m_max{ };
		float m_min{ };
		float m_cur{ };
	};

	struct esp_box {
	private:
		math::box calculate_box( sdk::c_cs_player* player, bool& on_screen );

	public:
		void render( sdk::c_cs_player* owner );

		bool m_outline[ 2 ]{ }; // 0 = outside, 1 = inside.
		bool m_cornered{ };

		color m_color{ };

		bool m_draw{ };

		bool m_skeleton{ };
		bool m_backtrack{ };

		std::deque< esp_title > m_titles{ };
		std::deque< esp_text > m_texts{ };
		std::deque< esp_bar > m_bars{ };
	};

	struct esp_object {
	public:
		esp_object( ) = default;
		esp_object( sdk::c_cs_player* owner ) : m_owner( owner ){ };

		sdk::c_cs_player* m_owner{ };

		esp_box m_box{ };
	};

	struct impl {
	private:
		void update_object( esp_object& object );
	public:
		std::array< esp_object, 65 > esp_objects{ };

		void update( );
		void render( );
	};
} // namespace visuals

inline visuals::impl g_visuals;
