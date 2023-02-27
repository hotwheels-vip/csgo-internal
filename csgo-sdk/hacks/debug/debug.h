#pragma once

#include "../../dependencies/imgui/imgui.h"
#include "../../game/sdk/includes/includes.h"
#include "../../globals/includes/includes.h"
#include "../movement/movement.h"
#include <format>

#ifdef _DEBUG

// debugger menu

namespace n_debugger
{
	struct impl_t {
		void draw_server_hitbox_model( )
		{
			static auto get_player_by_index = []( int index ) {
				static void* pattern = reinterpret_cast< void* >( g_modules[ SERVER_DLL ].find_pattern( "85 C9 7E 32 A1 ? ? ? ?" ) );

				auto util_player_by_index = reinterpret_cast< uintptr_t( __fastcall* )( unsigned int ) >( pattern );

				return util_player_by_index( index );
			};

			static void* pattern =
				reinterpret_cast< void* >( g_modules[ SERVER_DLL ].find_pattern( "55 8B EC 81 EC ? ? ? ? 53 56 8B 35 ? ? ? ? 8B D9 57 8B CE" ) );

			for ( int i = 1; i <= g_interfaces.m_global_vars_base->m_max_clients; i++ ) {
				uintptr_t player = get_player_by_index( i );
				if ( !player )
					return;

				static float duration = -1.0f;

				__asm
				{
					pushad
					movss xmm1, duration
					push 1
					mov ecx, player
					call pattern
					popad
				}
			}
		}

		void on_paint_traverse( )
		{
			if ( !GET_VARIABLE( g_variables.m_debugger_visual, bool ) )
				return;

			float offset = 0.f;

			constexpr auto render_debug = [ & ]( const char* indicator_name, const c_color& color, const bool active ) {
				ImAnimationHelper debug_animation = ImAnimationHelper( fnv1a::hash( indicator_name ), ImGui::GetIO( ).DeltaTime );
				debug_animation.Update( 2.f, active ? 2.f : -2.f );

				if ( debug_animation.AnimationData->second <= 0.f )
					return;

				const auto text_size = g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->CalcTextSizeA(
					g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ]->FontSize, FLT_MAX, 0.f, indicator_name );

				g_render.m_draw_data.emplace_back(
					e_draw_type::draw_type_text,
					std::make_any< text_draw_object_t >(
						g_render.m_fonts[ e_font_names::font_name_verdana_bd_11 ],
						c_vector_2d( ( g_ctx.m_width / 2 ) + text_size.x, ( g_ctx.m_height / 2 ) - offset - text_size.y ), indicator_name,
						color.get_u32( debug_animation.AnimationData->second ),
						ImColor( 0.f, 0.f, 0.f, color.base< e_color_type::color_type_a >( ) * debug_animation.AnimationData->second ),
						e_text_flags::text_flag_dropshadow ) );

				offset -= ( text_size.y + 2 ) * debug_animation.AnimationData->second;
			};

			std::stringstream ss_eb;
			ss_eb << "eb: duck: " << g_movement.m_edgebug_data.m_edgebug_method << " | delta: " << g_movement.m_edgebug_data.m_yaw_delta;

			auto eb = ss_eb.str( );

			render_debug( eb.c_str( ), c_color( 1.f, 1.f, 1.f, 1.f ), g_movement.m_edgebug_data.m_will_edgebug );

			render_debug( g_movement.m_pixelsurf_data.m_should_duck ? "ducked ps" : "stand ps", c_color( 1.f, 1.f, 1.f, 1.f ),
			              g_movement.m_pixelsurf_data.m_in_pixel_surf || g_movement.m_pixelsurf_data.m_predicted_succesful );
		}

		void on_frame_stage_notify( int stage )
		{
			if ( !GET_VARIABLE( g_variables.m_debugger_visual, bool ) || !g_interfaces.m_engine_client->is_connected_safe( ) || !g_ctx.m_local ||
			     !g_ctx.m_local->is_alive( ) )
				return;

			if ( stage == e_client_frame_stage::net_update_end )
				draw_server_hitbox_model( );
		}
	};
} // namespace n_debugger

inline n_debugger::impl_t g_debugger;
#endif
