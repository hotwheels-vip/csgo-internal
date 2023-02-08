#include "convars.h"

/* dont have to include all of the fcking source sdk classes */
#include "../../game/sdk/classes/c_cconvar.h"
#include "../../game/sdk/classes/c_con_base.h"
#include "../../game/sdk/classes/c_convar.h"
#include "../interfaces/interfaces.h"

#include "../macros/macros.h"
#include <unordered_map>

std::unordered_map< unsigned int, c_cconvar* > convars = { };

bool n_convars::impl_t::on_attach( )
{
	c_con_base* iterator = **reinterpret_cast< c_con_base*** >( reinterpret_cast< unsigned long >( g_interfaces.m_convar ) + 0x34 ); /* 0x13f62db8 */
	if ( !iterator )
		return false;

	for ( auto c = iterator->m_next; c; c = c->m_next )
		convars[ HASH_RT( c->m_name ) ] = reinterpret_cast< c_cconvar* >( c->m_accessor );

	return !( convars.empty( ) );
}

c_cconvar* n_convars::impl_t::operator[]( unsigned int hash )
{
	return convars[ hash ];
}
