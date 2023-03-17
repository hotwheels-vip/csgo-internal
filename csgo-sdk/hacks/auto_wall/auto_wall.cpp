#include "auto_wall.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

float n_auto_wall::impl_t::get_damage( c_base_entity* local, const c_vector& point, fire_bullet_data_t* data_out )
{
	const auto position = local->get_eye_position( );

	fire_bullet_data_t data = { };
	data.m_position         = position;
	data.m_direction        = ( point - position ).normalized( );

	if ( const auto weapon = g_interfaces.m_client_entity_list->get< c_base_entity >( local->get_active_weapon_handle( ) );
	     !weapon || !simulate_fire_bullet( weapon, data ) )
		return -1.0f;

	if ( data_out )
		*data_out = data;

	return data.m_current_damage;
}

void n_auto_wall::impl_t::scale_damage( const int hit_group, c_base_entity* entity, const float weapon_armor_ratio,
                                        const float weapon_head_shot_multiplier, float& damage )
{
	const auto has_heavy_armor = entity->has_heavy_armor( );
	const auto team            = entity->get_team( );

	static auto mp_damage_scale_ct_head = g_convars[ HASH_BT( "mp_damage_scale_ct_head" ) ];
	static auto mp_damage_scale_t_head  = g_convars[ HASH_BT( "mp_damage_scale_t_head" ) ];

	float head_damage_scale = team == e_team_id::team_ct   ? mp_damage_scale_ct_head->get_float( )
	                          : team == e_team_id::team_tt ? mp_damage_scale_t_head->get_float( )
	                                                       : 1.0f;

	static auto mp_damage_scale_ct_body = g_convars[ HASH_BT( "mp_damage_scale_ct_body" ) ];
	static auto mp_damage_scale_t_body  = g_convars[ HASH_BT( "mp_damage_scale_t_body" ) ];

	const float body_damage_scale = team == e_team_id::team_ct   ? mp_damage_scale_ct_body->get_float( )
	                                : team == e_team_id::team_tt ? mp_damage_scale_t_body->get_float( )
	                                                             : 1.0f;

	if ( has_heavy_armor )
		head_damage_scale *= 0.5f;

	switch ( hit_group ) {
	case e_hitgroup::hitgroup_head:
		damage *= weapon_head_shot_multiplier * head_damage_scale;
		break;
	case e_hitgroup::hitgroup_chest:
	case e_hitgroup::hitgroup_leftarm:
	case e_hitgroup::hitgroup_rightarm:
	case e_hitgroup::hitgroup_neck:
		damage *= body_damage_scale;
		break;
	case e_hitgroup::hitgroup_stomach:
		damage *= 1.25f * body_damage_scale;
		break;
	case e_hitgroup::hitgroup_leftleg:
	case e_hitgroup::hitgroup_rightleg:
		damage *= 0.75f * body_damage_scale;
		break;
	default:
		break;
	}

	if ( entity->is_armored( hit_group ) ) {
		// @ida ontakedamage: server.dll @ 80 BF ? ? ? ? ? F3 0F 10 5C 24 ? F3 0F 10 35

		const int armor        = entity->get_armor( );
		float heavy_armor_bonus = 1.0f, armor_bonus = 0.5f, armor_ratio = weapon_armor_ratio * 0.5f;

		if ( has_heavy_armor ) {
			heavy_armor_bonus = 0.25f;
			armor_bonus      = 0.33f;
			armor_ratio *= 0.20f;
		}

		float damage_to_health = damage * armor_ratio;
		if ( const float damage_to_armor = ( damage - damage_to_health ) * ( heavy_armor_bonus * armor_bonus );
		     damage_to_armor > static_cast< float >( armor ) )
			damage_to_health = damage - static_cast< float >( armor ) / armor_bonus;

		damage = damage_to_health;
	}
}