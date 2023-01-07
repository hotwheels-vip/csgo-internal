#pragma once

#include "../../../utils/math/types/vector.h"
#include "../enums/trace_type.h"

namespace sdk
{
	struct c_base_player;

	// this struct is here because its only used with conjuntion of engine trace
	struct ray_t {
		// no need to forceinline this i believe
		__forceinline void init( math::vec3 const& m_start, math::vec3 const& m_end )
		{
			world_axis_transform = nullptr;

			delta = m_end - m_start;

			is_swept = ( delta.length_sqr( ) != 0 );

			extents.x = extents.y = extents.z = 0.0f;

			is_ray = true;

			start_offset.x = start_offset.y = start_offset.z = 0.0f;

			start = m_start;
		}

		// initialize trace with mins and maxs
		__forceinline void init( math::vec3 const& m_start, math::vec3 const& m_end, math::vec3 const& m_mins, math::vec3 const& m_maxs )
		{
			delta = m_end - start;

			world_axis_transform = nullptr;
			is_swept             = ( delta.length_sqr( ) != 0 );

			extents = m_maxs - m_mins;
			extents *= 0.5f;
			is_ray = ( extents.length_sqr( ) < 1e-6 );

			start_offset = m_maxs + m_mins;
			start_offset *= 0.5f;
			start = m_start + start_offset;
			start_offset *= -1.0f;
		}

	public:
		__declspec( align( 16 ) ) math::vec3 start;
		__declspec( align( 16 ) ) math::vec3 delta;
		__declspec( align( 16 ) ) math::vec3 start_offset;
		__declspec( align( 16 ) ) math::vec3 extents;

		const math::matrix_3x4* world_axis_transform = nullptr;

		bool is_ray;
		bool is_swept;
	};

	struct plane_t {
		math::vec3 normal;
		float dist;
		BYTE type;
		BYTE signbits;
		PAD( 2 );
	};

	class c_base_trace
	{
	public:
		math::vec3 start_pos;
		math::vec3 end_pos;
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

		int get_entity_index( ) const;

		bool did_hit( ) const;

		float fraction_left_solid;
		surface_t surface;
		int hit_group;
		short physics_bone;
		WORD world_surface_index;
		c_base_player* entity = nullptr;
		int hitbox;
	};

	class i_trace_filter
	{
	public:
		virtual bool should_hit_entity( c_base_player* entity, int contents_mask ) = 0;
		virtual trace_type get_trace_type( ) const                                 = 0;
		int collision_group                                                        = 0;
		c_base_player* skip;
	};

	class c_trace_filter : public i_trace_filter
	{
	public:
		bool should_hit_entity( c_base_player* entity_handle, int contents_mask )
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
		__forceinline c_trace_filter_skip_two_entities( c_base_player* lhs, c_base_player* rhs )
		{
			skip  = lhs;
			skip2 = rhs;
		}

		virtual bool should_hit_entity( c_base_player* entity_handle, int contents_mask )
		{
			return !( entity_handle == skip || entity_handle == skip2 );
		}

		virtual trace_type get_trace_type( ) const
		{
			return trace_type::TRACE_EVERYTHING;
		}

		c_base_player* skip2;
	};

	class c_trace_filter_skip_grenades : public i_trace_filter
	{
	public:
		virtual bool should_hit_entity( c_base_player* entity_handle, int contents_mask )
		{
			if ( !entity_handle )
				return !( entity_handle == skip );

			// auto cclass = entity_handle->get_client_networkable( )->get_client_class( );

			// if ( !cclass )
			//	return !( entity_handle == skip );

			// if ( cclass->class_id == class_ids::BASE_CS_GRENADE_PROJECTILE )
			//	return false;

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
		virtual bool should_hit_entity( c_base_player* entity_handle, int contents_mask )
		{
			return entity_handle == whitelist;
		}

		virtual trace_type get_trace_type( ) const
		{
			return trace_type::TRACE_ENTITIES_ONLY;
		}

		void* whitelist;
	};

	class i_engine_trace_client
	{
	public:
		virtual int get_point_contents( const math::vec3& vec_abs_pos, int content_mask = masks::MASK_ALL, c_base_player** entity = nullptr ) = 0;
		virtual int get_point_contents_world_only( const math::vec3& vec_abs_pos, int content_mask = masks::MASK_ALL )                        = 0;
		virtual int get_point_contents_collideable( void* collide, const math::vec3& vec_abs_pos )                                            = 0;
		virtual void clip_ray_to_entity( const ray_t& ray, unsigned int fmask, c_base_player* entity, c_game_trace* trace )                   = 0;
		virtual void clip_ray_to_collideable( const ray_t& ray, unsigned int fmask, void* collide, c_game_trace* trace )                      = 0;
		virtual void trace_ray( const ray_t& ray, unsigned int fmask, i_trace_filter* trace_filter, c_game_trace* trace )                     = 0;
	};
} // namespace sdk

__forceinline bool sdk::c_game_trace::did_hit( ) const
{
	return fraction < 1 || all_solid || start_solid;
}

__forceinline bool sdk::c_game_trace::dit_hit_world( ) const
{
	return entity;
}

__forceinline bool sdk::c_game_trace::did_hit_non_world_entity( ) const
{
	return entity && !dit_hit_world( );
}

__forceinline bool sdk::c_game_trace::is_visible( ) const
{
	return fraction > 0.97f;
}
