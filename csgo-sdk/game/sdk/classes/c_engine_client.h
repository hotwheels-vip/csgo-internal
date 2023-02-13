#pragma once
#include "../../../game/sdk/classes/c_angle.h"
#include "../../../game/sdk/classes/c_angle.h"
#include "../../../utilities/memory/virtual.h"

struct player_info_t;
struct view_matrix_t;

class c_net_channel_info;

class c_engine_client
{
public:
	bool get_player_info( int index, player_info_t* info )
	{
		return g_virtual.call< bool >( this, 8, index, info );
	}

	int get_local_player( )
	{
		return g_virtual.call< int >( this, 12 );
	}

	int get_max_clients( )
	{
		return g_virtual.call< int >( this, 20 );
	}

	bool is_in_game( )
	{
		return g_virtual.call< bool >( this, 26 );
	}

	bool is_connected( )
	{
		return g_virtual.call< bool >( this, 27 );
	}

	void* get_bsp_tree_query( )
	{
		return g_virtual.call< void* >( this, 43 );
	}

	const char* get_level_name( )
	{
		return g_virtual.call< const char* >( this, 52 );
	}

	const char* get_level_name_short( )
	{
		return g_virtual.call< const char* >( this, 53 );
	}

	c_net_channel_info* get_net_channel_info( )
	{
		return g_virtual.call< c_net_channel_info* >( this, 78 );
	}

	bool is_playing_demo( )
	{
		return g_virtual.call< bool >( this, 82 );
	}

	bool is_recording_demo( )
	{
		return g_virtual.call< bool >( this, 83 );
	}

	bool is_playing_time_demo( )
	{
		return g_virtual.call< bool >( this, 84 );
	}

	bool is_taking_screenshot( )
	{
		return g_virtual.call< bool >( this, 92 );
	}

	bool is_hltv( )
	{
		return g_virtual.call< bool >( this, 93 );
	}

	void set_view_angles( c_angle& view_angle )
	{
		g_virtual.call< void >( this, 19, view_angle );
	}

	void get_view_angles( c_angle& view_angle )
	{
		g_virtual.call< void >( this, 18, std::ref(view_angle) );
	}

	unsigned int get_engine_build_number( )
	{
		return g_virtual.call< unsigned int >( this, 104 );
	}

	const char* get_product_version_string( )
	{
		return g_virtual.call< const char* >( this, 105 );
	}

	const view_matrix_t& get_world_to_screen_matrix( )
	{
		return g_virtual.call< const view_matrix_t& >( this, 37 );
	}

	void execute_client_cmd( const char* cmd_string )
	{
		g_virtual.call< void >( this, 108, cmd_string );
	}

	void client_cmd_unrestricted( const char* cmd_string, bool from_console_or_keybind = false )
	{
		g_virtual.call< void >( this, 114, cmd_string, from_console_or_keybind );
	}
};
