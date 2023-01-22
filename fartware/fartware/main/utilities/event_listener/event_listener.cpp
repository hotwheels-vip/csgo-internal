#include "event_listener.h"
#include "../../source_engine/interfaces/interfaces.h"
#include "../../utilities/../hashing/strings/fnv1a.h"
#include "../../utilities/console/console.h"

bool c_event_listener::on_attach( const std::deque< const char* >& events )
{
	if ( events.empty( ) ) {
		console.print( "event listener array returned empty()" );
		return false;
	}

	for ( const auto& event_name : events ) {
		interfaces.m_game_event->add_listener( this, event_name, false );

		if ( !interfaces.m_game_event->find_listener( this, event_name ) )
			console.print( "failed to add listener {:s}", event_name );
		else
			console.print( "added listener {:s}", event_name );
	}

	return true;
}

void c_event_listener::on_release( )
{
	interfaces.m_game_event->remove_listener( this );
}

void c_event_listener::fire_game_event( c_game_event* game_event )
{
	if ( !game_event )
		return;

	const unsigned int hashed_event_name = fnv1a::hash( game_event->get_name( ) );

	/* what ever small example, self explanatory */
	/*if ( hashed_event_name == fnv1a::hash_const( "player_death" ) ) {
	    const auto attacker =
	        interfaces.m_client_entity_list->get_client_entity( interfaces.m_engine->get_player_for_userid( game_event->get_int( "attacker" ) ) );
	}*/
}