#include "../../game/sdk/includes/includes.h"
#include "../hooks.h"

#include "../../globals/config/variables.h"
#include "../../globals/globals.h"
#include "../../globals/interfaces/interfaces.h"
#include "../../hacks/lagcomp/lagcomp.h"
#include <fstream>
#include <iosfwd>

void __fastcall n_detoured_functions::draw_model_execute( void* ecx, void* edx, void* context, void* state, model_render_info_t& info,
                                                          matrix3x4_t* custom_bone_to_world )
{
	static auto original = g_hooks.m_draw_model_execute.get_original< decltype( &n_detoured_functions::draw_model_execute ) >( );

	// [!] TEMPORARY [!]
	// todo - use key values for materials, dont create actual vmt files itself
	// creating vmt files on the csgo directory can make people go mess with it

	static c_material* animated_wireframe{ };

	if ( !animated_wireframe ) {
		std::ofstream( "csgo/materials/animated_wireframe.vmt" ) << R"#("UnlitGeneric" {
	        "$basetexture" "models/inventory_items/dreamhack_trophies/dreamhack_star_blur"
            "$wireframe" "1"
            "$alpha" "0.6"
            "$additive" "1"
			"$ignorez" "1"
            "proxies"
             {
                "texturescroll"
                {
                    "texturescrollvar" "$basetexturetransform"
                    "texturescrollrate" "0.2"
                    "texturescrollangle" "90"
                }
            }
        })#";

		animated_wireframe = g_interfaces.m_material_system->find_material( "debug/debugdrawflat" );
		animated_wireframe->increment_reference_count( );
	};

	const auto backtrack_color   = GET_VARIABLE( g_variables.m_player_lag_chams_color, c_color );
	const bool should_draw_chams = GET_VARIABLE( g_variables.m_player_lag_chams, bool );

	if ( !context || !custom_bone_to_world || !g_interfaces.m_engine_client->is_connected_safe( ) || !should_draw_chams ||
	     g_ctx.m_is_glow_being_drawn )
		return original( ecx, edx, context, state, info, custom_bone_to_world );

	if ( info.model ) {
		const std::string mdl = info.model->m_name;

		auto player = g_interfaces.m_client_entity_list->get< c_base_entity >( info.entity_index );

		// TODO fix for custom models
		//		make separate chams func

		if ( player->is_valid_enemy( ) && mdl.find( "player" ) != std::string::npos && info.entity_index > 0 && info.entity_index < 64 ) {
			auto oldest_record = g_lagcomp.oldest_record( info.entity_index );

			if ( const auto distance = oldest_record.value( ).m_vec_origin.dist_to( player->get_abs_origin( ) );
			     oldest_record.has_value( ) && distance > 1.f && distance < LAG_COMPENSATION_TELEPORTED_DISTANCE_SQR ) {
				// sorry
				animated_wireframe->color_modulate( backtrack_color[ 0 ] / 255, backtrack_color[ 1 ] / 255, backtrack_color[ 2 ] / 255 );
				animated_wireframe->alpha_modulate( backtrack_color[ 3 ] );

				// animated_wireframe->set_material_var_flag( material_var_nofog, true );
				animated_wireframe->set_material_var_flag( material_var_ignorez, true );
				// animated_wireframe->set_material_var_flag( material_var_znearer, true );

				g_interfaces.m_model_render->forced_material_override( animated_wireframe );

				original( g_interfaces.m_model_render, edx, context, state, info, oldest_record.value( ).m_matrix );

				g_interfaces.m_model_render->forced_material_override( nullptr );
			}
		}
	}

	original( ecx, edx, context, state, info, custom_bone_to_world );
	g_interfaces.m_model_render->forced_material_override( nullptr );
}
