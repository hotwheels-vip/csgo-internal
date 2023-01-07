#pragma once
#include "../../../globals/includes/includes.h"
#include "../structs/player_info_t.h"
#include "i_net_channel_info.h"

namespace sdk
{
	class iv_engine_client
	{
	public:
		void get_screen_size( int& w, int& h )
		{
			virtual_func::call< void, int&, int& >( this, 5, w, h );
		}
		bool get_player_info( int index, sdk::player_info_t* info )
		{
			return virtual_func::call< bool >( this, 8, index, info );
		}
		int get_player_for_user_id( int id )
		{
			return virtual_func::call< int >( this, 9, id );
		}
		bool is_console_visible( )
		{
			return virtual_func::call< bool >( this, 11 );
		}
		float get_last_time_stamp( )
		{
			return virtual_func::call< float >( this, 14 );
		}
		math::vec3& get_view_angles( )
		{
			math::vec3 ang{ };
			virtual_func::call< void, math::vec3& >( this, 18, ang );
			return ang;
		}
		void set_view_angles( math::vec3& ang )
		{
			virtual_func::call< void, math::vec3& >( this, 19, ang );
		}
		int get_max_clients( )
		{
			return virtual_func::call< int >( this, 20 );
		}
		bool is_in_game( )
		{
			return virtual_func::call< bool >( this, 26 );
		}
		bool is_connected( )
		{
			return virtual_func::call< bool >( this, 27 );
		}
		// @ liga i have no clue what to name this
		const bool is_fully_connected( )
		{
			return is_connected( ) && is_in_game( ) && !is_playing_demo( );
		}
		bool is_drawing_loading_image( )
		{
			return virtual_func::call< bool >( this, 28 );
		}
		const math::matrix_4x4& world_to_screen_matrix( )
		{
			return virtual_func::call< const math::matrix_4x4& >( this, 37 );
		}
		void* get_bsp_tree_query( )
		{
			return virtual_func::call< void* >( this, 43 );
		}
		const char* get_level_name( )
		{
			return virtual_func::call< const char* >( this, 52 );
		}
		const char* get_level_name_short( )
		{
			return virtual_func::call< const char* >( this, 53 );
		}
		sdk::i_net_channel_info* get_net_channel_info( )
		{
			return virtual_func::call< sdk::i_net_channel_info* >( this, 78 );
		}
		bool is_playing_demo( )
		{
			return virtual_func::call< bool >( this, 82 );
		}
		bool is_recording_demo( )
		{
			return virtual_func::call< bool >( this, 83 );
		}
		bool is_paused( )
		{
			return virtual_func::call< bool >( this, 90 );
		}
		float get_timescale( )
		{
			return virtual_func::call< float >( this, 91 );
		}
		bool is_playing_time_demo( )
		{
			return virtual_func::call< bool >( this, 84 );
		}
		bool is_taking_screenshot( )
		{
			return virtual_func::call< bool >( this, 92 );
		}
		bool is_hltv( )
		{
			return virtual_func::call< bool >( this, 93 );
		}
		unsigned int get_engine_build_number( )
		{
			return virtual_func::call< unsigned int >( this, 104 );
		}
		const char* get_product_version_string( )
		{
			return virtual_func::call< const char* >( this, 105 );
		}
		int get_local_player( )
		{
			return virtual_func::call< int >( this, 12 );
		}
		bool is_voice_recording( )
		{
			return virtual_func::call< bool >( this, 224 );
		}
		void execute_client_cmd( const char* st )
		{
			virtual_func::call< void >( this, 108, st );
		}
		void client_cmd_unrestricted( const char* st, bool from_console_or_keybind = false )
		{
			virtual_func::call< void >( this, 114, st, from_console_or_keybind );
		}
		void fire_events( )
		{
			static auto fire_events_address =
				g_engine_dll.pattern_scan( "e9 ? ? ? ? cc cc cc cc cc cc cc cc cc cc cc 8b 0d ? ? ? ? 8d 89" ).as< std::uintptr_t >( ); /* xd looks funny */
			using fire_events_type = void( __cdecl* )( );

			reinterpret_cast< fire_events_type >( fire_events_address )( );
		}
	};
} // namespace sdk
