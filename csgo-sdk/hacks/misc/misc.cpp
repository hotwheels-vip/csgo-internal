#include "misc.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

void n_misc::impl_t::on_create_move_pre( )
{
	this->disable_post_processing( );
	this->remove_panorama_blur( );
}

void n_misc::impl_t::disable_post_processing( )
{
	g_convars[ HASH_BT( "mat_postprocess_enable" ) ]->set_value( !( GET_VARIABLE( g_variables.m_disable_post_processing, bool ) ) );
}

void n_misc::impl_t::remove_panorama_blur( )
{
	g_convars[ HASH_BT( "@panorama_disable_blur" ) ]->set_value( GET_VARIABLE( g_variables.m_remove_panorama_blur, bool ) );
}