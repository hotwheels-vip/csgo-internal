#define _CRT_SECURE_NO_WARNINGS

#include "media_player.h"
#include <d3dx9tex.h>

void n_media_player::impl_t::init( )
{
	this->m_session_manager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync( ).get( );
}

char* alloc_wcstcs( winrt::hstring source )
{
	char* string_alloc = ( char* )malloc( ( source.size( ) + 1 ) * sizeof( char ) );
	wcstombs( string_alloc, source.c_str( ), source.size( ) + 1 );
	return string_alloc;
}

void* last_thumb_buffer = nullptr;
int last_m_current_time = -1;
int paused_ticks        = 0;

concurrency::task< void > n_media_player::impl_t::on_update( IDirect3DDevice9* device )
{
	this->m_session = this->m_session_manager->GetCurrentSession( );

	if ( this->m_session != nullptr ) {
		this->m_has_media = true;

		this->m_source_app_user_mode_id = alloc_wcstcs( this->m_session->SourceAppUserModelId( ) );

		auto info                 = this->m_session->TryGetMediaPropertiesAsync( ).get( );
		this->m_title             = alloc_wcstcs( info.Title( ) );
		this->m_artist            = alloc_wcstcs( info.Artist( ) );
		this->m_album_artist      = alloc_wcstcs( info.AlbumArtist( ) );
		this->m_album_title       = alloc_wcstcs( info.AlbumTitle( ) );
		this->m_track_number      = info.TrackNumber( );
		this->m_album_track_count = info.AlbumTrackCount( );

		if ( info.Thumbnail( ) ) {
			auto thumbnail_stream  = co_await info.Thumbnail( ).OpenReadAsync( );
			this->m_thumbnail_type = alloc_wcstcs( thumbnail_stream.ContentType( ) );
			Buffer buffer          = Buffer( thumbnail_stream.Size( ) );
			co_await thumbnail_stream.ReadAsync( buffer, buffer.Capacity( ), InputStreamOptions::ReadAhead );

			this->m_thumbnail_buffer = malloc( buffer.Length( ) );
			memcpy( this->m_thumbnail_buffer, buffer.data( ), buffer.Length( ) );
			this->m_thumbnail_size = buffer.Length( );

			if ( this->m_thumbnail_buffer != last_thumb_buffer ) {
				D3DXCreateTextureFromFileInMemoryEx( device, this->m_thumbnail_buffer, this->m_thumbnail_size, 512, 512, D3DX_DEFAULT, 0,
				                                     D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &this->m_image );
				last_thumb_buffer = this->m_thumbnail_buffer;
			}
		} else {
			this->m_thumbnail_size = 0;
			this->m_image          = nullptr;
		}

		const auto timeline_properties = this->m_session->GetTimelineProperties( );
		this->m_total_time             = timeline_properties.EndTime( ).count( );
		this->m_current_time           = timeline_properties.Position( ).count( );

		if ( last_m_current_time == this->m_current_time )
			paused_ticks++;
		else
			paused_ticks = 0;

		if ( paused_ticks >= 5 )
			this->m_is_paused = true;

		last_m_current_time = this->m_current_time;
	} else {
		this->m_has_media = false;
	}
}

void n_media_player::impl_t::toggle_play_pause( )
{
	if ( !this->m_has_media )
		return;

	this->m_session->TryTogglePlayPauseAsync( );
	if ( !this->m_is_paused ) {
		paused_ticks      = 10;
		this->m_is_paused = true;
	} else {
		paused_ticks      = 0;
		this->m_is_paused = false;
	}
}

void n_media_player::impl_t::pause( )
{
	if ( !this->m_has_media )
		return;

	this->m_session->TryPauseAsync( );
	if ( !this->m_is_paused ) {
		paused_ticks      = 10;
		this->m_is_paused = true;
	}
}

void n_media_player::impl_t::play( )
{
	if ( !this->m_has_media )
		return;

	this->m_session->TryPlayAsync( );

	this->m_is_paused = false;
	paused_ticks      = 0;
}

void n_media_player::impl_t::set_duration( float duration )
{
	if ( !this->m_has_media )
		return;

	if ( this->m_total_time > 0 )
		this->m_session->TryChangePlaybackPositionAsync( int64_t( this->m_total_time * duration ) );
}

int n_media_player::impl_t::get_time_by_seconds( )
{
	if ( !this->m_has_media )
		return 0;

	if ( this->m_total_time > 0 ) {
		std::chrono::milliseconds milliseconds( this->m_current_time / 10000 );
		const auto seconds = std::chrono::duration_cast< std::chrono::seconds >( milliseconds );
		return seconds.count( );
	}

	return 0;
}

int n_media_player::impl_t::get_time_by_minutes( )
{
	if ( !this->m_has_media )
		return 0;

	if ( this->m_total_time > 0 ) {
		std::chrono::seconds seconds( this->get_time_by_seconds( ) );
		const auto minutes = std::chrono::duration_cast< std::chrono::minutes >( seconds );
		return minutes.count( );
	}

	return 0;
}

std::string n_media_player::impl_t::get_time_formatted( )
{
	if ( !this->m_has_media )
		return "";

	const auto seconds = this->get_time_by_seconds( );
	const auto minutes = this->get_time_by_minutes( );

	if ( minutes < 1 ) {
		if ( seconds < 10 )
			return std::vformat( "0:0{}", std::make_format_args( seconds ) );
		else
			return std::vformat( "0:{}", std::make_format_args( seconds ) );
	}

	if ( minutes > 0 && seconds - ( 60 * minutes ) < 9 ) {
		return std::vformat( "{}:0{}", std::make_format_args( minutes, seconds - ( 60 * minutes ) ) );
	} else if ( minutes > 0 ) {
		return std::vformat( "{}:{}", std::make_format_args( minutes, seconds - ( 60 * minutes ) ) );
	}

	return "";
}

void n_media_player::impl_t::skip( )
{
	if ( !this->m_has_media )
		return;

	this->m_session->TrySkipNextAsync( );
	this->m_is_paused = false;
	paused_ticks      = 0;
}

void n_media_player::impl_t::rewind( )
{
	if ( !this->m_has_media )
		return;

	this->m_session->TrySkipPreviousAsync( );
	this->m_is_paused = false;
	paused_ticks      = 0;
}
