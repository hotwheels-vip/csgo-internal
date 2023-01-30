#pragma once
#include "../netvars/netvars.h"

#include "c_model_info.h"
#include "c_user_cmd.h"
#include "c_vector.h"

#include "../classes/c_weapon_data.h"
#include "../structs/bounding_box_t.h"
#include "../structs/matrix_t.h"

class c_base_entity;

/* TODO ~ move to another file (enumerations folder) */
enum e_obs_mode : int {
	obs_mode_none = 0,
	obs_mode_deathcam,
	obs_mode_freezecam,
	obs_mode_fixed,
	obs_mode_in_eye,
	obs_mode_chase,
	obs_mode_roaming
};

enum e_precipitation_type : int {
	precipitation_type_rain = 0,
	precipitation_type_snow,
	precipitation_type_ash,
	precipitation_type_snowfall,
	precipitation_type_particlerain,
	precipitation_type_particleash,
	precipitation_type_particlerainstorm,
	precipitation_type_particlesnow,
	num_precipitation_types
};

class c_player_animation_layer
{
public:
	bool client_bleed;
	float blend_in;
	void* studio_hdr;
	int dispatch_sequence;
	int dispatch_sequence2;
	int order;
	int sequence;
	float prev_cycle;
	float weight;
	float weight_delta_rate;
	float playback_rate;
	float cycle;
	void* owner;
	PAD( 4 );
};

constexpr float max_health = 100.f /* todo ~ dont hard codenz check game type */;

class c_base_client;

enum e_glow_style : int {
	glow_style_default = 0,
	glow_style_rim_3d,
	glow_style_edge_highlight,
	glow_style_edge_highlight_pulse,
	glow_style_count
};

class c_collideable
{
private:
	enum e_indexes {
		_obb_mins = 1,
		_obb_maxs = 2,
	};

public:
	c_vector& obb_mins( )
	{
		using fn = c_vector&( __thiscall* )( c_collideable* );
		return ( *( fn** )this )[ this->e_indexes::_obb_mins ]( this );
	}

	c_vector& obb_maxs( )
	{
		using fn = c_vector&( __thiscall* )( c_collideable* );
		return ( *( fn** )this )[ this->e_indexes::_obb_maxs ]( this );
	}
};

class c_client_unknown
{
	enum e_indexes {
		_get_base_entity = 7,
	};

public:
	c_base_entity* get_base_entity( )
	{
		using fn = c_base_entity*( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_get_base_entity ]( this );
	}
};

class c_client_networkable
{
private:
	enum e_indexes {
		_release             = 1,
		_get_client_class    = 2,
		_on_pre_data_changed = 4,
		_on_data_changed     = 5,
		_pre_data_update     = 6,
		_post_data_update    = 7
	};

public:
	void release( )
	{
		using fn = void( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_release ]( this );
	}

	c_base_client* get_client_class( )
	{
		using fn = c_base_client*( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_get_client_class ]( this );
	}

	void on_pre_data_changed( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_on_pre_data_changed ]( this, update_type );
	}

	void on_data_changed( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_on_data_changed ]( this, update_type );
	}

	void pre_data_update( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_pre_data_update ]( this, update_type );
	}

	void post_data_update( int update_type )
	{
		using fn = void( __thiscall* )( void*, int );
		return ( *( fn** )this )[ this->e_indexes::_post_data_update ]( this, update_type );
	}
};

class c_client_renderable
{
private:
	enum e_indexes {
		_get_client_unknown = 0,
		_model              = 7,
	};

public:
	c_client_unknown* get_client_unknown( )
	{
		using fn = c_client_unknown*( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_get_client_unknown ]( this );
	}

	model_t* model( )
	{
		using fn = model_t*( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_model ]( this );
	}
};

class c_base_entity
{
private:
	enum e_indexes {
		_collideable             = 3,
		_client_networkable      = 4,
		_client_renderable       = 5,
		_client_class            = 2,
		_abs_origin              = 10,
		_data_desc_map           = 15,
		_prediction_desc_map     = 17,
		_think                   = 139,
		_set_sequence            = 219,
		_pre_think               = 318,
		_is_player               = 158,
		_studio_frame_advance    = 220,
		_select_item             = 330,
		_update_collision_bounds = 340
	};

public:
	data_map_t* data_desc_map( )
	{
		using fn = data_map_t*( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_data_desc_map ]( this );
	}

	data_map_t* prediction_desc_map( )
	{
		using fn = data_map_t*( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_prediction_desc_map ]( this );
	}

	c_collideable* collideable( )
	{
		using fn = c_collideable*( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_collideable ]( this );
	}

	c_client_networkable* client_networkable( )
	{
		using fn = c_client_networkable*( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_client_networkable ]( this );
	}

	c_client_renderable* client_renderable( )
	{
		using fn = c_client_renderable*( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_client_renderable ]( this );
	}

	bool is_player( )
	{
		using fn = bool( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_is_player ]( this );
	}

	void set_sequence( int sequence )
	{
		using fn = void( __thiscall* )( c_base_entity*, int );
		return ( *( fn** )this )[ this->e_indexes::_set_sequence ]( this, sequence );
	}

	bool is_alive( )
	{
		return ( this->life_state( ) == 0 /* e_life_states::life_state_alive */ );
	}

	void select_item( const char* str, int sub_type )
	{
		using fn = void( __thiscall* )( c_base_entity*, const char*, int );
		return ( *( fn** )this )[ this->e_indexes::_select_item ]( this, str, sub_type );
	}

	void think( )
	{
		using fn = void( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_think ]( this );
	}

	void studio_frame_advance( )
	{
		using fn = void( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_studio_frame_advance ]( this );
	}

	void pre_think( )
	{
		using fn = void( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_pre_think ]( this );
	}

	void update_collision_bounds( )
	{
		using fn = void( __thiscall* )( c_base_entity* );
		return ( *( fn** )this )[ this->e_indexes::_update_collision_bounds ]( this );
	}

	bool physics_run_think( int think_method );
	bool is_visible( c_base_entity* entity, const c_vector& end_position );
	bool get_bounding_box( bounding_box_t* bbox );
	/*bool can_shoot( );*/
	c_user_cmd& last_command( );
	void post_think( );
	void set_next_think( int think );
	void set_abs_origin( const c_vector& origin );

	c_vector eye_position( )
	{
		return origin( ) + view_offset( );
	}

	add_variable( int, health, "CBasePlayer->m_iHealth" );
	add_variable( int, life_state, "CBasePlayer->m_lifeState" );
	add_variable( int, flags, "CBasePlayer->m_fFlags" );
	add_variable( int, tick_base, "CBasePlayer->m_nTickBase" );
	add_variable( float, duck_amount, "CBasePlayer->m_flDuckAmount" );
	add_variable( bool, ducked, "CBasePlayer->m_bDucked" );
	add_variable( c_vector, velocity, "CBasePlayer->m_vecVelocity[0]" );
	add_variable( c_vector, view_offset, "CBasePlayer->m_vecViewOffset[0]" );
	add_variable( c_angle, aim_punch_angle, "CBasePlayer->m_aimPunchAngle" );
	add_pvariable( float, fall_velocity, "CBasePlayer->m_flFallVelocity" );
	add_pvariable( int, next_think_tick, "CBasePlayer->m_nNextThinkTick" );

	/* CBaseEntity */
	add_variable( int, team, "CBaseEntity->m_iTeamNum" );
	add_variable( c_vector, origin, "CBaseEntity->m_vecOrigin" );
	add_variable( int, model_index, "CBaseEntity->m_nModelIndex" );

	c_vector& abs_origin( )
	{
		using fn = c_vector&( __thiscall* )( void* );
		return ( *( fn** )this )[ this->e_indexes::_abs_origin ]( this );
	}

	int get_bone_by_hash( const unsigned int bone_hash );

	void get_bone_position( const int bone, c_vector& origin );

	c_player_animation_layer* get_anim_layers( );

	c_player_animation_layer* get_anim_layer( const int i );

	void get_animation_layers( c_player_animation_layer* layers );

	void set_animation_layers( c_player_animation_layer* layers );

	bool has_bomb( );

	int get_sequence_activity( int sequence );

	bool reloading( );

	/* CCSPlayer */
	add_variable( int, money, "CCSPlayer->m_iAccount" );
	add_variable( int, armor, "CCSPlayer->m_ArmorValue" );
	add_variable( bool, helmet, "CCSPlayer->m_bHasHelmet" );
	add_variable( float, velocity_modifier, "CCSPlayer->m_flVelocityModifier" );
	add_variable( c_vector, player_patch_econ_indices, "CCSPlayer->m_vecPlayerPatchEconIndices" );

	/* CBaseAnimating */
	add_variable( int, sequence, "CBaseAnimating->m_nSequence" );

	/* CBaseCombatCharacter */
	add_variable( unsigned int, active_weapon_handle, "CBaseCombatCharacter->m_hActiveWeapon" );
	add_pvariable( unsigned int, weapons_handle, "CBaseCombatCharacter->m_hMyWeapons" );
	add_variable( unsigned int, get_observer_target_handle, "CBasePlayer->m_hObserverTarget" );
	add_variable( int, get_observer_mode, "CBasePlayer->m_iObserverMode" );
	add_variable( float, get_next_attack, "CBaseCombatCharacter->m_flNextAttack" );

	/* CWeaponCSBase */
	add_variable( bool, is_burst_mode, "CWeaponCSBase->m_bBurstMode" );
	add_variable( float, fire_ready_time, "CWeaponCSBase->m_flPostponeFireReadyTime" );

	/* CWeaponCSBaseGun */
	add_variable( int, burst_shots_remaining, "CWeaponCSBaseGun->m_iBurstShotsRemaining" );

	/* CBaseCombatWeapon */
	add_variable( float, next_primary_attack, "CBaseCombatWeapon->m_flNextPrimaryAttack" );
	add_variable( float, next_secondary_attack, "CBaseCombatWeapon->m_flNextSecondaryAttack" );
	add_variable( int, ammo, "CBaseCombatWeapon->m_iClip1" );
	add_variable( int, ammo_reserve, "CBaseCombatWeapon->m_iPrimaryReserveAmmoCount" );

	/* CBaseAttributableItem */
	add_variable( short, item_definition_index, "CBaseAttributableItem->m_iItemDefinitionIndex" );

	/* datamap variables */
	add_datafield( int, move_type, this->prediction_desc_map( ), "m_MoveType" );
	add_datafield( const matrix3x4_t, coordinate_frame, this->data_desc_map( ), "m_rgflCoordinateFrame" );
	add_datafield( int, eflags, this->prediction_desc_map( ), "m_iEFlags" );
	add_pdatafield( int, buttons, this->prediction_desc_map( ), "m_nButtons" );
	add_datafield( int, button_last, this->prediction_desc_map( ), "m_afButtonLast" );
	add_datafield( int, button_pressed, this->prediction_desc_map( ), "m_afButtonPressed" );
	add_datafield( int, button_released, this->prediction_desc_map( ), "m_afButtonReleased" );
	add_pdatafield( int, impulse, this->prediction_desc_map( ), "m_nImpulse" );
	add_datafield( unsigned int, ground_entity, this->prediction_desc_map( ), "m_hGroundEntity" );
	add_datafield( float, stamina, this->prediction_desc_map( ), "m_flStamina" );

	/* offsets */
	add_offset( unsigned int, index, 0x64 );

	add_offset( bool, is_dormant, 0xED );
	add_offset( bool, should_use_new_animation_state, 0x9B14 );

	add_variable_offset( int, is_used_new_animation_state, "CCSPlayer->m_flLastExoJumpTime", 0x8 );
	add_variable_offset( int, button_disabled, "CBasePlayer->m_hViewEntity", -0xC );
	add_variable_offset( int, button_forced, "CBasePlayer->m_hViewEntity", -0x8 );
	add_pvariable_offset( c_user_cmd*, current_command, "CBasePlayer->m_hViewEntity", -0x4 );
};

class c_fog_controller : public c_base_entity
{
public:
	add_variable( int, enable, "CFogController->m_fog.enable" );
	add_variable( float, start, "CFogController->m_fog.start" );
	add_variable( float, end, "CFogController->m_fog.end" );
	add_variable( float, density, "CFogController->m_fog.maxdensity" );
	add_variable( int, color, "CFogController->m_fog.colorPrimary" );
	add_variable( int, color_secondary, "CFogController->m_fog.colorSecondary" );
};

class c_precipitation : public c_base_entity
{
public:
	add_variable( e_precipitation_type, precipitation_type, "CPrecipitation->m_nPrecipType" );
};
