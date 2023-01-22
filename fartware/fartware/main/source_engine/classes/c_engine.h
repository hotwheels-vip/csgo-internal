#pragma once
#include "../../../dependencies/steam/isteamclient.h"
#include "../classes/c_angle.h"
#include "../structs/matrix_t.h"

struct player_info_t {
	unsigned long long m_version = 0ULL;
	union {
		unsigned long long m_xuid;
		struct {
			unsigned int m_xuid_low;
			unsigned int m_xuid_high;
		};
	};

	char m_name[ 128 ];
	int m_user_id;
	char m_steam_id[ 33 ];
	unsigned int m_friends_id;
	char m_friends_name[ 128 ];
	bool m_fake_player;
	bool m_is_hltv;
	unsigned int m_custom_files[ 4 ];
	unsigned char m_files_downloaded;
};

struct steam_api_context_t {
	ISteamClient* m_steam_client;
	ISteamUser* m_steam_user;
	ISteamFriends* m_steam_friends;
	ISteamUtils* m_steam_utils;
	ISteamMatchmaking* m_steam_matchmaking;
	void* m_steam_game_search;
	ISteamMatchmakingServers* m_steam_matchmaking_servers;
	ISteamUserStats* m_steam_user_stats;
	ISteamApps* m_steam_apps;
	ISteamNetworking* m_steam_networking;
	ISteamRemoteStorage* m_steam_remote_storage;
	ISteamScreenshots* m_steam_screenshots;
	ISteamHTTP* m_steam_http;
	ISteamController* m_controller;
	ISteamUGC* m_steam_ugc;
	ISteamAppList* m_steam_app_list;
	ISteamMusic* m_steam_music;
	ISteamMusicRemote* m_steam_music_remote;
	ISteamHTMLSurface* m_steam_html_surface;
	ISteamInventory* m_steam_inventory;
	ISteamVideo* m_steam_video;
	ISteamParentalSettings* m_steam_parental_settings;
	void* m_steam_input;
};

class c_engine
{
private:
	enum e_indexes {
		_get_player_info         = 8,
		_get_player_for_userid   = 9,
		_get_local_player        = 12,
		_get_view_angles         = 18,
		_set_view_angles         = 19,
		_is_in_game              = 26,
		_cull_box                = 34,
		_world_to_screen_matrix  = 37,
		_get_level_name          = 52,
		_get_level_name_short    = 53,
		_is_hltv                 = 93,
		_is_playing_demo         = 82,
		_steam_api_context       = 185,
		_client_cmd_unrestricted = 114,
	};

public:
	// todo: clean this shit
	bool is_in_game( )
	{
		using fn = bool( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_is_in_game ]( this );
	}

	int get_player_for_userid( int user_id )
	{
		using fn = int( __thiscall* )( c_engine*, int );
		return ( *( fn** )this )[ this->e_indexes::_get_player_for_userid ]( this, user_id );
	}

	bool is_playing_demo( )
	{
		using fn = bool( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_is_playing_demo ]( this );
	}

	bool is_hltv( )
	{
		using fn = bool( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_is_hltv ]( this );
	}

	void get_view_angles( c_angle& view_angle )
	{
		using fn = void( __thiscall* )( c_engine*, c_angle& );
		return ( *( fn** )this )[ this->e_indexes::_get_view_angles ]( this, std::ref( view_angle ) );
	}

	void set_view_angles( c_angle& view_angle )
	{
		using fn = void( __thiscall* )( c_engine*, c_angle& );
		return ( *( fn** )this )[ this->e_indexes::_set_view_angles ]( this, std::ref( view_angle ) );
	}

	bool get_player_info( int index, player_info_t* player_info )
	{
		using fn = bool( __thiscall* )( c_engine*, int, player_info_t* );
		return ( *( fn** )this )[ this->e_indexes::_get_player_info ]( this, index, player_info );
	}
	void client_cmd_unrestricted( const char* szCommand )
	{
		using fn = void( __thiscall* )( void*, const char*, bool );
		return ( *( fn** )this )[ this->e_indexes::_client_cmd_unrestricted ]( this, szCommand, false );
	}
	int get_local_player( )
	{
		using fn = int( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_get_local_player ]( this );
	}

	bool cull_box( const c_vector& mins, const c_vector& ma )
	{
		using fn = bool( __thiscall* )( c_engine*, const c_vector&, const c_vector& );
		return ( *( fn** )this )[ this->e_indexes::_cull_box ]( this, std::cref( mins ), std::cref( ma ) );
	}

	const view_matrix_t& world_to_screen_matrix( )
	{
		using fn = const view_matrix_t&( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_world_to_screen_matrix ]( this );
	}

	const char* get_level_name( )
	{
		using fn = const char*( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_get_level_name ]( this );
	}

	const char* get_level_name_short( )
	{
		using fn = const char*( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_get_level_name_short ]( this );
	}

	steam_api_context_t* steam_api_context( )
	{
		using fn = steam_api_context_t*( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_steam_api_context ]( this );
	}
};
