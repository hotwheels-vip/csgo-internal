#pragma once
#include "../../utilities/macros.h"
#include "../structs/matrix_t.h"
#include "c_vector.h"
#include <wtypes.h>

enum trace_type {
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,
	TRACE_ENTITIES_ONLY,
	TRACE_EVERYTHING_FILTER_PROPS,
};

enum contents {
	CONTENTS_EMPTY                = 0X0,
	CONTENTS_SOLID                = 0X1,
	CONTENTS_WINDOW               = 0X2,
	CONTENTS_AUX                  = 0X4,
	CONTENTS_GRATE                = 0X8,
	CONTENTS_SLIME                = 0X10,
	CONTENTS_WATER                = 0X20,
	CONTENTS_BLOCKLOS             = 0X40,
	CONTENTS_OPAQUE               = 0X80,
	CONTENTS_TESTFOGVOLUME        = 0X100,
	CONTENTS_UNUSED               = 0X200,
	CONTENTS_BLOCKLIGHT           = 0X400,
	CONTENTS_TEAM1                = 0X800,
	CONTENTS_TEAM2                = 0X1000,
	CONTENTS_IGNORE_NODRAW_OPAQUE = 0X2000,
	CONTENTS_MOVEABLE             = 0X4000,
	CONTENTS_AREAPORTAL           = 0X8000,
	CONTENTS_PLAYERCLIP           = 0X10000,
	CONTENTS_MONSTERCLIP          = 0X20000,
	CONTENTS_CURRENT0             = 0X40000,
	CONTENTS_CURRENT90            = 0X80000,
	CONTENTS_CURRENT180           = 0X100000,
	CONTENTS_CURRENT270           = 0X200000,
	CONTENTS_CURRENT_UP           = 0X400000,
	CONTENTS_CURRENT_DOWN         = 0X800000,
	CONTENTS_ORIGIN               = 0X1000000,
	CONTENTS_MONSTER              = 0X2000000,
	CONTENTS_DEBRIS               = 0X4000000,
	CONTENTS_DETAIL               = 0X8000000,
	CONTENTS_TRANSLUCENT          = 0X10000000,
	CONTENTS_LADDER               = 0X20000000,
	CONTENTS_HITBOX               = 0X40000000,

	LAST_VISIBLE_CONTENTS = CONTENTS_OPAQUE,
	ALL_VISIBLE_CONTENTS  = LAST_VISIBLE_CONTENTS | LAST_VISIBLE_CONTENTS - 1
};

enum masks {
	MASK_ALL                   = 0XFFFFFFFF,
	MASK_SOLID                 = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE ),
	MASK_PLAYERSOLID           = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE ),
	MASK_NPCSOLID              = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER | CONTENTS_GRATE ),
	MASK_NPCFLUID              = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTERCLIP | CONTENTS_WINDOW | CONTENTS_MONSTER ),
	MASK_WATER                 = ( CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_SLIME ),
	MASK_OPAQUE                = ( CONTENTS_WATER | CONTENTS_MOVEABLE | CONTENTS_OPAQUE ),
	MASK_OPAQUE_NPC            = ( MASK_OPAQUE | CONTENTS_MONSTER ),
	MASK_BLOCKLOS              = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_SLIME ),
	MASK_BLOCKLOS_NPC          = ( MASK_BLOCKLOS | CONTENTS_MONSTER ),
	MASK_VISIBLE               = ( MASK_OPAQUE | CONTENTS_IGNORE_NODRAW_OPAQUE ),
	MASK_VISIBLE_NPC           = ( MASK_OPAQUE_NPC | CONTENTS_IGNORE_NODRAW_OPAQUE ),
	MASK_SHOT                  = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_HITBOX ),
	MASK_SHOT_BRUSHONLY        = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_DEBRIS ),
	MASK_SHOT_HULL             = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_MONSTER | CONTENTS_WINDOW | CONTENTS_DEBRIS | CONTENTS_GRATE ),
	MASK_SHOT_PLAYER           = ( MASK_SHOT_HULL | CONTENTS_HITBOX ),
	MASK_SHOT_PORTAL           = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTER ),
	MASK_SOLID_BRUSHONLY       = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_GRATE ),
	MASK_PLAYERSOLID_BRUSHONLY = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_PLAYERCLIP | CONTENTS_GRATE ),
	MASK_NPCSOLID_BRUSHONLY    = ( CONTENTS_SOLID | CONTENTS_MOVEABLE | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE ),
	MASK_NPCWORLDSTATIC        = ( CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP | CONTENTS_GRATE ),
	MASK_NPCWORLDSTATIC_FLUID  = ( CONTENTS_SOLID | CONTENTS_WINDOW | CONTENTS_MONSTERCLIP ),
	MASK_SPLITAREAPORTAL       = ( CONTENTS_WATER | CONTENTS_SLIME ),
	MASK_CURRENT =
		( CONTENTS_CURRENT0 | CONTENTS_CURRENT90 | CONTENTS_CURRENT180 | CONTENTS_CURRENT270 | CONTENTS_CURRENT_UP | CONTENTS_CURRENT_DOWN ),
	MASK_DEADSOLID = ( CONTENTS_SOLID | CONTENTS_PLAYERCLIP | CONTENTS_WINDOW | CONTENTS_GRATE )
};

class c_base_entity;

struct ray_t {
	// no need to forceinline this i believe
	__forceinline void init( c_vector const& m_start, c_vector const& m_end )
	{
		world_axis_transform = nullptr;

		delta = m_end - m_start;

		is_swept = ( delta.length_squared( ) != 0 );

		extents.m_x = extents.m_y = extents.m_z = 0.0f;

		is_ray = true;

		start_offset.m_x = start_offset.m_y = start_offset.m_z = 0.0f;

		start = m_start;
	}

	// initialize trace with mins and maxs
	__forceinline void init( c_vector const& m_start, c_vector const& m_end, c_vector const& m_mins, c_vector const& m_maxs )
	{
		delta = m_end - start;

		world_axis_transform = nullptr;
		is_swept             = ( delta.length_squared( ) != 0 );

		extents = m_maxs - m_mins;
		extents *= 0.5f;
		is_ray = ( extents.length_squared( ) < 1e-6 );

		start_offset = m_maxs + m_mins;
		start_offset *= 0.5f;
		start = m_start + start_offset;
		start_offset *= -1.0f;
	}

public:
	__declspec( align( 16 ) ) c_vector start;
	__declspec( align( 16 ) ) c_vector delta;
	__declspec( align( 16 ) ) c_vector start_offset;
	__declspec( align( 16 ) ) c_vector extents;

	const matrix3x4_t* world_axis_transform = nullptr;

	bool is_ray;
	bool is_swept;
};

struct plane_t {
	constexpr auto operator!=( const plane_t& v ) const noexcept
	{
		return normal.m_x != v.normal.m_x || normal.m_y != v.normal.m_y || normal.m_z != v.normal.m_z;
	}
	c_vector normal;
	float dist;
	BYTE type;
	BYTE signbits;
	PAD( 2 );
};

class c_base_trace
{
public:
	c_vector start_pos;
	c_vector end_pos;
	plane_t plane;
	float fraction;
	int contents;
	WORD disp_flags;
	bool all_solid;
	bool start_solid;
};

struct surface_t {
	const char* name;
	short surface_props;
	WORD flags;
};

class c_game_trace : public c_base_trace
{
public:
	__forceinline c_game_trace( ) { }

	bool dit_hit_world( ) const;

	bool did_hit_non_world_entity( ) const;

	bool is_visible( ) const;

	// todo
	int get_entity_index( ) const;

	bool did_hit( ) const;

	float fraction_left_solid{ };
	surface_t surface{ };
	int hit_group{ };
	short physics_bone{ };
	WORD world_surface_index{ };
	c_base_entity* entity = nullptr;
	int hitbox{ };
};

class i_trace_filter
{
public:
	virtual bool should_hit_entity( c_base_entity* entity, int contents_mask ) = 0;
	virtual trace_type get_trace_type( ) const                                 = 0;
	int collision_group                                                        = 0;
	c_base_entity* skip{ };
};

class c_trace_filter : public i_trace_filter
{
public:
	bool should_hit_entity( c_base_entity* entity_handle, int contents_mask )
	{
		return !( entity_handle == skip );
	}

	trace_type get_trace_type( ) const
	{
		return trace_type::TRACE_EVERYTHING;
	}
};

class c_trace_filter_skip_two_entities : public i_trace_filter
{
public:
	__forceinline c_trace_filter_skip_two_entities( c_base_entity* lhs, c_base_entity* rhs )
	{
		skip  = lhs;
		skip2 = rhs;
	}

	virtual bool should_hit_entity( c_base_entity* entity_handle, int contents_mask )
	{
		return !( entity_handle == skip || entity_handle == skip2 );
	}

	virtual trace_type get_trace_type( ) const
	{
		return trace_type::TRACE_EVERYTHING;
	}

	c_base_entity* skip2{ };
};

class c_trace_filter_skip_grenades : public i_trace_filter
{
public:
	virtual bool should_hit_entity( c_base_entity* entity_handle, int contents_mask )
	{
		if ( !entity_handle )
			return !( entity_handle == skip );

		return !( entity_handle == skip );
	}

	virtual trace_type get_trace_type( ) const
	{
		return trace_type::TRACE_ENTITIES_ONLY;
	}
};

class c_trace_filter_whitelist : public i_trace_filter
{
public:
	virtual bool should_hit_entity( c_base_entity* entity_handle, int contents_mask )
	{
		return entity_handle == whitelist;
	}

	virtual trace_type get_trace_type( ) const
	{
		return trace_type::TRACE_ENTITIES_ONLY;
	}

	void* whitelist{ };
};

class c_engine_trace_client
{
public:
	virtual int get_point_contents( const c_vector& vec_abs_pos, int content_mask = masks::MASK_ALL, c_base_entity** entity = nullptr ) = 0;
	virtual int get_point_contents_world_only( const c_vector& vec_abs_pos, int content_mask = masks::MASK_ALL )                        = 0;
	virtual int get_point_contents_collideable( void* collide, const c_vector& vec_abs_pos )                                            = 0;
	virtual void clip_ray_to_entity( const ray_t& ray, unsigned int fmask, c_base_entity* entity, c_game_trace* trace )                 = 0;
	virtual void clip_ray_to_collideable( const ray_t& ray, unsigned int fmask, void* collide, c_game_trace* trace )                    = 0;
	virtual void trace_ray( const ray_t& ray, unsigned int fmask, i_trace_filter* trace_filter, c_game_trace* trace )                   = 0;
};

__forceinline bool c_game_trace::did_hit( ) const
{
	return fraction < 1 || all_solid || start_solid;
}

__forceinline bool c_game_trace::dit_hit_world( ) const
{
	return entity;
}

__forceinline bool c_game_trace::did_hit_non_world_entity( ) const
{
	return entity && !dit_hit_world( );
}

__forceinline bool c_game_trace::is_visible( ) const
{
	return fraction > 0.97f;
}
