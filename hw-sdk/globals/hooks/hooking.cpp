#include "hooking.h"

#include "../../dependencies/mocking_bird/mocking_bird.h"
#include "../../hacks/menu/imgui/imgui_helper.h"
#include "../ctx/ctx.h"

#include "alloc_key_value_memory/alloc_key_value_memory.h"
#include "anim_fix/anim_fix.h"
#include "calculate_view/calculate_view.h"
#include "cl_move/cl_move.h"
#include "create_move/create_move.h"
#include "draw_model_execute/draw_model_execute.h"
#include "emit_sound/emit_sound.h"
#include "end_scene/end_scene.h"
#include "fire_event_intern/fire_event_intern.h"
#include "frame_stage_notify/frame_stage_notify.h"
#include "glow_effect_spectator/glow_effect_spectator.h"
#include "is_loose_file_allowed/is_loose_file_allowed.h"
#include "item_post_frame/item_post_frame.h"
#include "list_leaves_in_box/list_leaves_in_box.h"
#include "modify_eye_position/modify_eye_position.h"
#include "paint_traverse/paint_traverse.h"
#include "reset/reset.h"
//#include "return_address/return_address.h"
#include "send_net_msg/send_net_msg.h"
//#include "wndproc/wndproc.h"

bool h_init( )
{
	MOCKING_TRY;

	if ( MH_Initialize( ) != MH_OK ) {
		console::print< console::log_level::FATAL >( _( "MH_Initialize was not MH_OK" ) );
		return false;
	}

	//hooks::wndproc::init( );
	hooks::reset::init( );
	hooks::end_scene::init( );
	hooks::create_move::init( );
	// hooks::cl_move::init( );
	hooks::item_post_frame::init( );
	hooks::emit_sound::init( );
	hooks::glow_effect_spectator::init( );
	hooks::paint_traverse::init( );
	hooks::frame_stage_notify::init( );
	hooks::standard_blending_rules::init( );
	hooks::do_extra_bone_processing::init( );
	hooks::update_client_side_animation::init( );
	hooks::should_skip_animation_frame::init( );
	hooks::process_interpolated_list::init( );
	hooks::draw_model_execute::init( );
	hooks::build_transformations::init( );
	hooks::list_leaves_in_box::init( );
	hooks::is_loose_file_allowed::init( );
	hooks::send_net_msg::init( );
	// hooks::fire_event_intern::init( ); /* note - coffin - crashing for now, add event listener as not all events get registered via this hook */
	//hooks::return_address::init( );
	hooks::alloc_key_value_memory::init( );
	hooks::modify_eye_position::init( );
	// hooks::calculate_view::init( );

	console::print< console::log_level::SUCCESS >( _( "Initialized all hooks." ) );

	MOCKING_CATCH( return false );

	return true;
}

void h_unload( )
{
	MOCKING_TRY;

	//hooks::wndproc::unload( );
	hooks::reset::unload( );
	hooks::end_scene::unload( );
	hooks::create_move::unload( );
	// hooks::cl_move::unload( );
	hooks::item_post_frame::unload( );
	hooks::emit_sound::unload( );
	hooks::glow_effect_spectator::unload( );
	hooks::paint_traverse::unload( );
	hooks::frame_stage_notify::unload( );
	hooks::do_extra_bone_processing::unload( );
	hooks::update_client_side_animation::unload( );
	hooks::should_skip_animation_frame::unload( );
	hooks::process_interpolated_list::unload( );
	hooks::draw_model_execute::unload( );
	hooks::build_transformations::unload( );
	hooks::list_leaves_in_box::unload( );
	hooks::is_loose_file_allowed::unload( );
	hooks::send_net_msg::unload( );
	// hooks::fire_event_intern::unload( );
	//hooks::return_address::unload( );
	hooks::alloc_key_value_memory::unload( );
	hooks::modify_eye_position::unload( );
	// hooks::calculate_view::unload( );

	// this is so useless lol
	if ( MH_Uninitialize( ) != MH_OK )
		console::print< console::log_level::FATAL >( _( "MH_Uninitialize was not MH_OK" ) );

	g_imgui.destroy( );

	MOCKING_CATCH( );
}
