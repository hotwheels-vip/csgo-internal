#include "c_anim_state.h"
#include "../interfaces/interfaces.h"
#include "c_base_entity.h"

void c_anim_state::modify_eye_position( c_vector& input_eye_pos )
{
	if ( interfaces.m_engine->is_hltv( ) || interfaces.m_engine->is_playing_demo( ) )
		return;

	if ( !this->player )
		return;

	const auto ground_entity = interfaces.m_client_entity_list->get_client_entity_from_handle( this->player->ground_entity( ) );
	if ( !this->landing && this->anim_duck_amount == 0.f && ground_entity )
		return;

	c_vector head_position = this->player->get_bone_position( e_hitgroup::hitgroup_head );
	if ( head_position.is_zero( ) )
		return;

	head_position.m_z += 1.7f;

	if ( head_position.m_z < input_eye_pos.m_z ) {
		float factor       = 0.f;
		const float delta  = std::fabsf( input_eye_pos.m_z - head_position.m_z );
		const float offset = ( delta - 4.0f ) / 6.0f;

		if ( offset >= 0.f )
			factor = std::min( offset, 1.0f );

		const float sqaured_factor = ( factor * factor );
		input_eye_pos.m_z += ( ( head_position.m_z - input_eye_pos.m_z ) * ( ( sqaured_factor * 3.0f ) - ( ( sqaured_factor * 2.0f ) * factor ) ) );
	}
}
