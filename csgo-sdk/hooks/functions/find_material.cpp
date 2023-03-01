#include "../../game/sdk/includes/includes.h"
#include "../../globals/globals.h"
#include "../../globals/interfaces/interfaces.h"
#include "../../utilities/console/console.h"
#include "../hooks.h"
#include <fstream>

/* can be used to a world material changer */
c_material* __fastcall n_detoured_functions::find_material( void* ecx, void* edx, const char* material_name, const char* texture_group, bool complain,
                                                            const char* complain_prefix )
{
	static auto original = g_hooks.m_find_material.get_original< decltype( &n_detoured_functions::find_material ) >( );

	// TOOK ME WAY TO LONG TO DO AND IT DOESNT WORK FUCK

	static c_material* custom_glow_mat;

	if ( static bool init = false; !init ) {
		std::ofstream( "csgo/materials/custom_glow_material.vmt" ) << R"#("UnlitGeneric"
		{
			"$basetexture" "vgui/white_additive"
			"$ignorez" "1"
			"$model" "1"
			"$additive" "0"
			"$linearwrite" "1"
			"$translucent" "0"
			"$alpha"	"1"
			"$color" "[1 1 1]"
	    }
		)#";

		custom_glow_mat = original( g_interfaces.m_material_system, edx, "custom_glow_material", TEXTURE_GROUP_MODEL, true, NULL );
		custom_glow_mat->increment_reference_count( );

		init = true;
	};

	static bool found_ignorez      = false;
	static bool found_translucent  = false;
	static bool found_base_texture = false;

	if ( strstr( material_name, "dev/glow_color" ) ) {
		// g_console.print( "found dev/glow_colow" );

		// auto result = original( ecx, edx, material_name, texture_group, complain, complain_prefix );
		// custom_glow_mat->set_material_var_flag( e_material_var_flags::material_var_pseudo_translucent, false );
		// custom_glow_mat->set_material_var_flag( e_material_var_flags::material_var_translucent, false );
		// custom_glow_mat->set_material_var_flag( e_material_var_flags::material_var_ignorez, true );
		// custom_glow_mat->set_material_var_flag( e_material_var_flags::material_var_nofog, true );
		// custom_glow_mat->set_material_var_flag( e_material_var_flags::material_var_model, false );
		//// result->color_modulate( 1, 1, 1 );
		// custom_glow_mat->set_material_var_flag( e_material_var_flags::material_var_additive, false );
		return custom_glow_mat;
	}

	return original( ecx, edx, material_name, texture_group, complain, complain_prefix );
}
