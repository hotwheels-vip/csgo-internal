#pragma once

#pragma comment( lib, "windowsapp" )

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/base.h>

#include <chrono>
#include <functional>
#include <sstream>

#include <pplawait.h>
#include <ppltasks.h>

#include <d3d9.h>

using namespace winrt;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media::Control;
using namespace Windows::Storage::Streams;
using namespace Windows::Foundation::Collections;
using namespace winrt::Windows::Media::Control;

namespace n_media_player
{
	struct impl_t {
		void init( );
		concurrency::task< void > on_update( IDirect3DDevice9* device );
		void toggle_play_pause( );
		void pause( );
		void play( );
		void set_duration( float duration );

		bool m_has_media{ };
		bool m_is_paused{ };
		std::optional< GlobalSystemMediaTransportControlsSession > m_session{ };
		char* m_source_app_user_mode_id{ };
		char* m_title{ };
		char* m_artist{ };
		char* m_album_artist{ };
		char* m_album_title{ };
		int m_track_number{ };
		int m_album_track_count{ };
		int m_playback_type{ };
		char* m_thumbnail_type{ };
		void* m_thumbnail_buffer{ };
		int m_thumbnail_size{ };

		IDirect3DTexture9* m_image{ };

		int m_total_time{ };
		int m_current_time{ };

		int get_time_by_seconds( );
		int get_time_by_minutes( );
		std::string get_time_formatted( );
		void skip( );
		void rewind( );

	private:
		std::optional< GlobalSystemMediaTransportControlsSessionManager > m_session_manager;
	};
} // namespace n_media_player

inline n_media_player::impl_t g_media_player{ };
