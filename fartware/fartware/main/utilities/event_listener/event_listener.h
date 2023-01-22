#pragma once
#include <deque>

#include "../../source_engine/classes/c_game_event_manager.h"

class c_event_listener : public c_game_event_listener2
{
public:
	bool on_attach( const std::deque< const char* >& events );
	void on_release( );

	virtual void fire_game_event( c_game_event* game_event ) override;
	virtual int get_event_debug_id( ) override
	{
		return EVENT_DEBUG_ID_INIT;
	}
};

inline c_event_listener event_listener = { };