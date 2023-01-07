#include "draw_model_execute.h"
#include "../../../hacks/features/lagcomp/lagcomp.h"
#include "../../ctx/ctx.h"
#include <type_traits>

void __fastcall hooks::draw_model_execute::draw_model_execute_detour( void* ecx, void* edx, void* context, void* state, sdk::model_render_info& info,
                                                                      math::matrix_3x4* custom_bone_to_world )
{
	static sdk::i_material* animated_wireframe{ };
	static sdk::i_material* material_regular{ };
	static sdk::i_material* material_flat{ };

	if ( !animated_wireframe ) {
		std::ofstream( _( "csgo/materials/animated_wireframe.vmt" ) ) << _( R"#("UnlitGeneric" {
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
        })#" );
	}

	animated_wireframe = g_interfaces.material_system->find_material( _( "animated_wireframe" ) );
	material_regular   = g_interfaces.material_system->find_material( _( "debug/debugambientcube" ) );
	material_flat      = g_interfaces.material_system->find_material( _( "debug/debugdrawflat" ) );

	bool model_is_player = strstr( info.model->name, _( "models/player" ) );
	bool model_is_weapon = strstr( info.model->name, _( "weapons/v_" ) );

	if ( model_is_player ) {
		if ( info.entity_index >= 0 && info.entity_index <= 64 ) {
			static auto unlag_pointer = g_convars[ _( "sv_maxunlag" ) ];
			auto sv_maxunlag_ticks    = sdk::time_to_ticks( unlag_pointer->get_float( ) );

			for ( int current_heap_iterator = 0; current_heap_iterator < sv_maxunlag_ticks; current_heap_iterator++ ) {
				auto current_record = &g_lagcomp.heap_records[ info.entity_index ][ current_heap_iterator ];

				// if ( !current_record || !current_record->valid || !current_record->bone_matrix )
				//	continue;
				//
				// const int green = static_cast< int >( ( current_heap_iterator + 1 ) * 3 * 2.55f );
				//
				// material_regular->color_modulate( ( 255 - green ) / 255.f, green / 255.f, 0.7058f );
				// material_regular->alpha_modulate( 0.7f );
				// material_regular->set_material_var_flag( sdk::MATERIAL_VAR_IGNOREZ, true );
				//
				// g_interfaces.model_render->forced_material_override( material_regular );
				//
				// draw_model_execute_hook.call_original< void >( ecx, edx, context, state, info, current_record->bone_matrix );
				//
				// g_interfaces.model_render->forced_material_override( nullptr );
			}

			// auto& player_info = g_entity_list.players[ info.entity_index ];
			//
			// if ( player_info.m_valid ) {
			//	material_regular->color_modulate( 99 / 255.f, 0 / 255.f, 114 / 255.f );
			//	material_regular->set_material_var_flag( sdk::MATERIAL_VAR_IGNOREZ, true );
			//
			//	draw_model_execute_hook.call_original< void >( ecx, edx, context, state, info, custom_bone_to_world );
			//
			//	g_interfaces.model_render->forced_material_override( material_regular );
			//
			//	draw_model_execute_hook.call_original< void >( ecx, edx, context, state, info, custom_bone_to_world );
			//
			//	material_regular->color_modulate( 144 / 255.f, 2 / 255.f, 168 / 255.f );
			//	material_regular->set_material_var_flag( sdk::MATERIAL_VAR_IGNOREZ, false );
			//
			//	draw_model_execute_hook.call_original< void >( ecx, edx, context, state, info, custom_bone_to_world );
			//
			//	return g_interfaces.model_render->forced_material_override( nullptr );
			// }
		}
	}

	if ( model_is_weapon ) {
		// animated_wireframe->color_modulate( 0 / 255.f, 255 / 255.f, 255 / 255.f );
		// animated_wireframe->set_material_var_flag( sdk::MATERIAL_VAR_IGNOREZ, false );
		//
		// draw_model_execute_hook.call_original< void >( ecx, edx, context, state, info, custom_bone_to_world );
		//
		// g_interfaces.model_render->forced_material_override( animated_wireframe );
		//
		// draw_model_execute_hook.call_original< void >( ecx, edx, context, state, info, custom_bone_to_world );
		//
		// return g_interfaces.model_render->forced_material_override( nullptr );
	}

	draw_model_execute_hook.call_original< void >( ecx, edx, context, state, info, custom_bone_to_world );
}
