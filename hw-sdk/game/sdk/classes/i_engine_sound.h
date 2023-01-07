#pragma once
#include "../../../utils/math/types/vector.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../datatypes/cutl.h"

namespace sdk
{
	struct sound_info {
		int g_uid;
		void* name_handle;
		int sound_source;
		int channel;
		int speaker_ent;
		float volume;
		float last_spatialized_volume;
		float radius;
		int pitch;
		math::vec3* origin;
		math::vec3* direction;
		bool update_positions;
		bool is_sentence;
		bool dry_mix;
		bool speaker;
		bool from_server;
	};

	class i_engine_sound
	{
	public:
		// returns servers active sounds to vector list
		void get_active_sounds( sdk::datatypes::c_utl_vector< sdk::sound_info >& sound_list )
		{
			virtual_func::call< void*, sdk::datatypes::c_utl_vector< sdk::sound_info >& >( this, 19, sound_list );
		}
	};
} // namespace sdk
