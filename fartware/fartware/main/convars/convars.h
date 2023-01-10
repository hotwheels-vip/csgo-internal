#pragma once
#include <unordered_map>

class c_cconvar;

struct convars_t {
	bool on_attach( );

	c_cconvar* find( unsigned int hash )
	{
		return this->m_convars[ hash ];
	}

private:
	std::unordered_map< unsigned int, c_cconvar* > m_convars = { };
};

inline convars_t convars = { };