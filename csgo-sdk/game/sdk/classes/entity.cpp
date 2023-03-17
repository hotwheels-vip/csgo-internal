#include "entity.h"
#include "../../../globals/includes/includes.h"
#include "../includes/includes.h"

#include <array>

void c_client_entity::set_abs_origin( const c_vector& origin )
{
	static auto set_abs_origin_fn = reinterpret_cast< void( __thiscall* )( void*, const c_vector& ) >(
		g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) );

	set_abs_origin_fn( this, origin );
}

void c_base_entity::invalidate_bone_cache( )
{
	static auto invalidate_bone_cache_address = reinterpret_cast< std::uintptr_t >(
		g_modules[ CLIENT_DLL ].find_pattern( "80 ? ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 ? ? ? ? ? ? ? ? ? 89 ? ? ? ? ? C3" ) );

	reinterpret_cast< void( __thiscall* )( void* ) >( invalidate_bone_cache_address )( this );
}

void c_client_entity::set_abs_angles( const c_angle& angle )
{
	static auto set_abs_angles_fn = reinterpret_cast< void( __thiscall* )( void*, const c_angle& ) >(
		g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) );

	set_abs_angles_fn( this, angle );
}

void c_base_entity::modify_eye_position( const c_animation_state* animation_state, c_vector* position ) const
{
	if ( g_interfaces.m_engine_client->is_hltv( ) || g_interfaces.m_engine_client->is_playing_demo( ) )
		return;

	const auto entity = animation_state->m_entity;
	if ( !entity )
		return;

	const auto ground_entity = g_interfaces.m_client_entity_list->get< c_client_entity >( entity->get_ground_entity_handle( ) );

	if ( !animation_state->m_hit_ground_animation && animation_state->m_duck_amount == 0.f && ground_entity )
		return;

	const auto hashed_bone = entity->get_bone_by_hash( HASH_BT( "head_0" ) );

	if ( hashed_bone == -1 )
		return;

	const auto head_position = entity->get_bone_position( hashed_bone );
	if ( head_position.is_zero( ) )
		return;

	c_vector modified_position = head_position;
	modified_position.m_z += 1.7f;

	if ( modified_position.m_z < position->m_z ) {
		float factor       = 0.f;
		const float delta  = abs( position->m_z - modified_position.m_z );
		const float offset = ( delta - 4.0f ) / 6.0f;

		if ( offset >= 0.f )
			factor = std::min( offset, 1.0f );

		const float factor_squared = ( factor * factor );
		position->m_z += ( ( modified_position.m_z - position->m_z ) * ( ( factor_squared * 3.0f ) - ( ( factor_squared * 2.0f ) * factor ) ) );
	}
}

void c_base_entity::set_next_think( int think )
{
	static auto set_next_think_fn =
		reinterpret_cast< void( __thiscall* )( void*, int ) >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B" ) );

	set_next_think_fn( this, think );
}

void c_base_entity::post_think( )
{
	static auto post_think_vphysics = reinterpret_cast< bool( __thiscall* )( c_base_entity* ) >(
		g_modules[ CLIENT_DLL ].find_pattern( ( ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) ) ) );

	static auto simulate_player_simulated_entities = reinterpret_cast< void( __thiscall* )( c_base_entity* ) >(
		g_modules[ CLIENT_DLL ].find_pattern( ( ( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 74" ) ) ) );

	g_interfaces.m_model_cache->begin_lock( );

	if ( this->is_alive( ) ) {
		this->update_collision_bounds( );

		if ( this->get_flags( ) & e_flags::fl_onground )
			*this->get_fall_velocity( ) = 0.f;

		if ( this->get_sequence( ) == -1 )
			this->set_sequence( 0 );

		this->studio_frame_advance( );
		post_think_vphysics( this );
	}

	simulate_player_simulated_entities( this );

	g_interfaces.m_model_cache->end_lock( );
}

bool c_base_entity::is_valid_player( )
{
	if ( !this || !g_ctx.m_local )
		return false;

	return this->is_alive( ) && !this->is_dormant( ) && this->is_player( ) && this != g_ctx.m_local;
}

bool c_base_entity::is_armored( const int hit_group )
{
	bool ret = false;

	if ( this->get_armor( ) > 0 ) {
		switch ( hit_group ) {
		case e_hitgroup::hitgroup_generic:
		case e_hitgroup::hitgroup_chest:
		case e_hitgroup::hitgroup_stomach:
		case e_hitgroup::hitgroup_leftarm:
		case e_hitgroup::hitgroup_rightarm:
		case e_hitgroup::hitgroup_neck:
			ret = true;
			break;
		case e_hitgroup::hitgroup_head:
			if ( this->has_helmet( ) )
				ret = true;
			[[fallthrough]];
		case e_hitgroup::hitgroup_leftleg:
		case e_hitgroup::hitgroup_rightleg:
			if ( this->has_heavy_armor( ) )
				ret = true;
			break;
		default:
			break;
		}
	}

	return ret;
}

bool c_base_entity::is_breakable( )
{
	const int health = this->get_health( );

	if ( health < 0 && this->get_max_health( ) > 0 )
		return true;

	if ( this->get_take_damage( ) != e_damage_type::damage_yes ) {
		const auto client_renderable = this->get_client_renderable( );
		if ( !client_renderable )
			return false;

		const auto client_unknown = client_renderable->get_client_unknown( );
		if ( !client_unknown )
			return false;

		const auto client_networkable = client_unknown->get_client_networkable( );
		if ( !client_networkable)
			return false;

		const auto client_class = client_networkable->get_client_class( );
		if ( !client_class )
			return false;

		const auto class_id = client_class->m_class_id;
		if ( class_id != e_class_ids::c_func_brush )
			return false;
	}

	if ( const int collision_group = this->get_collision_group( ); collision_group != e_collision_group::collision_group_pushaway &&
	                                                             collision_group !=   e_collision_group::collision_group_breakable_glass &&
	                                                             collision_group !=   e_collision_group::collision_group_none )
		return false;

	if ( health > 200 )
		return false;

	if ( const auto physics_interface = dynamic_cast< c_multiplayer_physics* >( this ); physics_interface ) {
		if ( physics_interface->get_multiplayer_physics_mode( ) != e_multiplayer_physics_mode::physics_multiplayer_solid )
			return false;
	} else {
		if ( const char* class_name = this->get_class_name( );
		     !strcmp( class_name, "func_breakable" ) || !strcmp( class_name, "func_breakable_surf" ) ) {
			if ( !strcmp( class_name, "func_breakable_surf" ) ) {
				const auto surface = static_cast< c_breakable_surface* >( this );

				if ( surface->is_broken( ) )
					return false;
			}
		} else if ( this->physics_solid_mask_for_entity( ) & e_contents::contents_playerclip ) 
			return false;
	}

	if ( const auto breakable_interface = dynamic_cast< c_breakable_with_prop_data* >( this ); breakable_interface ) {
		if ( breakable_interface->get_dmg_mod_bullet( ) <= 0.0f )
			return false;
	}

	return true;
}

bool c_base_entity::is_valid_enemy( )
{
	if ( !this || !g_ctx.m_local )
		return false;

	return this->is_alive( ) && !this->is_dormant( ) && this->is_player( ) && this != g_ctx.m_local && this->is_enemy( g_ctx.m_local );
}

bool c_base_entity::can_shoot( c_base_entity* weapon )
{
	const float server_time = g_interfaces.m_global_vars_base->m_interval_per_tick * static_cast< float >( this->get_tick_base( ) );

	if ( weapon->get_ammo( ) <= 0 )
		return false;

	if ( this->get_next_attack( ) > server_time )
		return false;

	const short item_definition_index = weapon->get_item_definition_index( );
	if ( ( item_definition_index == e_item_definition_index::weapon_famas || item_definition_index == e_item_definition_index::weapon_glock ) &&
	     weapon->is_burst_mode( ) && weapon->get_burst_shots_remaining( ) > 0 )
		return true;

	if ( weapon->get_next_primary_attack( ) > server_time )
		return false;

	if ( item_definition_index == e_item_definition_index::weapon_revolver && weapon->get_fire_ready_time( ) > server_time )
		return false;

	return true;
}

void c_base_entity::restore_data( const char* context, int slot, int type )
{
	static const auto restore_data_fn = reinterpret_cast< void( __thiscall* )( void*, const char*, int, int ) >(
		g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 E4 F8 83 EC 44 53 56 8B" ) );
	// manual fix for deathcam crash
	restore_data_fn( this, context, slot, type );
}

void c_base_entity::on_post_restore_data( )
{
	static const auto on_post_restore_data_fn =
		reinterpret_cast< void( __thiscall* )( void* ) >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 51 53 56 6A" ) );

	on_post_restore_data_fn( this );
}

bool c_base_entity::physics_run_think( int think_method )
{
	static auto physics_run_think_fn =
		reinterpret_cast< bool( __thiscall* )( void*, int ) >( g_modules[ CLIENT_DLL ].find_pattern( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87" ) );

	return physics_run_think_fn( this, think_method );
}

bool c_base_entity::is_enemy( c_base_entity* entity )
{
	if ( !entity )
		return false;

	if ( g_interfaces.m_game_types->get_current_game_type( ) == 6 /* GAMETYPE_FREEFORALL */ )
		return ( this->get_survival_team( ) != entity->get_survival_team( ) );

	static auto mp_teammates_are_enemies = g_convars[ HASH_BT( "mp_teammates_are_enemies" ) ];

	if ( mp_teammates_are_enemies && mp_teammates_are_enemies->get_bool( ) && this->get_team( ) == entity->get_team( ) && this != entity )
		return true;

	if ( this->get_team( ) != entity->get_team( ) )
		return true;

	return false;
}

bool c_base_entity::get_bounding_box( bounding_box_t* box )
{
	const auto client_renderable = this->get_client_renderable( );
	if ( !client_renderable )
		return false;

	const auto client_unknown = client_renderable->get_client_unknown( );
	if ( !client_unknown )
		return false;

	const auto collideable = client_unknown->get_collideable( );
	if ( !collideable )
		return false;

	/* https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/game/shared/collisionproperty.h#L77 */
	const c_vector mins = collideable->get_obb_mins( );
	const c_vector maxs = collideable->get_obb_maxs( );

	std::array< c_vector, 8U > points = { c_vector( mins.m_x, mins.m_y, mins.m_z ), c_vector( mins.m_x, maxs.m_y, mins.m_z ),
		                                  c_vector( maxs.m_x, maxs.m_y, mins.m_z ), c_vector( maxs.m_x, mins.m_y, mins.m_z ),
		                                  c_vector( maxs.m_x, maxs.m_y, maxs.m_z ), c_vector( mins.m_x, maxs.m_y, maxs.m_z ),
		                                  c_vector( mins.m_x, mins.m_y, maxs.m_z ), c_vector( maxs.m_x, mins.m_y, maxs.m_z ) };

	if ( !( points.data( ) ) )
		return false;

	const matrix3x4_t& transformed_matrix = this->get_coordinate_frame( );

	float left   = std::numeric_limits< float >::max( );
	float top    = std::numeric_limits< float >::max( );
	float right  = std::numeric_limits< float >::lowest( );
	float bottom = std::numeric_limits< float >::lowest( );

	std::array< c_vector_2d, 8U > screen_points = { };
	for ( std::size_t i = 0U; i < 8U; i++ ) {
		if ( !g_render.world_to_screen( g_math.vector_transform( points[ i ], transformed_matrix ), screen_points[ i ] ) )
			return false;

		left   = std::min( left, screen_points[ i ].m_x );
		top    = std::min( top, screen_points[ i ].m_y );
		right  = std::max( right, screen_points[ i ].m_x );
		bottom = std::max( bottom, screen_points[ i ].m_y );
	}

	box->m_left   = static_cast< int >( left );
	box->m_top    = static_cast< int >( top );
	box->m_right  = static_cast< int >( right );
	box->m_bottom = static_cast< int >( bottom );
	box->m_width  = static_cast< int >( right ) - static_cast< int >( left );
	box->m_height = static_cast< int >( bottom ) - static_cast< int >( top );
	return true;
}

int c_base_entity::get_bone_by_hash( const unsigned int hash ) const
{
	if ( const auto model = this->get_model( ); model ) {
		if ( const auto studio_model = g_interfaces.m_model_info->get_studio_model( model ); studio_model ) {
			for ( int i = 0; i < studio_model->n_bones; i++ ) {
				if ( const auto bone = studio_model->get_bone( i ); bone && HASH_RT( bone->get_name( ) ) == hash )
					return i;
			}
		}
	}

	return -1;
}

int c_base_entity::get_max_health( )
{
	if ( g_interfaces.m_game_types->get_current_game_type( ) == 6 /* GAMETYPE_FREEFORALL */ )
		return 120;

	return 100;
}

int& c_base_entity::get_take_damage( )
{
	static const unsigned int take_damage_fn =
		*reinterpret_cast< unsigned int* >( g_modules[ CLIENT_DLL ].find_pattern( "80 BE ? ? ? ? ? 75 46 8B 86" ) + 0x2 );

	return *reinterpret_cast< int* >( reinterpret_cast< unsigned int >( this ) + take_damage_fn );
}

c_vector c_base_entity::get_bone_position( int bone )
{
	matrix3x4_t bone_matrix[ 128 ]{ };
	memcpy( bone_matrix, this->get_cached_bone_data( ).get_elements( ), this->get_cached_bone_data( ).count( ) * sizeof( matrix3x4_t ) );

	if ( !bone_matrix )
		return { };

	return bone_matrix[ bone ][ 3 ];
}

c_vector c_base_entity::get_hitbox_position( int index, float point_scale )
{
	if ( auto model = get_model( ) ) {
		if ( auto studio_model = g_interfaces.m_model_info->get_studio_model( model ) ) {
			matrix3x4_t matrix[ 128 ];

			if ( setup_bones( matrix, 128, 0x100, 0.f ) ) {
				if ( auto hitbox_set_ = studio_model->get_hitbox_set( get_hitbox_set( ) ) ) {
					if ( auto hitbox = hitbox_set_->get_hitbox( index ) ) {
						auto position = ( hitbox->m_bb_min + hitbox->m_bb_max ) * point_scale;

						return g_math.vector_transform( position, matrix[ hitbox->m_bone ] );
					}
				}
			}
		}
	}

	return { };
}

c_vector c_base_entity::get_hitbox_position( int hitbox, matrix3x4_t* matrix, float point_scale )
{
	auto hdr = g_interfaces.m_model_info->get_studio_model( get_model( ) );
	if ( !hdr )
		return { };

	auto hitbox_set = hdr->get_hitbox_set( get_hitbox_set( ) );
	[[unlikely]] if ( !hitbox_set )
		return { };

	auto new_hitbox = hitbox_set->get_hitbox( hitbox );
	if ( !new_hitbox )
		return { };

	return g_math.vector_transform( ( new_hitbox->m_bb_min + new_hitbox->m_bb_max ) * point_scale, matrix[ new_hitbox->m_bone ] );
}

c_vector c_base_entity::get_eye_position( bool should_correct )
{
	c_vector out{ };

	g_virtual.call< void >( this, 169, std::ref( out ) );

	if ( this->is_used_new_animation_state( ) && should_correct ) {
		if ( const auto animation_state = this->get_animation_state( ); animation_state )
			this->modify_eye_position( animation_state, &out );
	}

	return out;
}

bool c_base_entity::can_see_player( c_base_entity* player )
{
	if ( !this || !player || !player->is_alive( ) || player->is_dormant( ) )
		return false;

	c_game_trace trace;
	c_trace_filter filter( this );
	ray_t ray( get_eye_position( false ), player->get_hitbox_position( e_hitboxes::hitbox_head, player->get_cached_bone_data( ).base( ) ) );

	g_interfaces.m_engine_trace->trace_ray( ray, mask_shot | contents_grate, &filter, &trace );

	return trace.m_hit_entity == player || trace.m_fraction > 0.97f;
}

c_user_cmd& c_base_entity::get_last_command( )
{
	static const unsigned int get_last_command_fn =
		*reinterpret_cast< unsigned int* >( g_modules[ CLIENT_DLL ].find_pattern( "8D 8E ? ? ? ? 89 5C 24 3C" ) + 0x2 );

	return *reinterpret_cast< c_user_cmd* >( reinterpret_cast< unsigned int >( this ) + get_last_command_fn );
}

bool c_base_entity::can_see_matrix( c_base_entity* player, matrix3x4_t* matrix )
{
	if ( !this || !matrix || !player || !player->is_alive( ) || player->is_dormant( ) )
		return false;

	c_game_trace trace;
	c_trace_filter filter( this );
	ray_t ray( get_eye_position( false ), player->get_hitbox_position( e_hitboxes::hitbox_head, matrix ) );

	g_interfaces.m_engine_trace->trace_ray( ray, mask_shot | contents_grate, &filter, &trace );

	return trace.m_hit_entity == player || trace.m_fraction > 0.97f;
}
