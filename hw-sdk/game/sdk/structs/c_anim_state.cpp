#include "c_anim_state.h"

void sdk::c_anim_state::modify_eye_position( math::vec3& input_eye_pos )
{
	if ( !this )
		return;

	if ( player && ( landing || anim_duck_amount != 0.f || !player->flags( ).has( sdk::flags::ONGROUND ) ) ) {
		g_interfaces.model_cache->begin_lock( );

		math::vec3 head_pos = player->hitbox_position( sdk::HITGROUP_HEAD );
		head_pos += 1.7f;

		if ( head_pos.z < input_eye_pos.z ) {
			float lerp{ };
			if ( float abs_pos = ( fabs( input_eye_pos.z - head_pos.z ) - 4.f ) * 0.16666667f; abs_pos >= 0.f )
				lerp = std::min( abs_pos, 1.f );

			input_eye_pos.z =
				( ( head_pos.z - input_eye_pos.z ) * ( ( ( lerp * lerp ) * 3.f ) - ( ( ( lerp * lerp ) * 2.f ) * lerp ) ) ) + input_eye_pos.z;
		}

		g_interfaces.model_cache->end_lock( );
	}
}
