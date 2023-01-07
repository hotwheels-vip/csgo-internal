#include "../../sdk/classes/c_base_animating.h"

math::vec3 sdk::c_base_animating::hitbox_position( int index, float point_scale )
{
	if ( auto model = get_model( ) ) {
		if ( auto studio_model = g_interfaces.model_info->get_studio_model( model ) ) {
			math::matrix_3x4 matrix[ 128 ];

			if ( setup_bones( matrix, 128, 0x100, 0.f ) ) {
				if ( auto hitbox_set_ = studio_model->get_hitbox_set( hitbox_set( ) ) ) {
					if ( auto hitbox = hitbox_set_->hitbox( index ) ) {
						auto position = ( hitbox->mins + hitbox->maxs ) * point_scale;

						return math::vector_transform( position, matrix[ hitbox->bone ] );
					}
				}
			}
		}
	}

	return { };
}

math::vec3 sdk::c_base_animating::hitbox_position( int index, math::matrix_3x4* matrix, float point_scale )
{
	if ( auto model = get_model( ) ) {
		if ( auto studio_model = g_interfaces.model_info->get_studio_model( model ) ) {
			if ( auto hitbox_set_ = studio_model->get_hitbox_set( hitbox_set( ) ) ) {
				if ( auto hitbox = hitbox_set_->hitbox( index ) ) {
					auto position = ( hitbox->mins + hitbox->maxs ) * point_scale;

					return math::vector_transform( position, matrix[ hitbox->bone ] );
				}
			}
		}
	}

	return { };
}
