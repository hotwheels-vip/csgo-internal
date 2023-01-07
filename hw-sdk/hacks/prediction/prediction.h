#pragma once

#include "../../globals/includes/includes.h"

#include "../../game/sdk/classes/c_base_entity.h"
#include "../../game/sdk/classes/c_client_state.h"
#include "../../game/sdk/classes/i_prediction.h"

namespace prediction
{
	struct impl {
		// NOTE @ liga: since we're gonna run prediction alot of times(edgebug, jumpbug and other movement features) we need to have 2 types of
		// 'backup' vars, one being curtime, frametime, tick shit and one being stuff like flags, velocity since we want to use original values for
		// them and not one that has been restored 1 bajillion times.

		bool in_prediction = false;

		struct {
			int tick_base;
			float current_time, frame_time;
			sdk::move_data_t move_data;
		} reset_vars;

		struct {
			math::vec3 velocity;
			math::vec3 view_angles;
			float fall_velocity;
			sdk::int_flag_t flags;
			sdk::int_flag_t move_type;
		} backup_vars;

		// gets players tickbase correctly
		int get_tick_base( sdk::c_base_player* player );
		// starts prediction and sets all global variables to predict next tick
		void start( sdk::c_base_player* player );
		// resets all global variables to current tick, doesnt restore predicted values
		void end( sdk::c_base_player* player );
		// updates prediction incase of fps lower then tickrate
		void update( );
		// resets everything and returns to non predicted tick
		void reset( );
		// store all backup vars
		void store_backup( );

	private:
		bool m_backup_is_first_time_predicted;
		bool m_backup_in_prediction;
	};
} // namespace prediction

inline prediction::impl g_prediction;
