#pragma once
#include "../../../../game/sdk/classes/c_engine_sound.h"
#include "../../../../game/sdk/includes/includes.h"
#include "../../../../globals/includes/includes.h"
#include "../../../entity_cache/entity_cache.h"
#include <array>

// this but better
// https://github.com/sstokic-tgm/Gladiatorcheatz-v2.1/blob/master/features/ServerSounds.hpp

constexpr static const int EXPIRE_DURATION = 550;

namespace n_dormancy
{
	struct impl_t {
	public:
		struct entity_sound_t {
			void override_info( sound_info_t& sound )
			{
				m_index        = sound.m_sound_source;
				m_origin       = *sound.m_origin;
				m_receive_time = GetTickCount64( );
			}

			int m_index             = 0;
			int m_receive_time      = 0;
			c_vector m_origin       = { };
			int m_flags             = 0;
			c_base_entity* m_player = nullptr;
			c_vector m_abs_origin   = c_vector( 0, 0, 0 );
			bool m_dormant          = false;
		} m_sound_players[ 65 ];

		void start( );
		void finish( );

	private:
		void adjust_player_begin( c_base_entity* player );
		void adjust_player_finish( );

		void setup_adjust_player( c_base_entity* player, sound_info_t& sound );

		bool is_valid_sound( sound_info_t& sound );

		c_utl_vector< sound_info_t > m_vec_sound_buffer{ };
		c_utl_vector< sound_info_t > m_cur_sound_list{ };

		// sorry
		std::vector< entity_sound_t > m_array_restore_players{ };
	};
} // namespace n_dormancy

inline n_dormancy::impl_t g_dormancy{ };
