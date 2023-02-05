#include "../../../game/sdk/includes/includes.h"
#include "../../../globals/includes/includes.h"
#include "../hooks.h"

void __stdcall n_detoured_functions::emit_sound( void* filter, int idx, int channel, const char* sound_entry, unsigned int sound_entry_hash,
                                                 const char* sample, float volume, int seed, float attenuation, int flags, int pitch,
                                                 const c_vector* origin, const c_vector* direction, void* vec_origins, bool update_pos,
                                                 float soundtime, int speakerentity, int unk )
{
	static auto original = g_hooks.m_emit_sound.get_original< decltype( &n_detoured_functions::emit_sound ) >( );

	if ( g_interfaces.m_prediction->m_in_prediction && !g_interfaces.m_prediction->m_is_first_time_predicted )
		return;

	original( filter, idx, channel, sound_entry, sound_entry_hash, sample, volume, seed, attenuation, flags, pitch, origin, direction, vec_origins,
	          update_pos, soundtime, speakerentity, unk );
}