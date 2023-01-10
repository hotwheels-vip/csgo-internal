#include "c_anim_state.h"
#include "../interfaces/interfaces.h"
#include "c_base_entity.h"

void c_anim_state::modify_eye_position( c_vector& input_eye_pos )
{
	if ( !this )
		return;

	auto player = this->player;

	if ( player && ( this->landing || this->anim_duck_amount != 0.f || !player->ground_entity( ) ) ) {
		interfaces.m_mdl_cache->begin_lock( );

		if ( int head_bone = player->lookup_bone( "head_0" ); head_bone != -1 ) {
			c_vector head_pos;
			player->get_bone_position( head_bone, head_pos );
			head_pos.m_z += 1.7f;
			if ( head_pos.m_z < input_eye_pos.m_z ) {
				float lerp{ };
				float abs_pos = ( std::fabs( input_eye_pos.m_z - head_pos.m_z ) - 4.f ) * 0.16666667f;
				if ( abs_pos >= 0.f )
					lerp = std::min( abs_pos, 1.f );

				input_eye_pos.m_z = ( ( head_pos.m_z - input_eye_pos.m_z ) * ( ( ( lerp * lerp ) * 3.f ) - ( ( ( lerp * lerp ) * 2.f ) * lerp ) ) ) +
				                    input_eye_pos.m_z;
			}
		}

		interfaces.m_mdl_cache->end_lock( );
	}
}
