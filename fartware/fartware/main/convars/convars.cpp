#include "convars.h"
#include "../hashing/strings/fnv1a.h"
#include "../source_engine/classes/c_convar.h"
#include "../source_engine/interfaces/interfaces.h"
#include "../utilities/console/console.h"

bool convars_t::on_attach( )
{
	c_conbase* iterator = **reinterpret_cast< c_conbase*** >( reinterpret_cast< unsigned long >( interfaces.m_convar ) + 0x34 );
	if ( !iterator )
		return false;

	for ( auto c = iterator->m_next; c != nullptr; c = c->m_next ) {
		const auto convar_name = c->m_name;
		const auto hashed_convar_name = fnv1a::hash( convar_name );

		auto convar_value = interfaces.m_convar->find_var( convar_name );
		if ( !convar_value )
			continue;

		this->m_convars[ hashed_convar_name ] = convar_value;
	}

	return true;
}