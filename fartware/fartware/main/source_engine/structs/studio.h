#pragma once
#include "../classes/c_angle.h"
#include "../classes/c_utl_vector.h"
#include "../structs/matrix_t.h"

using radian_euler = float[ 3 ];
using quaternion   = float[ 4 ];

#define maxstudiobonectrls  4
#define maxstudioposeparam  24
#define maxstudioskins      32  // total textures
#define maxstudioflexctrl   96  // maximum number of flexcontrollers (input sliders)
#define maxstudiobones      128 // total bones actually used
#define maxstudioanimblocks 256
#define maxstudioflexdesc   1024 // maximum number of low level flexes (actual morph targets)

#define bone_calculate_mask        0x1f
#define bone_physically_simulated  0x01
#define bone_physics_procedural    0x02
#define bone_always_procedural     0x04
#define bone_screen_align_sphere   0x08
#define bone_screen_align_cylinder 0x10

#define bone_used_mask           0x0007ff00
#define bone_used_by_anything    0x0007ff00
#define bone_used_by_hitbox      0x00000100
#define bone_used_by_attachment  0x00000200
#define bone_used_by_vertex_mask 0x0003fc00
#define bone_used_by_vertex_lod0 0x00000400
#define bone_used_by_vertex_lod1 0x00000800
#define bone_used_by_vertex_lod2 0x00001000
#define bone_used_by_vertex_lod3 0x00002000
#define bone_used_by_vertex_lod4 0x00004000
#define bone_used_by_vertex_lod5 0x00008000
#define bone_used_by_vertex_lod6 0x00010000
#define bone_used_by_vertex_lod7 0x00020000
#define bone_used_by_bone_merge  0x00040000
#define bone_always_setup        0x00080000

#define bone_used_by_vertex_at_lod( i_lod )   ( bone_used_by_vertex_lod0 << ( i_lod ) )
#define bone_used_by_anything_at_lod( i_lod ) ( ( bone_used_by_anything & ~bone_used_by_vertex_mask ) | bone_used_by_vertex_at_lod( i_lod ) )

#define max_num_lods 8

#define bone_type_mask           0x00f00000
#define bone_fixed_alignment     0x00100000
#define bone_has_saveframe_pos   0x00200000
#define bone_has_saveframe_rot64 0x00400000
#define bone_has_saveframe_rot32 0x00800000

struct virtualmodel_t;
struct vertexfileheader_t;
struct studiohdr_t;
class c_studio_hdr
{
public:
	studiohdr_t* p_studio_hdr;
	virtualmodel_t* p_virtual_model;
	void* p_softbody;
	mutable c_utl_vector< const studiohdr_t* > p_studio_hdr_cache;
	mutable int mn_frame_unlock_counter;
	int* p_frame_unlock_counter;
	unsigned char pad0[ 0x8 ];
	c_utl_vector< int > vec_bone_flags;
	c_utl_vector< int > vec_bone_parent;
	void* p_activity_to_sequence;
};

struct mstudiobone_t {
	int n_name_index;
	int i_parent;               // parent bone
	int i_bone_controller[ 6 ]; // bone controller index, -1 == none

	c_vector vec_position;
	quaternion q_world;
	radian_euler rad_rotation;

	// compression scale
	c_vector vec_position_scale;
	c_vector vec_euler_scale;

	matrix3x4_t mat_pose_to_bone;
	quaternion q_alignment;
	int i_flags;
	int i_procedure_type;
	int n_procedure_index;       // procedural rule
	mutable int mi_physics_bone; // index into physically simulated bone
	int n_surface_prop_index;    // index into string table for property name
	int i_contents;              // see bspflags.h for the contents flags
	int i_surface_prop;          // this index must be cached by the loader, not saved in the file
	unsigned char pad0[ 0x1c ];

	inline const char* get_name( )
	{
		if ( !n_name_index )
			return nullptr;
		return ( char* )this + n_name_index;
	}

	inline void* get_procedure( ) const
	{
		if ( !n_procedure_index )
			return nullptr;
		return ( std::uint8_t* )this + n_procedure_index;
	}

	inline const char* get_surface_prop_name( ) const
	{
		if ( !n_surface_prop_index )
			return nullptr;
		return ( char* )this + n_surface_prop_index;
	}
};

struct mstudiobonecontroller_t {
	int i_bone; // -1 == 0
	int n_type; // x, y, z, xr, yr, zr, m
	float fl_start;
	float fl_end;
	int i_rest;        // byte index value at rest
	int i_input_field; // 0-3 user set controller, 4 mouth
	unsigned char pad0[ 0x40 ];
};

struct mstudiobbox_t {
	int i_bone;
	int i_group;         // intersection group
	c_vector vec_bb_min; // bounding box
	c_vector vec_bb_max;
	int n_hitbox_name_index; // offset to the name of the hitbox
private:
	int32_t pad01[ 3 ];

public:
	float m_radius;

private:
	int32_t pad02[ 4 ];

public:
	const char* get_hitbox_name( ) const
	{
		if ( n_hitbox_name_index == 0 )
			return reinterpret_cast< char* >( '\0' );

		return reinterpret_cast< const char* >( this ) + n_hitbox_name_index;
	}
};

struct mstudiohitboxset_t {
	int m_name_index;
	int m_num_hitboxes;
	int m_hitbox_index;

	inline const char* get_name( ) const
	{
		if ( !m_name_index )
			return reinterpret_cast< char* >( '\0' );

		return reinterpret_cast< const char* >( this ) + m_name_index;
	}

	inline mstudiobbox_t* get_hitbox( int i_hit_box ) const
	{
		if ( i_hit_box < 0 || i_hit_box >= m_num_hitboxes )
			return nullptr;

		return reinterpret_cast< mstudiobbox_t* >( ( std::uint8_t* )this + m_hitbox_index ) + i_hit_box;
	}
};

class virtualgroup_t
{
public:
	void* p_cache;
	c_utl_vector< int > vec_bone_map;
	c_utl_vector< int > vec_master_bone;
	c_utl_vector< int > vec_master_sequence;
	c_utl_vector< int > vec_master_anim;
	c_utl_vector< int > vec_master_attachment;
	c_utl_vector< int > vec_master_pose;
	c_utl_vector< int > vec_master_node;
};

struct virtualsequence_t {
	int n_flags;
	int i_activity;
	int i_group;
	int n_index;
};

struct virtualgeneric_t {
	int i_group;
	int n_index;
};

struct virtualmodel_t {
	inline virtualgroup_t* get_anim_group( const int i_animation )
	{
		// note: user must manage mutex for this
		return &vec_group[ vec_anim[ i_animation ].i_group ];
	}

	inline virtualgroup_t* get_sequence_group( const int i_sequence )
	{
		// note: user must manage mutex for this
		return &vec_group[ vec_sequence[ i_sequence ].i_group ];
	}

	unsigned char pad0[ 0x8 ]; // c_thread_fast_mutex
	c_utl_vector< virtualsequence_t > vec_sequence;
	c_utl_vector< virtualgeneric_t > vec_anim;
	c_utl_vector< virtualgeneric_t > vec_attachment;
	c_utl_vector< virtualgeneric_t > vec_pose;
	c_utl_vector< virtualgroup_t > vec_group;
	c_utl_vector< virtualgeneric_t > vec_node;
	c_utl_vector< virtualgeneric_t > vec_ik_lock;
	c_utl_vector< unsigned short > vec_autoplay_sequences;
};

struct studiohdr_t;
struct mstudioseqdesc_t {
	int n_base_studio;
	inline studiohdr_t* get_studio_hdr( ) const
	{
		return ( studiohdr_t* )( ( std::uint8_t* )this + n_base_studio );
	}

	int n_label_index;
	inline const char* get_label( ) const
	{
		return ( char* )this + n_label_index;
	}

	int n_activity_name_index;
	inline const char* get_activity_name( ) const
	{
		return ( char* )this + n_activity_name_index;
	}

	int i_flags;

	int i_activity;
	int i_activity_weight;

	int n_events;
	int n_event_index;
	inline void* get_event( int i_event ) const
	{
		if ( i_event < 0 || i_event >= n_events )
			return nullptr;
		return ( ( std::uint8_t* )this + n_event_index ) + i_event;
	}

	c_vector vec_bb_min;
	c_vector vec_bb_max;

	int n_blends;
	int n_blend_index;

	inline int get_anim_value( int x, int y ) const
	{
		if ( x >= i_group_size[ 0 ] )
			x = i_group_size[ 0 ] - 1;

		if ( y >= i_group_size[ 1 ] )
			y = i_group_size[ 1 ] - 1;

		int i_offset      = y * i_group_size[ 0 ] + x;
		short* arr_blends = ( short* )( ( std::uint8_t* )this + n_blend_index );
		return ( int )arr_blends[ i_offset ];
	}

	int n_movement_index; // [blend] float array for blended movement
	int i_group_size[ 2 ];
	int n_param_index[ 2 ];    // x, y, z, xr, yr, zr
	float fl_param_start[ 2 ]; // local (0..1) starting value
	float fl_param_end[ 2 ];   // local (0..1) ending value
	int i_param_parent;

	float fl_fade_in_time;  // ideal cross fate in time (0.2 default)
	float fl_fade_out_time; // ideal cross fade out time (0.2 default)

	int i_entry_node; // transition node at entry
	int i_exit_node;  // transition node at exit
	int i_node_flags; // transition rules

	float fl_entry_phase; // used to match entry gait
	float fl_exit_phase;  // used to match exit gait

	float fl_last_frame; // frame that should generation end_of_sequence

	int i_next_sequence; // auto advancing sequences
	int i_pose;          // index of delta animation between end and nextseq

	int n_ik_rules;

	int n_auto_layers;
	int n_auto_layer_index;
	inline void* get_auto_layer( int i_auto_layer ) const
	{
		if ( i_auto_layer < 0 || i_auto_layer >= n_auto_layers )
			return nullptr;
		return ( ( std::uint8_t* )this + n_auto_layer_index ) + i_auto_layer;
	}

	int n_weight_list_index;
	inline float* get_bone_weight( int i_bone ) const
	{
		return ( ( float* )( ( std::uint8_t* )this + n_weight_list_index ) + i_bone );
	}

	int n_pose_key_index;
	inline float* get_pose_key( int i_parameter, int i_animation ) const
	{
		return ( float* )( ( std::uint8_t* )this + n_pose_key_index ) + i_parameter * i_group_size[ 0 ] + i_animation;
	}

	int n_ik_locks;
	int n_ik_lock_index;
	inline void* get_ik_lock( int i_ik ) const
	{
		if ( i_ik < 0 || i_ik >= n_ik_locks )
			return nullptr;
		return ( ( std::uint8_t* )this + n_ik_lock_index ) + i_ik;
	}

	int n_key_value_index;
	int i_key_value_size;
	inline const char* key_value_text( ) const
	{
		if ( !i_key_value_size )
			return nullptr;
		return ( char* )this + n_key_value_index;
	}

	int n_cycle_pose_index;

	int n_activity_modifier_index;
	int n_activity_modifiers;

	int n_anim_tag_index;
	int n_anim_tags;

	int n_root_driver_index;
	unsigned char pad[ 0x8 ];
};

struct mstudioposeparamdesc_t {
	int n_name_index;
	int n_flags;
	float fl_start; // starting value
	float fl_end;   // ending value
	float fl_loop;  // looping range, 0 for no looping, 360 for rotations, etc.

	inline const char* get_name( ) const
	{
		return ( char* )this + n_name_index;
	}
};

struct studiohwdata_t;
struct studiohdr_t {
	int n_id;
	int i_version;
	int i_checksum;
	char sz_name[ 64 ];
	int i_length;

	c_vector vec_eye_position;
	c_vector vec_illum_position;
	c_vector vec_hull_min;
	c_vector vec_hull_max;
	c_vector vec_view_bb_min;
	c_vector vec_view_bb_max;

	int i_flags;

	int n_bones;
	int n_bone_index;
	inline mstudiobone_t* get_bone( int i_bone ) const
	{
		if ( i_bone < 0 || i_bone >= n_bones )
			return nullptr;
		return ( mstudiobone_t* )( ( std::uint8_t* )this + n_bone_index ) + i_bone;
	}

	int n_bone_controllers;
	int n_bone_controller_index;
	inline void* get_bone_controller( int i_bone_controller ) const
	{
		if ( i_bone_controller < 0 || i_bone_controller >= n_bone_controllers )
			return nullptr;
		return ( ( std::uint8_t* )this + n_bone_controller_index ) + i_bone_controller;
	}

	int n_hitbox_sets;
	int n_hitbox_set_index;
	inline mstudiohitboxset_t* get_hitbox_set( int i_set ) const
	{
		if ( i_set < 0 || i_set >= n_hitbox_sets )
			return nullptr;
		return ( mstudiohitboxset_t* )( ( std::uint8_t* )this + n_hitbox_set_index ) + i_set;
	}

	inline mstudiobbox_t* get_hitbox( int i_hit_box, int i_set ) const
	{
		const mstudiohitboxset_t* p_hitbox_set = get_hitbox_set( i_set );

		if ( p_hitbox_set == nullptr )
			return nullptr;

		return p_hitbox_set->get_hitbox( i_hit_box );
	}

	inline int get_hitbox_count( int i_set ) const
	{
		const mstudiohitboxset_t* p_hitbox_set = get_hitbox_set( i_set );

		if ( p_hitbox_set == nullptr )
			return 0;

		return p_hitbox_set->m_num_hitboxes;
	}

	int n_local_animations;
	int n_local_animation_index;
	inline void* get_anim_description( int i_animation ) const
	{
		if ( i_animation < 0 || i_animation >= n_local_animations )
			i_animation = 0;
		return ( ( std::uint8_t* )this + n_local_animation_index ) + i_animation;
	}

	int n_local_sequences;
	int n_local_sequence_index;
	inline mstudioseqdesc_t* get_local_sequence_description( int i_sequence ) const
	{
		if ( i_sequence < 0 || i_sequence >= n_local_sequences )
			i_sequence = 0;
		return ( mstudioseqdesc_t* )( ( std::uint8_t* )this + n_local_sequence_index ) + i_sequence;
	}

	inline mstudioseqdesc_t& get_sequence_description( int i_sequence, virtualmodel_t* p_virtual_model = nullptr,
	                                                   const studiohdr_t* p_studio_hdr = nullptr ) const
	{
		if ( n_include_models == 0 || p_virtual_model == nullptr || p_studio_hdr == nullptr )
			return *get_local_sequence_description( i_sequence );

		return *p_studio_hdr->get_local_sequence_description( p_virtual_model->vec_sequence[ i_sequence ].n_index );
	}

	mutable int mi_activity_list_version;
	mutable int mi_events_indexed;

	int n_textures;
	int n_texture_index;
	inline void* get_texture( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_textures )
			return nullptr;
		return ( ( std::uint8_t* )this + n_texture_index ) + n_index;
	}

	int n_cd_textures;
	int n_cd_texture_index;
	inline char* get_cd_texture( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_cd_textures )
			return nullptr;
		return ( ( ( char* )this ) + *( ( int* )( ( std::uint8_t* )this + n_cd_texture_index ) + n_index ) );
	}

	int n_skin_refs;
	int n_skin_families;
	int n_skin_index;
	inline short* get_skin_ref( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_skin_refs )
			return nullptr;
		return ( short* )( ( std::uint8_t* )this + n_skin_index ) + n_index;
	}

	int n_body_parts;
	int n_body_part_index;
	inline void* get_body_part( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_body_parts )
			return nullptr;
		return ( ( std::uint8_t* )this + n_body_part_index ) + n_index;
	}

	int n_attachments;
	int n_attachment_index;
	inline void* get_attachment( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_attachments )
			return nullptr;
		return ( ( std::uint8_t* )this + n_attachment_index ) + n_index;
	}

	int n_transitions;
	int n_transition_index;
	int n_transition_name_index;
	inline std::uint8_t* get_transition( int n_index ) const
	{
		if ( n_index < 0 || n_index >= ( n_transitions * n_transitions ) )
			return nullptr;
		return ( std::uint8_t* )( ( std::uint8_t* )this + n_transition_index ) + n_index;
	}

	int n_flex_descs;
	int n_flex_desc_index;
	inline void* get_flex_description( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_flex_descs )
			return nullptr;
		return ( ( std::uint8_t* )this + n_flex_desc_index ) + n_index;
	}

	int n_flex_controllers;
	int n_flex_controller_index;
	inline void* get_flex_controller( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_flex_controllers )
			return nullptr;
		return ( ( std::uint8_t* )this + n_flex_controller_index ) + n_index;
	}

	int n_flex_rules;
	int n_flex_rule_index;
	inline void* get_flex_rule( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_flex_rules )
			return nullptr;
		return ( ( std::uint8_t* )this + n_flex_rule_index ) + n_index;
	}

	int n_ik_chains;
	int n_ik_chain_index;
	inline void* get_ik_chain( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_ik_chains )
			return nullptr;
		return ( ( std::uint8_t* )this + n_ik_chain_index ) + n_index;
	}

	int n_mouths;
	int n_mouth_index;
	inline void* get_mouth( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_mouths )
			return nullptr;
		return ( ( std::uint8_t* )this + n_mouth_index ) + n_index;
	}

	int n_pose_parameters;
	int n_pose_parameter_index;
	inline void* get_pose_parameter( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_pose_parameters )
			return nullptr;
		return ( ( std::uint8_t* )this + n_pose_parameter_index ) + n_index;
	}

	int n_surface_prop_index;
	inline const char* get_surface_prop( ) const
	{
		if ( !n_surface_prop_index )
			return nullptr;
		return ( char* )this + n_surface_prop_index;
	}

	int n_key_value_index;
	int n_key_value_size;
	inline const char* key_value_text( ) const
	{
		if ( !n_key_value_size )
			return nullptr;
		return ( char* )this + n_key_value_index;
	}

	int n_ik_autoplay_locks;
	int n_ik_autoplay_lock_index;
	inline void* get_ik_autoplay_lock( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_ik_autoplay_locks )
			return nullptr;
		return ( ( std::uint8_t* )this + n_ik_autoplay_lock_index ) + n_index;
	}

	float fl_mass;
	int i_contents;

	int n_include_models;
	int n_include_model_index;
	inline void* get_model_group( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_include_models )
			return nullptr;
		return ( ( std::uint8_t* )this + n_include_model_index ) + n_index;
	}

	int i_virtual_model;

	int n_anim_block_name_index;
	inline const char* get_anim_block_name( ) const
	{
		return ( ( char* )this ) + n_anim_block_name_index;
	}

	int n_anim_blocks;
	int n_anim_block_index;
	inline void* get_anim_block( int n_index ) const
	{
		if ( n_index < 0 || n_index >= n_anim_blocks )
			return nullptr;
		return ( ( ( std::uint8_t* )this ) + n_anim_block_index ) + n_index;
	}

	int i_anim_block_model;

	int n_bone_table_by_name_index;
	inline const std::uint8_t* get_bone_table_sorted_by_name( ) const
	{
		return ( std::uint8_t* )this + n_bone_table_by_name_index;
	}

	int i_vertex_base;
	int n_index_base;

	std::uint8_t u_directional_light_dot;
	std::uint8_t u_root_lod;
	std::uint8_t n_allowed_root_lo_ds;
	unsigned char pad0[ 0x5 ];

	int n_flex_controller_ui;
	int n_flexcontroller_ui_index;

	float fl_vert_anim_fixed_point_scale;
	mutable int mi_surface_prop_lookup;
	int n_studio_hdr2_index;

	unsigned char pad1[ 0x4 ];
};
