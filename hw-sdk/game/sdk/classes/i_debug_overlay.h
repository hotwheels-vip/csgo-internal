#pragma once
#include "../../../utils/helpers/primitive_definitions/primitives.h"
#include "../../../utils/math/math.h"
#include <minwindef.h>

namespace sdk
{
	class i_debug_overlay
	{
	private:
		virtual void __unkn( ) = 0;

	public:
		virtual void add_entity_text_overlay( int ent_index, int line_offset, float duration, int r, int g, int b, int a, const char* format,
		                                      ... )                                                                                               = 0;
		virtual void add_box_overlay( const math::vec3& origin, const math::vec3& mins, const math::vec3& max, math::vec3 const& orientation, int r,
		                              int g, int b, int a, float duration )                                                                       = 0;
		virtual void add_sphere_overlay( const math::vec3& origin, float radius, int theta, int phi, int r, int g, int b, int a, float duration ) = 0;
		virtual void add_triangle_overlay( const math::vec3& p1, const math::vec3& p2, const math::vec3& p3, int r, int g, int b, int a,
		                                   bool no_depth_test, float duration )                                                                   = 0;
		virtual void add_line_overlay( const math::vec3& origin, const math::vec3& dest, int r, int g, int b, bool no_depth_test,
		                               float duration )                                                                                           = 0;
		virtual void add_text_overlay( const math::vec3& origin, float duration, const char* format, ... )                                        = 0;
		virtual void add_text_overlay( const math::vec3& origin, int line_offset, float duration, const char* format, ... )                       = 0;
		virtual void add_screen_text_overlay( float x, float y, float z, int r, int g, int b, int a, const char* text )                           = 0;
		virtual void add_swept_box_overlay( const math::vec3& start, const math::vec3& end, const math::vec3& mins, const math::vec3& max,
		                                    const math::vec3& angles, int r, int g, int b, int a, float duration )                                = 0;
		virtual void add_grid_overlay( const math::vec3& origin )                                                                                 = 0;
		virtual void add_coord_frame_overlay( const math::matrix_3x4& frame, float scale, int color_table[ 3 ][ 3 ] = NULL )                      = 0;
		virtual int screen_position( const math::vec3& point, math::vec3& screen )                                                                = 0;
		virtual int screen_position( float x, float y, math::vec3& screen )                                                                       = 0;
		virtual unk* get_first( void )                                                                                                            = 0;
		virtual unk* get_next( unk* current )                                                                                                     = 0;
		virtual void clear_dead_overlays( void )                                                                                                  = 0;
		virtual void clear_all_overlays( )                                                                                                        = 0;
		virtual void add_text_overlay_rgba( const math::vec3& origin, int line_offset, float duration, float r, float g, float b, float alpha,
		                                    const char* format, ... )                                                                             = 0;
		virtual void add_text_overlay_rgba( const math::vec3& origin, int line_offset, float duration, int r, int g, int b, int a, const char* format,
		                                    ... )                                                                                                 = 0;
		virtual void add_line_overlay_alpha( const math::vec3& origin, const math::vec3& dest, int r, int g, int b, int a, bool no_depth_test,
		                                     float duration )                                                                                     = 0;
		virtual void add_box_overlay_2( const math::vec3& origin, const math::vec3& mins, const math::vec3& max, math::vec3 const& orientation,
		                                const BYTE* face_color, const BYTE* edge_color, float duration )                                          = 0;
		virtual void purge_text_overlays( )                                                                                                       = 0;
		virtual void draw_pill( const math::vec3& mins, const math::vec3& max, float& diameter, int r, int g, int b, int a, float duration )      = 0;
	};
} // namespace sdk
