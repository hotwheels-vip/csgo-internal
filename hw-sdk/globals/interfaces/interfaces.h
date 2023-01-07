#pragma once

#include <d3dx9.h>
#include <iostream>
#include <string.h>

#include "../../game/sdk/include.h"

#include "../../game/sdk/classes/i_model_cache.h"
#include "../../utils/utils.h"
#include "../../utils/vfunc/vfunc.h"
#include "../macros/macros.h"

namespace sdk
{
	struct interfaces {
		IDirect3DDevice9* device;
		sdk::iv_engine_client* engine;
		sdk::i_client_dll* client;
		sdk::i_global_vars* globals;
		sdk::c_input* input;
		sdk::i_client_entity_list* entity_list;
		sdk::i_game_movement* game_movement;
		sdk::i_prediction* prediction;
		sdk::i_move_helper* move_helper;
		sdk::i_cvar* convar;
		sdk::c_client_state* client_state;
		sdk::i_engine_trace_client* engine_trace;
		sdk::i_engine_sound* engine_sound;
		sdk::c_studio_render* studio_render;
		sdk::i_debug_overlay* debug_overlay;
		sdk::i_material_system* material_system;
		sdk::i_panel* panel;
		sdk::i_localize* localize;
		sdk::iv_model_render* model_render;
		sdk::iv_model_info* model_info;
		sdk::i_model_cache* model_cache;
		void* file_system;
		void* key_values_system;

		bool init( );
	};

	// sry for putting these here, include errors fudge me
	inline std::int32_t time_to_ticks( float time );

	inline float ticks_to_time( std::int32_t ticks );
} // namespace sdk

inline sdk::interfaces g_interfaces;

inline std::int32_t sdk::time_to_ticks( float time )
{
	return static_cast< std::int32_t >( 0.5f + time / g_interfaces.globals->interval_per_tick );
}

inline float sdk::ticks_to_time( std::int32_t ticks )
{
	return static_cast< float >( ticks ) * g_interfaces.globals->interval_per_tick;
}
