
namespace hooks
{
	inline WNDPROC wnd_proc;

	// pointers
	inline detour::hook create_move;
	inline detour::hook send_net_msg;
	inline detour::hook frame_stage_notify;
	inline detour::hook emit_sound;
	inline detour::hook glow_effet_spectator;
	inline detour::hook end_scene;
	inline detour::hook paint_traverse;
	inline detour::hook reset;
	inline detour::hook modify_eye_position;
	inline detour::hook list_leaves_in_box;
	inline detour::hook item_post_frame;
	inline detour::hook is_loose_file_allowed;

	// functions
	struct detours {
		static bool __fastcall is_loose_file_allowed( void*, void* );

		static int __fastcall list_leaves_in_box( void*, void*, math::vec3&, math::vec3&, unsigned short*, int );

		static void __fastcall item_post_frame( void*, void* );

		static void __fastcall modify_eye_position( sdk::c_anim_state*, void*, math::vec3& );

		static LONG WINAPI wnd_proc( HWND , UINT , WPARAM , LPARAM  );

		static void __stdcall create_move( int, float, bool, bool& );
		static void __fastcall create_move_proxy( void*, int, int, float, bool );

		static void __fastcall frame_stage_notify( void*, void*, sdk::frame_stage );

		static bool __cdecl glow_effect_spectator( sdk::c_cs_player*, sdk::c_cs_player*, sdk::glow_style&, math::vec3&, float&, float&, float&,
		                                           float&, bool& );

		static LONG __stdcall end_scene( IDirect3DDevice9* );

		static long __stdcall reset( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );

		static void __fastcall emit_sound( EMIT_SOUND_PARAMS );

		static void __fastcall paint_traverse( sdk::i_panel*, void*, unsigned int, bool, bool );

		static bool __stdcall ret_add( LPCSTR );

		static bool __fastcall send_net_msg( void*, void*, sdk::i_net_msg*, bool, bool );
	};

	// ret_add hooks
	namespace ret_add
	{
		inline detour::hook return_address_hook_client;
		inline detour::hook return_address_hook_engine;
		inline detour::hook return_address_hook_studio;
		inline detour::hook return_address_hook_material;

		static bool init( )
		{
			HOOK( hooks::ret_add::return_address_hook_client.create( g_client_dll.pattern_scan( _( "55 8B EC 56 8B F1 33 C0 57" ) ).as< void* >( ),
			                                                         &hooks::detours::ret_add ) );

			HOOK( hooks::ret_add::return_address_hook_engine.create( g_engine_dll.pattern_scan( _( "55 8B EC 56 8B F1 33 C0 57" ) ).as< void* >( ),
			                                                         &hooks::detours::ret_add ) );

			HOOK( hooks::ret_add::return_address_hook_studio.create(
				g_studio_render_dll.pattern_scan( _( "55 8B EC 56 8B F1 33 C0 57" ) ).as< void* >( ), &hooks::detours::ret_add ) );

			HOOK( hooks::ret_add::return_address_hook_material.create(
				g_material_system_dll.pattern_scan( _( "55 8B EC 56 8B F1 33 C0 57" ) ).as< void* >( ), &hooks::detours::ret_add ) );

			return true;
		};
	}
} // namespace hooks