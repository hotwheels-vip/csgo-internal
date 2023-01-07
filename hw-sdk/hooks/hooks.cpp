#include "hooks.h"
#include "../dependencies/imgui/dx9/imgui_impl_dx9.h"
#include "../dependencies/imgui/imgui_impl_win32.h"

bool hooks::impl::init( )
{
	MOCKING_TRY;

	if ( const auto status = MH_Initialize( ); status != MH_OK ) {
		console::print< console::log_level::FATAL >( _( "hooks::impl::init( ) failed to initialize [ status: {} ]" ), MH_StatusToString( status ) );
		return false;
	}

	if ( hooks::wnd_proc = reinterpret_cast< WNDPROC >( LI_FN( SetWindowLongA )( FindWindowA( _( "Valve001" ), nullptr ), GWL_WNDPROC,
	                                                                             reinterpret_cast< LONG_PTR >( hooks::detours::wnd_proc ) ) );
	     !hooks::wnd_proc ) {
		console::print< console::log_level::SUCCESS >( _( "Failed find game window" ) );
		return false;
	}

	g_render.init( g_interfaces.device );

	MOCK hooks::ret_add::init( );

	// virtual calls

	HOOK( hooks::create_move.create( virtual_func::get( g_interfaces.client, 22 ), hooks::detours::create_move_proxy ) );

	HOOK( hooks::frame_stage_notify.create( virtual_func::get( g_interfaces.client, 37 ), hooks::detours::frame_stage_notify ) );

	HOOK( hooks::emit_sound.create( virtual_func::get( g_interfaces.engine_sound, 5 ), hooks::detours::emit_sound ) );

	HOOK( hooks::end_scene.create( virtual_func::get( g_interfaces.device, 42 ), hooks::detours::end_scene ) );

	HOOK( hooks::paint_traverse.create( virtual_func::get( g_interfaces.panel, 41 ), hooks::detours::paint_traverse ) );

	HOOK( hooks::reset.create( virtual_func::get( g_interfaces.device, 16 ), hooks::detours::reset ) );

	HOOK( hooks::is_loose_file_allowed.create( virtual_func::get( g_interfaces.file_system, 128 ), hooks::detours::is_loose_file_allowed ) );

	// patterns

	HOOK( hooks::modify_eye_position.create( g_client_dll.pattern_scan( _( "55 8B EC 83 E4 F8 83 EC 70 56 57 8B F9 89 7C 24 14" ) ).as< void* >( ),
	                                         hooks::detours::modify_eye_position ) );

	HOOK( hooks::list_leaves_in_box.create( g_engine_dll.pattern_scan( _( "55 8B EC 83 EC ? 8B 4D ? 8D 55" ) ).as< void* >( ),
	                                        hooks::detours::list_leaves_in_box ) );

	HOOK( hooks::item_post_frame.create( g_client_dll.pattern_scan( _( "55 8B EC 83 EC ? 56 57 8B F9 8B 37" ) ).as< void* >( ),
	                                     hooks::detours::item_post_frame ) );

	MOCKING_CATCH( return false );

	console::print< console::log_level::SUCCESS >( _( "Initialized all hooks." ) );
	return true;
}

void hooks::impl::unload( )
{
	MOCKING_TRY;

	MH_DisableHook( MH_ALL_HOOKS );
	MH_RemoveHook( MH_ALL_HOOKS );

	MH_Uninitialize( );

	LI_FN( SetWindowLongA )( hotwheels::window, GWL_WNDPROC, reinterpret_cast< LONG_PTR >( hooks::wnd_proc ) );

	g_imgui.destroy( );

	MOCKING_CATCH( );
}

__declspec( naked ) void __fastcall hooks::detours::create_move_proxy( void* edx, int, int sequence_number, float input_sample_frametime,
                                                                       bool active )
{
	__asm {
		push ebp
		mov  ebp, esp
		push ebx;
		push esp
		push dword ptr[active]
		push dword ptr[input_sample_frametime]
		push dword ptr[sequence_number]
		call create_move
		pop  ebx
		pop  ebp
		retn 0Ch
	}
}

void __stdcall hooks::detours::create_move( int sequence_number, float input_sample_frametime, bool is_active, bool& send_packet )
{
	static auto og = hooks::create_move.get_original< decltype( &hooks::detours::create_move_proxy ) >( );

	og( g_interfaces.client, 0, sequence_number, input_sample_frametime, is_active );

	sdk::c_user_cmd* cmd              = g_interfaces.input->get_user_cmd( sequence_number );
	sdk::c_verified_cmd* verified_cmd = g_interfaces.input->get_verified_cmd( sequence_number );

	if ( !cmd || !cmd->command_number || !is_active )
		return;

	g_ctx.cmd = cmd;

	auto net_channel = g_interfaces.client_state->net_channel;

	if ( !g_ctx.local )
		return;

	g_movement.pre_prediction.think( );

	g_prediction.update( );
	g_prediction.store_backup( );

	g_prediction.start( g_ctx.local );
	g_prediction.end( g_ctx.local );

	g_aimbot.run( );
	g_movement.movement_fix( cmd, g_prediction.backup_vars.view_angles );

	g_movement.post_prediction.think( );

	if ( net_channel ) {
		if ( !hooks::send_net_msg.is_hooked( ) )
			hooks::send_net_msg.create( virtual_func::get( net_channel, 40 ), &hooks::detours::send_net_msg );
	}

	verified_cmd->command  = *cmd;
	verified_cmd->checksum = cmd->checksum( );
}

extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND window, UINT message, WPARAM wideParam, LPARAM longParam );
HRESULT WINAPI hooks::detours::wnd_proc( HWND window, UINT message, WPARAM parameter, LPARAM long_parameter )
{
	g_input.think( message, parameter, long_parameter );

	if ( g_input.key_state( input::key_state_t::KEY_RELEASED, VK_INSERT ) )
		g_menu.menu_open = !g_menu.menu_open;

	static auto lazy_callwndproc = LI_FN( CallWindowProcA ).get( );

	if ( g_menu.menu_initialised && g_menu.menu_open && ImGui_ImplWin32_WndProcHandler( window, message, parameter, long_parameter ) )
		return 1L;

	return lazy_callwndproc( hooks::wnd_proc, window, message, parameter, long_parameter );
}

bool __cdecl hooks::detours::glow_effect_spectator( sdk::c_cs_player* player, sdk::c_cs_player* local, sdk::glow_style& style, math::vec3& glow_color,
                                                    float& alpha_start, float& alpha, float& time_start, float& time_target, bool& animate )
{
	static auto og = hooks::glow_effet_spectator.get_original< decltype( &hooks::detours::glow_effect_spectator ) >( );

	return og( player, local, style, glow_color, alpha_start, alpha, time_start, time_target, animate );
}

int __fastcall hooks::detours::list_leaves_in_box( void* ecx, void* edx, math::vec3& mins, math::vec3& maxs, unsigned short* list, int list_max )
{
	static auto og = hooks::list_leaves_in_box.get_original< decltype( &hooks::detours::list_leaves_in_box ) >( );

	static auto return_address = g_client_dll.pattern_scan( _( "89 44 24 ? EB ? C7 44 24 ? ? ? ? ? 8B 45" ) ).as< std::uintptr_t >( );

	if ( reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ) != return_address )
		return og( ecx, edx, mins, maxs, list, list_max );

	auto info = *reinterpret_cast< sdk::renderable_info** >( reinterpret_cast< std::uintptr_t >( _AddressOfReturnAddress( ) ) + 0x14 );

	if ( !info || !info->renderable )
		return og( ecx, edx, mins, maxs, list, list_max );

	auto base_entity = info->renderable->get_i_client_unkown( )->get_base_entity( );

	if ( !base_entity || !base_entity->is_player( ) )
		return og( ecx, edx, mins, maxs, list, list_max );

	info->flags &= ~0x100;
	info->flags_alt |= 0xC0;

	static math::vec3 new_mins = { -16384.f, -16384.f, -16384.f };
	static math::vec3 new_maxs = { 16384.f, 16384.f, 16384.f };

	return og( ecx, edx, new_mins, new_maxs, list, list_max );
}

void __fastcall hooks::detours::frame_stage_notify( void* ecx, void* edx, sdk::frame_stage stage )
{
	static auto og = hooks::frame_stage_notify.get_original< decltype( &hooks::detours::frame_stage_notify ) >( );

	og( ecx, edx, stage );

	switch ( stage ) {
	case sdk::frame_stage::NET_UPDATE_END:
		g_entity_list.update( );

		for ( auto event = g_interfaces.client_state->events; event; event = event->next )
			event->fire_delay = 0.f;

		g_interfaces.engine->fire_events( );

		for ( auto& player_info : g_entity_list.players ) {
			auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

			if ( !player_info.m_valid || !player || !player->is_enemy( g_ctx.local ) || player == g_ctx.local )
				continue;

			g_ctx.updating_animations = true;

			// player->update_client_side_animation( );
			// g_ctx.updating_animations = false;

			auto var_mapping = player->get_var_map( );

			for ( int iterator = 0; iterator < var_mapping->interpolated_entries; iterator++ )
				var_mapping->entries[ iterator ].needs_to_interpolate = false;
		}

		g_lagcomp.update( );

		if ( g_ctx.local ) {
			sdk::datatypes::c_utl_vector< sdk::sound_info > sounds{ };

			g_interfaces.engine_sound->get_active_sounds( sounds );

			for ( int iterator = 0; iterator < sounds.count( ); iterator++ ) {
				auto& info = sounds[ iterator ];

				if ( !info.origin )
					return;

				auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( info.sound_source );

				if ( !player )
					return;

				if ( !player->is_player( ) )
					return;

				if ( !info.origin )
					return;

				math::vec3 sound_origin = *info.origin;
				math::vec3 sound_floor  = { sound_origin.x, sound_origin.y, sound_origin.z - 9000 };

				sdk::ray_t ray;
				sdk::c_game_trace trace;
				sdk::c_trace_filter filter;

				filter.skip = player;

				ray.init( sound_origin, sound_floor );
				g_interfaces.engine_trace->trace_ray( ray, sdk::masks::MASK_ALL, &filter, &trace );

				sound_origin = trace.end_pos;

				g_entity_list.players[ player->entity_index( ) ].m_dormant_info.m_last_position = sound_origin;
				g_entity_list.players[ player->entity_index( ) ].m_dormant_info.m_found_tick    = g_interfaces.globals->tick_count;
			}
		}
		break;
	}
}

bool __stdcall hooks::detours::ret_add( LPCSTR )
{
	return true;
}

void __fastcall hooks::detours::modify_eye_position( sdk::c_anim_state* anim_state, void* edx, math::vec3& input_eye_pos )
{
	static auto calc_view_retadd = g_client_dll.pattern_scan( _( "8B ? ? ? ? ? 30 ? ? ? ? C0 ? 50" ) ).as< std::uintptr_t >( );

	if ( reinterpret_cast< std::uintptr_t >( _ReturnAddress( ) ) == calc_view_retadd )
		return;

	return anim_state->modify_eye_position( input_eye_pos );
}

long __stdcall hooks::detours::reset( IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* presentation_parameters )
{
	static auto og = hooks::reset.get_original< decltype( &hooks::detours::reset ) >( );

	if ( !g_menu.menu_initialised )
		return og( device, presentation_parameters );

	ImGui_ImplDX9_InvalidateDeviceObjects( );

	const HRESULT hreset = og( device, presentation_parameters );

	if ( hreset == D3D_OK )
		ImGui_ImplDX9_CreateDeviceObjects( );

	return hreset;
}

LONG __stdcall hooks::detours::end_scene( IDirect3DDevice9* device )
{
	static auto og = hooks::end_scene.get_original< decltype( &hooks::detours::end_scene ) >( );

	static auto static_return = _ReturnAddress( );

	if ( static_return != _ReturnAddress( ) )
		return og( device );

	if ( device ) {
		D3DVIEWPORT9 vp;
		device->GetViewport( &vp );

		g_ctx.screen_size.x = vp.Width;
		g_ctx.screen_size.y = vp.Height;
	}

	g_render.setup_state( );

	g_indicators.render( );

	g_visuals.render( );

	g_log.think( );

	if ( !g_menu.menu_initialised )
		g_imgui.menu_init( device );

	g_imgui.new_frame( );

	g_menu.draw( );

	g_imgui.end_frame( );

	g_render.finish_state( );

	return og( device );
}

void __fastcall hooks::detours::item_post_frame( void* self, void* edx )
{
	static auto og = hooks::item_post_frame.get_original< decltype( &hooks::detours::item_post_frame ) >( );

	static auto prediction_return_address = g_client_dll.pattern_scan( _( "F6 87 ? ? ? ? ? 74 ? C7 87 ? ? ? ? ? ? ? ? 83 BF" ) ).as< void* >( );

	if ( _ReturnAddress( ) == prediction_return_address && g_ctx.running_post_think )
		return;

	return og( self, edx );
}

bool __fastcall hooks::detours::is_loose_file_allowed( void* ecx, void* edx )
{
	return true;
}

void __fastcall hooks::detours::emit_sound( EMIT_SOUND_PARAMS )
{
	static auto og = hooks::emit_sound.get_original< decltype( &hooks::detours::emit_sound ) >( );

	if ( !g_prediction.in_prediction )
		og( EMIT_SOUND_PARAM_CALL );
}

void __fastcall hooks::detours::paint_traverse( sdk::i_panel* self, void* edx, unsigned int panel, bool force_repaint, bool allow_force )
{
	static auto og = hooks::paint_traverse.get_original< decltype( &hooks::detours::paint_traverse ) >( );

	const auto panel_hash = HASH_RT( g_interfaces.panel->panel_name( panel ) );

	if ( static bool already_logged = false; !already_logged ) {
		g_log.print( _( " injection success." ), _( "[hotwheels.vip]" ) );

		already_logged = true;
	}

	if ( panel_hash == HASH_BT( "FocusOverlayPanel" ) ) {
		g_ctx.view_matrix = g_interfaces.engine->world_to_screen_matrix( );

		for ( auto& player_info : g_entity_list.players ) {
			auto player = g_interfaces.entity_list->get_client_entity< sdk::c_cs_player* >( player_info.m_index );

			if ( ( !player_info.m_valid && !player_info.m_dormant_info.m_valid ) || !player )
				continue;

			visuals::esp_object& object = g_visuals.esp_objects[ player->entity_index( ) ];

			if ( object.m_owner != player )
				continue;

			math::vec3 old_origin = player->get_abs_origin( );

			if ( !player_info.m_valid && sdk::ticks_to_time( g_interfaces.globals->tick_count - player_info.m_dormant_info.m_found_tick ) < 3.f ) {
				player->set_abs_origin( sdk::ticks_to_time( g_interfaces.globals->tick_count - player_info.m_dormant_info.m_vouchable_tick ) < 3.f
				                            ? player_info.m_dormant_info.m_vouchable_position
				                            : player_info.m_dormant_info.m_last_position );
				player->invalidate_bone_cache( );
			}

			auto collideable = object.m_owner->get_collideable( );

			if ( !collideable )
				continue;

			player_info.m_mins = collideable->obb_mins( );
			player_info.m_maxs = collideable->obb_maxs( );

			player_info.m_rgfl = player->rgfl_coordinate_frame( );

			if ( !player_info.m_valid && sdk::ticks_to_time( g_interfaces.globals->tick_count - player_info.m_dormant_info.m_found_tick ) < 3.f ) {
				player->set_abs_origin( old_origin );
				player->invalidate_bone_cache( );
			}
		}

		if ( g_menu.menu_open ) {
			g_interfaces.panel->set_input_keyboard_state( panel, true );
			g_interfaces.panel->set_input_mouse_state( panel, true );
		} else {
			g_interfaces.panel->set_input_keyboard_state( panel, false );
			g_interfaces.panel->set_input_mouse_state( panel, false );
		}
	}

	og( self, edx, panel, force_repaint, allow_force );
}

bool __fastcall hooks::detours::send_net_msg( void* ecx, void* edx, sdk::i_net_msg* net_message, bool reliable, bool voice )
{
	static auto og = hooks::send_net_msg.get_original< decltype( &hooks::detours::send_net_msg ) >( );

	// CRC check bypass
	if ( net_message->get_type( ) == 14 )
		return false;

	// fix for fakelag voice lag
	if ( net_message->get_group( ) == sdk::net_channel_type::VOICE )
		voice = true;

	return og( ecx, edx, net_message, reliable, voice );
}
