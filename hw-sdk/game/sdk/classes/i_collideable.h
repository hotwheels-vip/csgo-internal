#pragma once
#include "../../../utils/math/types/matrix.h"
#include "../../../utils/math/types/vector.h"
#include "../structs/i_model.h"
#include "i_engine_trace.h"
#include "i_handle_entity.h"

namespace sdk
{
	struct trace_t;
	struct i_physics_object;
	struct i_client_unknown;

	enum solid_type_t;

	struct i_collideable {
	public:
		// gets at the entity handle associated with the collideable
		virtual i_handle_entity* get_entity_handle( ) = 0;

		// these methods return the bounds of an obb measured in "collision" space
		// which can be retreived through the collision_to_world_transform or
		// get_collision_origin/get_collision_angles methods
		virtual const math::vec3& obb_mins( ) const = 0;
		virtual const math::vec3& obb_maxs( ) const = 0;

		// returns the bounds of a world-space box used when the collideable is being traced
		// against as a trigger. it's only valid to call these methods if the solid flags
		// have the fsolid_use_trigger_bounds flag set.
		virtual void world_space_trigger_bounds( math::vec3* p_vec_world_mins, math::vec3* p_vec_world_maxs ) const = 0;

		// custom collision test
		virtual bool test_collision( const ray_t& ray, unsigned int f_contents_mask, trace_t& tr ) = 0;

		// perform hitbox test, returns true *if hitboxes were tested at all*!!
		virtual bool test_hitboxes( const ray_t& ray, unsigned int f_contents_mask, trace_t& tr ) = 0;

		// returns the brush model index if this is a brush model. otherwise, returns -1.
		virtual int get_collision_model_index( ) = 0;

		// return the model, if it's a studio model.
		virtual const i_model* get_collision_model( ) = 0;

		// get angles and origin.
		virtual const math::vec3& get_collision_origin( ) const               = 0;
		virtual const math::vec3& get_collision_angles( ) const               = 0;
		virtual const math::matrix_3x4& collision_to_world_transform( ) const = 0;

		// return a solid_ define.
		virtual solid_type_t get_solid( ) const = 0;
		virtual int get_solid_flags( ) const    = 0;

		// gets at the containing class...
		virtual i_client_unknown* get_i_client_unknown( ) = 0;

		// we can filter out collisions based on collision group
		virtual int get_collision_group( ) const = 0;

		// returns a world-aligned box guaranteed to surround *everything* in the collision representation
		// note that this will surround hitboxes, trigger bounds, physics.
		// it may or may not be a tight-fitting box and its volume may suddenly change
		virtual void world_space_surrounding_bounds( math::vec3* p_vec_mins, math::vec3* p_vec_maxs ) = 0;

		virtual std::uint32_t get_required_trigger_flags( ) const = 0;

		// returns null unless this collideable has specified fsolid_root_parent_aligned
		virtual const math::matrix_3x4* get_root_parent_to_world_transform( ) const = 0;
		virtual i_physics_object* get_v_physics_object( ) const                     = 0;
	};
} // namespace sdk
