#include "indicators.h"

#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"

void n_indicators::impl_t::on_paint_traverse( )
{
	if ( !g_render.m_initialised )
		return;

	const bool on_ground = g_ctx.m_local->get_flags( ) & e_flags::fl_onground;

	g_render.m_draw_data.emplace_back( e_draw_type::draw_type_text,
	                                   std::make_any< text_draw_object_t >( g_render.m_fonts[ e_font_names::font_name_indicator_29 ],
	                                                                        c_vector_2d( 5, 5 ), "fart", ImColor( 1.f, 1.f, 1.f, 1.f ),
	                                                                        ImColor( 0.f, 0.f, 0.f, 0.f ), e_text_flags::text_flag_dropshadow ) );
}