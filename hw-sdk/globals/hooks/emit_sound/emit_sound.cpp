#include "emit_sound.h"
#include "../../../hacks/prediction/prediction.h"
#include "../../ctx/ctx.h"
#include <type_traits>

void __fastcall hooks::emit_sound::emit_sound_detour( void* ecx, void* edx, void* filter, int entity_index, int channel, const char* sound_entry,
                                                      unsigned int sound_entry_hash, const char* sample, float volume, int seed, int sound_level,
                                                      int flags, int pitch, const math::vec3* origin, const math::vec3* direction,
                                                      void* utl_vec_origins, bool update_positions, float soundtime, int speaker_entity,
                                                      void* sound_params )
{
	if ( !g_prediction.in_prediction )
		emit_sound_hook.call_original< void >( ecx, edx, filter, entity_index, channel, sound_entry, sound_entry_hash, sample, volume, seed,
		                                       sound_level, flags, pitch, origin, direction, utl_vec_origins, update_positions, soundtime,
		                                       speaker_entity, sound_params );
}
