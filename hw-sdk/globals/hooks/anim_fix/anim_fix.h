#pragma once

#include <d3dx9.h>
#include <iostream>

#include "../../../game/sdk/classes/c_cs_player.h"
#include "../../../utils/renderer/renderer.h"
#include "../../../utils/vfunc/vfunc.h"
#include "../../interfaces/interfaces.h"
#include "../hooking.h"

namespace hooks
{
	CREATE_HOOK_HELPER( standard_blending_rules_hook,
	                    void( __fastcall )( sdk::c_cs_player*, void*, void*, math::vec3*, math::quaternion< float >*, float, int ) );
	struct standard_blending_rules {
		static void __fastcall standard_blending_rules_detour( sdk::c_cs_player* player, void* edx, void* hdr, math::vec3* position,
		                                                       math::quaternion< float >* quaternion, float curtime, int bone_mask );

		static void init( )
		{
			standard_blending_rules_hook.create( g_client_dll.pattern_scan( _( "55 8B EC 83 E4 F0 B8 F8 10" ) ).as< void* >( ),
			                                     standard_blending_rules_detour, _( "standard_blending_rules_detour" ) );
		}
		static void unload( )
		{
			standard_blending_rules_hook.disable( );
		}
	};

	CREATE_HOOK_HELPER( do_extra_bone_processing_hook, void( __fastcall )( sdk::c_cs_player*, void*, void*, math::vec3*, math::quaternion< float >*,
	                                                                       math::matrix_3x4*, std::uint8_t*, void* ) );
	struct do_extra_bone_processing {
		static void __fastcall do_extra_bone_processing_detour( sdk::c_cs_player* player, void* edx, void* hdr, math::vec3* position,
		                                                        math::quaternion< float >* quaternion, math::matrix_3x4* matrix,
		                                                        std::uint8_t* bone_list, void* context );

		static void init( )
		{
			do_extra_bone_processing_hook.create(
				g_client_dll.pattern_scan( _( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 56 8B F1 57 89 74 24 1C 80" ) ).as< void* >( ),
				do_extra_bone_processing_detour, _( "do_extra_bone_processing_detour" ) );
		}
		static void unload( )
		{
			do_extra_bone_processing_hook.disable( );
		}
	};

	CREATE_HOOK_HELPER( update_client_side_animation_hook, void( __fastcall )( sdk::c_cs_player*, void* edx ) );
	struct update_client_side_animation {
		static void __fastcall update_client_side_animation_detour( sdk::c_cs_player* player, void* edx );

		static void init( )
		{
			update_client_side_animation_hook.create( g_client_dll.pattern_scan( _( "55 8B EC 51 56 8B F1 80 BE ? ? ? ? ? 74 36" ) ).as< void* >( ),
			                                          update_client_side_animation_detour, _( "update_client_side_animation_detour" ) );
		}
		static void unload( )
		{
			update_client_side_animation_hook.disable( );
		}
	};

	CREATE_HOOK_HELPER( should_skip_animimation_frame_hook, bool( __fastcall )( void*, void* ) );
	struct should_skip_animation_frame {
		static bool __fastcall should_skip_animation_frame_detour( void* ecx, void* edx );

		static void init( )
		{
			should_skip_animimation_frame_hook.create(
				g_client_dll.pattern_scan( _( "57 8B F9 8B 07 8B 80 ? ? ? ? FF D0 84 C0 75 02" ) ).as< void* >( ), should_skip_animation_frame_detour,
				_( "should_skip_animation_frame_detour" ) );
		}
		static void unload( )
		{
			should_skip_animimation_frame_hook.disable( );
		}
	};

	CREATE_HOOK_HELPER( process_interpolated_list_hook, int( __cdecl )( ) );
	struct process_interpolated_list {
		static int __cdecl process_interpolated_list_detour( );

		static void init( )
		{
			process_interpolated_list_hook.create( g_client_dll.pattern_scan( _( "53 0F B7 1D ? ? ? ? 56" ) ).as< void* >( ),
			                                       process_interpolated_list_detour, _( "process_interpolated_list_detour" ) );
		}
		static void unload( )
		{
			process_interpolated_list_hook.disable( );
		}
	};

	CREATE_HOOK_HELPER( build_transformations_hook, void( __fastcall )( void*, void*, void*, void*, void*, const math::matrix_3x4&, int, void* ) );
	struct build_transformations {
		static void __fastcall build_transformations_detour( void* ecx, void* edx, void* hdr, void* position, void* q,
		                                                     const math::matrix_3x4& camera_transformation, int bone_mask, void* bone_computed );

		static void init( )
		{
			build_transformations_hook.create(
				g_client_dll
					.pattern_scan(
						_( "55 8B EC 83 E4 ? 81 EC ? ? ? ? 56 57 8B F9 8B 0D ? ? ? ? 89 7C 24 ? 8B 81 ? ? ? ? 89 44 24 ? 85 C0 74 ? 80 3D" ) )
					.as< void* >( ),
				build_transformations_detour, _( "build_transformations_detour" ) );
		}
		static void unload( )
		{
			build_transformations_hook.disable( );
		}
	};
} // namespace hooks
