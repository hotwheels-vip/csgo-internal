#include "convars.h"

/* dont have to include all of the fcking source sdk classes */
#include "../../game/sdk/classes/c_cconvar.h"
#include "../../game/sdk/classes/c_con_base.h"
#include "../../game/sdk/classes/c_convar.h"
#include "../interfaces/interfaces.h"

#include "../../dependencies/fnv1a/fnv1a.h"
#include <unordered_map>

std::unordered_map< unsigned int, c_cconvar* > convars = { };

bool n_convars::impl_t::on_attach( )
{
	c_con_base* iterator = **reinterpret_cast< c_con_base*** >( reinterpret_cast< unsigned long >( g_interfaces.m_convar ) + 0x34 );
	if ( !iterator )
		return false;

	for ( auto c = iterator->m_next; c; c = c->m_next ) {
		const auto convar_name        = c->m_name;
		const auto hashed_convar_name = HASH_RT( convar_name );

		convars[ hashed_convar_name ] = ( c_cconvar* )c->m_accessor;
	}

	return !( convars.empty( ) );
}

c_cconvar* n_convars::impl_t::operator[]( unsigned int hash )
{
	return convars[ hash ];
}