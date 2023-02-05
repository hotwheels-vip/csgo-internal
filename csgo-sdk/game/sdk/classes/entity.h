#pragma once
#include "../../../globals/netvars/netvars.h"
#include "../../../utilities/memory/virtual.h"

enum e_data_update_type;
enum class e_class_id;

class c_vector;
class c_angle;
class c_client_unknown;
class c_game_trace;
class c_vector;
class c_angle;
class bf_read;

struct ray_t;
struct model_t;
struct matrix3x4_t;
struct data_map_t;

class c_handle_entity
{
public:
	virtual ~c_handle_entity( ) { }
	virtual void set_ref_ehandle( const unsigned int& ref ) = 0;
	virtual const unsigned int& get_ref_ehandle( ) const    = 0;
};

class c_collideable
{
public:
	virtual c_handle_entity* get_entity_handle( )                                                 = 0;
	virtual const c_vector& obb_mins( ) const                                                     = 0;
	virtual const c_vector& obb_maxs( ) const                                                     = 0;
	virtual void world_space_trigger_bounds( c_vector* world_mins, c_vector* world_maxs ) const   = 0;
	virtual bool test_collision( const ray_t& ray, unsigned int contents_mask, c_game_trace& tr ) = 0;
	virtual bool test_hitboxes( const ray_t& ray, unsigned int contents_mask, c_game_trace& tr )  = 0;
	virtual int get_collision_model_index( )                                                      = 0;
	virtual const model_t* get_collision_model( )                                                 = 0;
	virtual c_vector& get_collision_origin( ) const                                               = 0;
	virtual c_angle& get_collision_angles( ) const                                                = 0;
	virtual const matrix3x4_t& collision_to_world_transform( ) const                              = 0;
	virtual int get_solid( ) const                                                                = 0;
	virtual int get_solid_flags( ) const                                                          = 0;
	virtual c_client_unknown* get_i_client_unknown( )                                             = 0;
	virtual int get_collision_group( ) const                                                      = 0;

	virtual void world_space_surrounding_bounds( c_vector* mins, c_vector* maxs ) = 0;
	virtual unsigned int get_required_trigger_flags( ) const                      = 0;
	virtual const matrix3x4_t* get_root_parent_to_world_transform( ) const        = 0;
	virtual void* get_v_physics_object( ) const                                   = 0;
};

class c_client_alpha_property
{
public:
	virtual c_client_unknown* get_i_client_unknown( )                                              = 0;
	virtual void set_alpha_modulation( unsigned char alpha )                                       = 0;
	virtual void set_render_fx( int render_fx, int render_mode, float start_time, float duration ) = 0;
	virtual void set_fade( float global_fade_scale, float dist_fade_start, float dist_fade_end )   = 0;
	virtual void set_desync_offset( int offset )                                                   = 0;
	virtual void enable_alpha_modulation_override( bool enable )                                   = 0;
	virtual void enable_shadow_alpha_modulation_override( bool enable )                            = 0;
	virtual void set_distance_fade_mode( int fade_mode )                                           = 0;
};

class c_client_networkable;
class c_client_renderable;
class c_client_entity;
class c_base_entity;
class c_client_thinkable;
class c_user_cmd;
class c_animation_state;

class c_client_unknown : public c_handle_entity
{
public:
	virtual c_collideable* get_collideable( )                     = 0;
	virtual c_client_networkable* get_client_networkable( )       = 0;
	virtual c_client_renderable* get_client_renderable( )         = 0;
	virtual c_client_entity* get_client_entity( )                 = 0;
	virtual c_base_entity* get_base_entity( )                     = 0;
	virtual c_client_thinkable* get_client_thinkable( )           = 0;
	virtual c_client_alpha_property* get_client_alpha_property( ) = 0;
};

struct renderable_instance_t {
	unsigned char m_alpha;
};

class c_client_renderable
{
public:
	virtual c_client_unknown* get_client_unknown( )                                                                                         = 0;
	virtual c_vector& get_render_origin( )                                                                                                  = 0;
	virtual c_angle& get_render_angles( )                                                                                                   = 0;
	virtual bool should_draw( )                                                                                                             = 0;
	virtual int get_render_flags( )                                                                                                         = 0;
	virtual bool is_transparent( )                                                                                                          = 0;
	virtual unsigned short get_shadow_handle( ) const                                                                                       = 0;
	virtual unsigned short& render_handle( )                                                                                                = 0;
	virtual const model_t* get_model( ) const                                                                                               = 0;
	virtual int draw_model( int flags, const renderable_instance_t& instance )                                                              = 0;
	virtual int get_body( )                                                                                                                 = 0;
	virtual void get_color_modulation( float* color )                                                                                       = 0;
	virtual bool lod_test( )                                                                                                                = 0;
	virtual bool setup_bones( matrix3x4_t* bone_to_world_out, int max_bones, int bone_mask, float current_time )                            = 0;
	virtual void setup_weights( const matrix3x4_t* bone_to_world, int flex_weight_count, float* flex_weights, float* flex_delayed_weights ) = 0;
	virtual void do_animation_events( )                                                                                                     = 0;
	virtual void* get_pvs_notify_interface( )                                                                                               = 0;
	virtual void get_render_bounds( c_vector& mins, c_vector& maxs )                                                                        = 0;
	virtual void get_render_bounds_worldspace( c_vector& mins, c_vector& maxs )                                                             = 0;
	virtual void get_shadow_render_bounds( c_vector& mins, c_vector& maxs, int shadow_type )                                                = 0;
	virtual bool should_receive_projected_textures( int flags )                                                                             = 0;
	virtual bool get_shadow_cast_distance( float* distance, int shadow_type ) const                                                         = 0;
	virtual bool get_shadow_cast_direction( c_vector* direction, int shadow_type ) const                                                    = 0;
	virtual bool is_shadow_dirty( )                                                                                                         = 0;
	virtual void mark_shadow_dirty( bool dirty )                                                                                            = 0;
	virtual c_client_renderable* get_shadow_parent( )                                                                                       = 0;
	virtual c_client_renderable* first_shadow_child( )                                                                                      = 0;
	virtual c_client_renderable* next_shadow_peer( )                                                                                        = 0;
	virtual int shadow_cast_type( )                                                                                                         = 0;
	virtual void unused2( ) { }
	virtual void create_model_instance( )                                         = 0;
	virtual unsigned short get_model_instance( )                                  = 0;
	virtual const matrix3x4_t& renderable_to_world_transform( )                   = 0;
	virtual int lookup_attachment( const char* attachment_name )                  = 0;
	virtual bool get_attachment( int index, c_vector& origin, c_angle& aview )    = 0;
	virtual bool get_attachment( int index, matrix3x4_t& attachment )             = 0;
	virtual bool compute_lighting_origin( int attachment_index, c_vector model_lighting_center, const matrix3x4_t& matrix,
	                                      c_vector& transformed_lighting_center ) = 0;
	virtual float* get_render_clip_plane( )                                       = 0;
	virtual int get_skin( )                                                       = 0;
	virtual void on_threaded_draw_setup( )                                        = 0;
	virtual bool uses_flex_delayed_weights( )                                     = 0;
	virtual void record_tool_message( )                                           = 0;
	virtual bool should_draw_for_split_screen_user( int slot )                    = 0;
	virtual unsigned char override_alpha_modulation( unsigned char alpha )        = 0;
	virtual unsigned char override_shadow_alpha_modulation( unsigned char alpha ) = 0;
	virtual void* get_client_model_renderable( )                                  = 0;
};

class c_base_client;
class c_client_networkable
{
public:
	virtual c_client_unknown* get_client_unknown( )                      = 0;
	virtual void release( )                                              = 0;
	virtual c_base_client* get_client_class( )                           = 0;
	virtual void notify_should_transmit( int state )                     = 0;
	virtual void on_pre_data_changed( e_data_update_type update_type )   = 0;
	virtual void on_data_changed( e_data_update_type update_type )       = 0;
	virtual void pre_data_update( e_data_update_type update_type )       = 0;
	virtual void post_data_update( e_data_update_type update_type )      = 0;
	virtual void on_data_unchanged_in_pvs( )                             = 0;
	virtual bool is_dormant( ) const                                     = 0;
	virtual int get_index( ) const                                       = 0;
	virtual void receive_message( e_class_id class_index, bf_read& msg ) = 0;
	virtual void* get_data_table_base_ptr( )                             = 0;
	virtual void set_destroyed_on_recreate_entities( )                   = 0;
};

class c_client_think_handle;
class c_client_thinkable
{
public:
	virtual c_client_unknown* get_client_unknown( )               = 0;
	virtual void client_think( )                                  = 0;
	virtual c_client_think_handle* get_think_handle( )            = 0;
	virtual void set_think_handle( c_client_think_handle* think ) = 0;
	virtual void release( )                                       = 0;
};

class c_client_entity : public c_client_unknown, public c_client_renderable, public c_client_networkable, public c_client_thinkable
{
public:
	virtual const c_vector& get_abs_origin( ) const                             = 0;
	virtual const c_angle& get_abs_angles( ) const                              = 0;
	virtual void* get_mouth( )                                                  = 0;
	virtual bool get_sound_spatialization( struct spatialization_info_t& info ) = 0;
	virtual bool is_blurred( )                                                  = 0;

	void set_abs_origin( const c_vector& origin );
	void set_abs_angles( const c_angle& angle );

	data_map_t* get_data_desc_map( )
	{
		return g_virtual.call< data_map_t* >( this, 15 );
	}

	data_map_t* get_prediction_desc_map( )
	{
		return g_virtual.call< data_map_t* >( this, 17 );
	}
};

class c_base_entity : public c_client_entity
{
public:
	/* DT_BasePlayer */
	add_pvariable( float, get_fall_velocity, "CBasePlayer->m_flFallVelocity" );
	add_variable( c_angle, get_view_punch, "CBasePlayer->m_viewPunchAngle" );
	add_variable( c_angle, get_punch, "CBasePlayer->m_aimPunchAngle" );
	add_variable( c_vector, get_view_offset, "CBasePlayer->m_vecViewOffset[0]" );
	add_variable( float, get_friction, "CBasePlayer->m_flFriction" );
	add_variable( int, get_tick_base, "CBasePlayer->m_nTickBase" );
	add_pvariable( int, get_next_think_tick, "CBasePlayer->m_nNextThinkTick" );
	add_variable( c_vector, get_velocity, "CBasePlayer->m_vecVelocity[0]" );
	add_pvariable_offset( c_angle, get_view_angles, "CBasePlayer->deadflag", 0x4 );
	add_variable( unsigned int, get_ground_entity_handle, "CBasePlayer->m_hGroundEntity" );
	add_variable( int, get_health, "CBasePlayer->m_iHealth" );
	add_variable( int, get_life_state, "CBasePlayer->m_lifeState" );
	add_variable( float, get_max_speed, "CBasePlayer->m_flMaxspeed" );
	add_variable( int, get_flags, "CBasePlayer->m_fFlags" );
	add_pvariable( int, get_observer_mode, "CBasePlayer->m_iObserverMode" );
	add_variable( unsigned int, get_observer_target_handle, "CBasePlayer->m_hObserverTarget" );
	add_variable( unsigned int, get_view_model_handle, "CBasePlayer->m_hViewModel[0]" );
	add_pvariable( const char, get_last_place, "CBasePlayer->m_szLastPlaceName" );
	add_variable_offset( int, get_button_disabled, "CBasePlayer->m_hViewEntity", -0xc );
	add_variable_offset( int, get_button_forced, "CBasePlayer->m_hViewEntity", -0x8 );
	add_pvariable_offset( c_user_cmd*, get_current_command, "CBasePlayer->m_hViewEntity", -0x4 );

	add_datafield( int, get_eflags, this->get_prediction_desc_map( ), "m_iEFlags" );
	add_pdatafield( int, get_buttons, this->get_prediction_desc_map( ), "m_nButtons" );
	add_datafield( int, get_button_last, this->get_prediction_desc_map( ), "m_afButtonLast" );
	add_datafield( int, get_button_pressed, this->get_prediction_desc_map( ), "m_afButtonPressed" );
	add_datafield( int, get_button_released, this->get_prediction_desc_map( ), "m_afButtonReleased" );
	add_pdatafield( int, get_impulse, this->get_prediction_desc_map( ), "m_nImpulse" );
	add_datafield( float, get_surface_friction, this->get_prediction_desc_map( ), "m_surfaceFriction" );

	add_variable( int, get_team, "CBaseEntity->m_iTeamNum" );
	add_variable( c_vector, get_origin, "CBaseEntity->m_vecOrigin" );
	add_variable( float, get_simulation_time, "CBaseEntity->m_flSimulationTime" );
	add_variable( bool, get_predictable, "CBaseEntity->m_bPredictable" );

	add_offset( unsigned int, get_index, 0x64 );
	add_offset( bool, is_dormant, 0xed );

	inline bool is_alive( )
	{
		return ( this->get_life_state( ) == 0 /* LIFE_ALIVE */ );
	}

	void modify_eye_position( const c_animation_state* animation_state, c_vector* position ) const;

	int get_bone_by_hash( const unsigned int hash ) const;

	c_vector get_bone_position( int bone );
};