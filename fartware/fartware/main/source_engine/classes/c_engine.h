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

#define FLOW_OUTGOING 0
#define FLOW_INCOMING 1
#define MAX_FLOWS     2

class c_net_channel_info
{
public:
	enum {
		generic = 0, 
		localplayer, 
		otherplayers,
		entities,    
		sounds,      
		events,      
		tempents,    
		usermessages,
		entmessages, 
		voice,       
		stringtable, 
		move,        
		stringcmd,   
		signon,      
		paintmap,    
		encrypted,   
		total,       
	};

	virtual const char* get_name( ) const     = 0; 
	virtual const char* get_address( ) const  = 0; 
	virtual float get_time( ) const           = 0; 
	virtual float get_time_connected( ) const = 0; 
	virtual int get_buffer_size( ) const      = 0; 
	virtual int get_data_rate( ) const        = 0; 

	virtual bool is_loopback( ) const                                                     = 0;
	virtual bool is_timing_out( ) const                                                   = 0;
	virtual bool is_playback( ) const                                                     = 0;
	virtual float get_latency( int i_flow ) const                                         = 0;
	virtual float get_avg_latency( int i_flow ) const                                     = 0;
	virtual float get_avg_loss( int i_flow ) const                                        = 0;
	virtual float get_avg_choke( int i_flow ) const                                       = 0;
	virtual float get_avg_data( int i_flow ) const                                        = 0;
	virtual float get_avg_packets( int i_flow ) const                                     = 0;
	virtual int get_total_data( int i_flow ) const                                        = 0;
	virtual int get_total_packets( int i_flow ) const                                     = 0;
	virtual int get_sequence_nr( int i_flow ) const                                       = 0;
	virtual bool is_valid_packet( int i_flow, int n_frame ) const                         = 0;
	virtual float get_packet_time( int i_flow, int n_frame ) const                        = 0;
	virtual int get_packet_bytes( int i_flow, int n_frame, int group ) const              = 0;
	virtual bool get_stream_progress( int i_flow, int* pi_received, int* pi_total ) const = 0;
	virtual float get_time_since_last_received( ) const                                   = 0;
	virtual float get_command_interpolation_amount( int i_flow, int n_frame ) const       = 0;
	virtual void get_packet_response_latency( int i_flow, int frame_number, int* pn_latency_msecs, int* pn_choke ) const = 0;
	virtual void get_remote_framerate( float* pfl_frame_time, float* pfl_frame_time_std_deviation,
	                                   float* pfl_frame_start_time_std_deviation ) const                                 = 0;
	virtual float get_timeout_seconds( ) const                                                                           = 0;
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
		_get_net_channel_info    = 78,
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

	c_net_channel_info* get_net_channel_info() {
		using fn = c_net_channel_info*( __thiscall* )( c_engine* );
		return ( *( fn** )this )[ this->e_indexes::_get_net_channel_info ]( this );
	}
};
