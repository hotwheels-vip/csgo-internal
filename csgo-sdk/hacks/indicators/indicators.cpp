#include "indicators.h"

#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

void n_indicators::impl_t::on_paint_traverse( )
{
	if ( !g_render.m_initialised )
		return;

	const bool on_ground = g_ctx.m_local->get_flags( ) & e_flags::fl_onground;

	this->velocity( on_ground );
}

void n_indicators::impl_t::velocity( const bool on_ground )
{
	
}