#include "../../game/sdk/includes/includes.h"
#include "../hooks.h"

#include "../../globals/config/variables.h"
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
	};

	const auto backtrack_color   = GET_VARIABLE( g_variables.m_player_lag_chams_color, c_color );
	const bool should_draw_chams = GET_VARIABLE( g_variables.m_player_lag_chams, bool );

	if ( !context || !custom_bone_to_world || !g_interfaces.m_engine_client->is_connected_safe( ) || !should_draw_chams )
		return original( ecx, edx, context, state, info, custom_bone_to_world );

	if ( info.model ) {
		const std::string mdl = info.model->m_name;

		auto player = g_interfaces.m_client_entity_list->get< c_base_entity >( info.entity_index );

		// TODO fix for custom models
		//		make separate chams func

		if ( player->is_valid_enemy( ) && mdl.find( "player" ) != std::string::npos && info.entity_index > 0 && info.entity_index < 64 ) {
			auto last_record = g_lagcomp.oldest_record( info.entity_index );
			if ( last_record.has_value( ) ) {

				matrix3x4_t arr[ 128 ];
				if ( g_lagcomp.generate_lerped_lag_matrix( info.entity_index, arr ) )
				animated_wireframe->color_modulate( backtrack_color[ 0 ], backtrack_color[ 1 ], backtrack_color[ 2 ] );
				animated_wireframe->alpha_modulate( backtrack_color[ 3 ] );

				animated_wireframe->set_material_var_flag( material_var_nofog, true );
				animated_wireframe->set_material_var_flag( material_var_ignorez, true );
				// animated_wireframe->set_material_var_flag( material_var_, true );
				//
				// g_interfaces.m_model_render->forced_material_override( material_btrack );
				//
				// ofunc( interfaces::model_render, ctx, state, info, records[ info.entity_index ].back( ).matrix );
				//
				// g_interfaces.m_model_render->forced_material_override( nullptr );
			}
		}
	}

	original( ecx, edx, context, state, info, custom_bone_to_world );
	g_interfaces.m_model_render->forced_material_override( nullptr );
}
