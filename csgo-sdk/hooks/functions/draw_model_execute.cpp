#include "../../game/sdk/includes/includes.h"
#include "../hooks.h"

#include "../../dependencies/imgui/imgui.h"
#include "../../globals/config/variables.h"
#include "../../globals/globals.h"
#include "../../globals/interfaces/interfaces.h"
#include "../../hacks/lagcomp/lagcomp.h"
#include "../../utilities/console/console.h"
#include <fstream>
#include <iosfwd>

c_material* return_material( const int mat_index )
{
	switch ( mat_index ) {
	case 0:
		return g_interfaces.m_material_system->find_material( "debug/debugdrawflat" );
	case 1:
		return g_interfaces.m_material_system->find_material( "mat_textured", TEXTURE_GROUP_MODEL );
	case 2:
		return g_interfaces.m_material_system->find_material( "mat_metallic", TEXTURE_GROUP_MODEL );
	case 3:
		return g_interfaces.m_material_system->find_material( "mat_glow", TEXTURE_GROUP_MODEL );
	default:
		return g_interfaces.m_material_system->find_material( "debug/debugdrawflat" );
	}

	g_console.print( std::format( "return_material failed at index {}", mat_index ).c_str( ) );

	return g_interfaces.m_material_system->find_material( "debug/debugdrawflat" );
}

void __fastcall n_detoured_functions::draw_model_execute( void* ecx, void* edx, void* context, void* state, model_render_info_t& info,
                                                          matrix3x4_t* custom_bone_to_world )
{
	static auto original = g_hooks.m_draw_model_execute.get_original< decltype( &n_detoured_functions::draw_model_execute ) >( );

	// [!] TEMPORARY [!]
	// todo - use key values for materials, dont create actual vmt files itself
	// creating vmt files on the csgo directory can make people go mess with it

	static c_material* lag_material{ };
	static c_material* vis_material{ };
	static c_material* invis_material{ };

	if ( static bool init = false; !init ) {
		// metallic vis = mat_metallic.vmt
		std::ofstream( "csgo/materials/mat_metallic.vmt" ) << R"#("UnlitGeneric" 
		{
	        "$basetexture" "vgui/white"
			"$ignorez"      "0"
			"$envmap"       "env_cubemap"
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "1"
			"$rimlight"     "1"
			"$rimlightexponent"  "2"
			"$rimlightboost"     ".2"	
			"$reflectivity" "[1 1 1]"
        }
		)#";

		// textured vis = mat_textured.vmt
		std::ofstream( "csgo/materials/mat_textured.vmt" ) << R"#("UnlitGeneric" 
		{
			"$basetexture" "vgui/white"
			"$ignorez"      "0"
			"$envmap"       ""
			"$nofog"        "1"
			"$model"        "1"
			"$nocull"       "0"
			"$selfillum"    "1"
			"$halflambert"  "1"
			"$znearer"      "0"
			"$flat"         "1"
			"$reflectivity" "[1 1 1]"
        }
		)#";

		// glow = mat_glow.vmt
		std::ofstream( "csgo/materials/mat_glow.vmt" ) << R"#("VertexLitGeneric" 
		{
    		"$additive" "1"
    		"$envmap" "models/effects/cube_white"
    		"$envmaptint" "[1 1 1]"
    		"$envmapfresnel" "1"
    		"$envmapfresnelminmaxexp" "[0 1 2]"
    		"$alpha" "0.2"
		}
		)#";

		init = true;
	};

	// set materials

	if ( static int lag_mat_index = -1; !lag_material || lag_mat_index != GET_VARIABLE( g_variables.m_player_lag_chams_material, int ) ) {
		lag_material = return_material( GET_VARIABLE( g_variables.m_player_lag_chams_material, int ) );
		lag_material->increment_reference_count( );

		lag_mat_index = GET_VARIABLE( g_variables.m_player_lag_chams_material, int );
	}

	if ( static int vis_mat_index = -1; !vis_material || vis_mat_index != GET_VARIABLE( g_variables.m_player_visible_chams_material, int ) ) {
		vis_material = return_material( GET_VARIABLE( g_variables.m_player_visible_chams_material, int ) );
		vis_material->increment_reference_count( );

		vis_mat_index = GET_VARIABLE( g_variables.m_player_visible_chams_material, int );
	}

	if ( static int invis_mat_index = -1; !invis_material || invis_mat_index != GET_VARIABLE( g_variables.m_player_invisible_chams_material, int ) ) {
		invis_material = return_material( GET_VARIABLE( g_variables.m_player_invisible_chams_material, int ) );
		invis_material->increment_reference_count( );

		invis_mat_index = GET_VARIABLE( g_variables.m_player_invisible_chams_material, int );
	}

	// variable spam
	const auto backtrack_color = GET_VARIABLE( g_variables.m_player_lag_chams_color, c_color );
	const auto visible_color   = GET_VARIABLE( g_variables.m_player_visible_chams_color, c_color );
	const auto invisible_color = GET_VARIABLE( g_variables.m_player_invisible_chams_color, c_color );

	const bool should_draw_lag   = GET_VARIABLE( g_variables.m_player_lag_chams, bool );
	const bool should_draw_vis   = GET_VARIABLE( g_variables.m_player_visible_chams, bool );
	const bool should_draw_invis = GET_VARIABLE( g_variables.m_player_invisible_chams, bool );

	const bool should_draw_chams = should_draw_lag || should_draw_vis || should_draw_invis;

	if ( !context || !custom_bone_to_world || !g_interfaces.m_engine_client->is_connected_safe( ) || !g_ctx.m_local || !should_draw_chams ||
	     g_ctx.m_is_glow_being_drawn || strstr( info.model->m_name, "player/contactshadow" ) || !info.model ||
	     g_interfaces.m_model_render->is_forced_material_override( ) )
		return original( ecx, edx, context, state, info, custom_bone_to_world );

	const std::string mdl = info.model->m_name;
	auto player           = g_interfaces.m_client_entity_list->get< c_base_entity >( info.entity_index );

	// TODO ~ fix for custom models
	//		~ make separate chams func
	//		~ use keyvalues
	//		~ clean up code

	if ( player->is_valid_enemy( ) && mdl.find( "player" ) != std::string::npos && info.entity_index > 0 && info.entity_index < 64 ) {
		auto oldest_record    = g_lagcomp.oldest_record( info.entity_index );
		static float distance = 0.f;

		switch ( GET_VARIABLE( g_variables.m_player_lag_chams_type, int ) ) {
		case 0: // oldest record
			if ( !should_draw_lag || !oldest_record.has_value( ) )
				break;

			if ( distance = oldest_record.value( ).m_vec_origin.dist_to( player->get_abs_origin( ) );
			     oldest_record.has_value( ) && distance < LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ) {
				ImAnimationHelper alpha_animation = ImAnimationHelper( HASH_RT( mdl.c_str( ) ), ImGui::GetIO( ).DeltaTime );
				alpha_animation.Update( 2.f, distance > 3.f ? 2.f : -2.f );

				if ( alpha_animation.AnimationData->second > 0.f ) {
					// sorry
					lag_material->color_modulate( backtrack_color.base< color_type_r >( ), backtrack_color.base< color_type_g >( ),
					                              backtrack_color.base< color_type_b >( ) );
					lag_material->alpha_modulate( backtrack_color.base< color_type_a >( ) * alpha_animation.AnimationData->second );

					lag_material->set_material_var_flag( material_var_nofog, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );
					lag_material->set_material_var_flag( material_var_ignorez, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );
					lag_material->set_material_var_flag( material_var_znearer, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );

					g_interfaces.m_model_render->forced_material_override( lag_material );

					original( g_interfaces.m_model_render, edx, context, state, info, oldest_record.value( ).m_matrix );

					g_interfaces.m_model_render->forced_material_override( nullptr );
				}
			}
			break;
		case 1: // all records
			if ( !should_draw_lag || !oldest_record.has_value( ) )
				break;

			distance = oldest_record.value( ).m_vec_origin.dist_to( player->get_abs_origin( ) );

			if ( const auto record_list = g_lagcomp.m_records[ info.entity_index ];
			     record_list && oldest_record.has_value( ) && distance < LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ) {
				for ( int i = 0; i < g_ctx.m_max_allocations; i++ ) {
					if ( !record_list[ i ].m_valid )
						continue;
					ImAnimationHelper alpha_animation = ImAnimationHelper( HASH_RT( mdl.c_str( ) ), ImGui::GetIO( ).DeltaTime );
					alpha_animation.Update( 2.f, distance > 3.f ? 2.f : -2.f );

					if ( alpha_animation.AnimationData->second > 0.f ) {
						// sorry
						lag_material->color_modulate( backtrack_color.base< color_type_r >( ), backtrack_color.base< color_type_g >( ),
						                              backtrack_color.base< color_type_b >( ) );
						lag_material->alpha_modulate( backtrack_color.base< color_type_a >( ) * alpha_animation.AnimationData->second );

						lag_material->set_material_var_flag( material_var_nofog, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );
						lag_material->set_material_var_flag( material_var_ignorez, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );
						lag_material->set_material_var_flag( material_var_znearer, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );

						g_interfaces.m_model_render->forced_material_override( lag_material );

						original( g_interfaces.m_model_render, edx, context, state, info, record_list[ i ].m_matrix );

						g_interfaces.m_model_render->forced_material_override( nullptr );
					}
				}
			}
			break;
		case 2: // aimbot targeted record
			if ( !g_ctx.m_record || !should_draw_lag || !oldest_record.has_value( ) )
				break;

			if ( distance = g_ctx.m_record->m_vec_origin.dist_to( player->get_abs_origin( ) ); distance < LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ) {
				ImAnimationHelper alpha_animation = ImAnimationHelper( HASH_RT( mdl.c_str( ) ), ImGui::GetIO( ).DeltaTime );
				alpha_animation.Update( 2.f, distance > 3.f ? 2.f : -2.f );

				if ( alpha_animation.AnimationData->second > 0.f ) {
					// sorry
					lag_material->color_modulate( backtrack_color.base< color_type_r >( ), backtrack_color.base< color_type_g >( ),
					                              backtrack_color.base< color_type_b >( ) );
					lag_material->alpha_modulate( backtrack_color.base< color_type_a >( ) * alpha_animation.AnimationData->second );

					lag_material->set_material_var_flag( material_var_nofog, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );
					lag_material->set_material_var_flag( material_var_ignorez, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );
					lag_material->set_material_var_flag( material_var_znearer, GET_VARIABLE( g_variables.m_player_lag_chams_xqz, bool ) );

					g_interfaces.m_model_render->forced_material_override( lag_material );

					original( g_interfaces.m_model_render, edx, context, state, info, g_ctx.m_record->m_matrix );

					g_interfaces.m_model_render->forced_material_override( nullptr );
				}
			}
		}

		if ( should_draw_invis ) {
			invis_material->color_modulate( invisible_color.base< color_type_r >( ), invisible_color.base< color_type_g >( ),
			                                invisible_color.base< color_type_b >( ) );
			invis_material->alpha_modulate( invisible_color.base< color_type_a >( ) );

			invis_material->set_material_var_flag( material_var_nofog, true );
			invis_material->set_material_var_flag( material_var_ignorez, true );
			invis_material->set_material_var_flag( material_var_znearer, true );

			g_interfaces.m_model_render->forced_material_override( invis_material );

			original( g_interfaces.m_model_render, edx, context, state, info, custom_bone_to_world );

			g_interfaces.m_model_render->forced_material_override( nullptr );

			// fixes invis only chams
			invis_material->color_modulate( 1.f, 1.f, 1.f );
		}

		if ( should_draw_vis ) {
			vis_material->color_modulate( visible_color.base< color_type_r >( ), visible_color.base< color_type_g >( ),
			                              visible_color.base< color_type_b >( ) );
			vis_material->alpha_modulate( visible_color.base< color_type_a >( ) );

			vis_material->set_material_var_flag( material_var_nofog, false );
			vis_material->set_material_var_flag( material_var_ignorez, false );
			vis_material->set_material_var_flag( material_var_znearer, false );

			g_interfaces.m_model_render->forced_material_override( vis_material );

			original( g_interfaces.m_model_render, edx, context, state, info, custom_bone_to_world );
		}
	}

	original( ecx, edx, context, state, info, custom_bone_to_world );
	g_interfaces.m_model_render->forced_material_override( nullptr );
}
