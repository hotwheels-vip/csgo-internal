#pragma once
#include <iostream>

#include "i_collideable.h"
#include "i_handle_entity.h"

struct i_client_networkable;
struct i_client_renderable;
struct i_client_entity;
struct i_client_thinkable;

namespace sdk
{
	struct c_base_entity;

	struct i_client_unknown : public i_handle_entity {
	public:
		virtual i_collideable* get_collideable( )               = 0;
		virtual i_client_networkable* get_client_networkable( ) = 0;
		virtual i_client_renderable* get_client_renderable( )   = 0;
		virtual i_client_entity* get_i_client_entity( )         = 0;
		virtual c_base_entity* get_base_entity( )               = 0;
		virtual i_client_thinkable* get_client_thinkable( )     = 0;
		virtual std::uintptr_t* get_client_alpha_property( )    = 0;
	};
} // namespace sdk
