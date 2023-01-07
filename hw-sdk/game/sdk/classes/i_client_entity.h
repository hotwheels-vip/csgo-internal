#pragma once

#include <iostream>

#include "i_client_networkable.h"
#include "i_client_renderable.h"
#include "i_client_thinkable.h"
#include "i_client_unknown.h"

#include "../../../utils/modules/modules.h"

namespace sdk
{
	struct i_client_entity : public i_client_unknown, public i_client_renderable, public i_client_networkable, public i_client_thinkable {
		virtual void release( ) = 0;

		virtual math::vec3& get_abs_origin( ) = 0;
		virtual math::vec3& get_abs_angles( ) = 0;

		void set_abs_origin( math::vec3 origin )
		{
			static auto set_abs_origin_address = g_client_dll.pattern_scan( _( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8" ) ).as< std::uintptr_t >( );

			reinterpret_cast< void( __thiscall* )( void*, math::vec3& ) >( set_abs_origin_address )( this, origin );
		}

		template< typename T >
		T as( )
		{
			return reinterpret_cast< T >( this );
		}
	};
} // namespace sdk
