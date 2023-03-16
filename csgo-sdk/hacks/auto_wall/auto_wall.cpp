#include "auto_wall.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

float n_auto_wall::impl_t::get_damage( c_base_entity* weapon, const c_vector& point, fire_bullet_data_t* data_out )
{
	const c_vector position = g_ctx.m_local->get_eye_position( );

	fire_bullet_data_t data = { };
	data.m_position      = position;
	data.m_direction     = ( point - position ).normalized( );

	if ( !simulate_fire_bullet( weapon, data ) )
		return -1.0f;

	if ( data_out != nullptr )
		*data_out = data;

	return data.m_current_damage;
}