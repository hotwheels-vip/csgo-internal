#pragma once
#include "../../../globals/includes/includes.h"
#include "i_handle_entity.h"

#define NUM_ENT_ENTRY_BITS        ( 11 + 2 )
#define NUM_ENT_ENTRIES           ( 1 << NUM_ENT_ENTRY_BITS )
#define INVALID_EHANDLE_INDEX     0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS       16
#define NUM_SERIAL_NUM_SHIFT_BITS ( 32 - NUM_SERIAL_NUM_BITS )
#define ENT_ENTRY_MASK            ( ( 1 << NUM_SERIAL_NUM_BITS ) - 1 )

namespace sdk
{
	template< class T >
	class c_handle
	{
	public:
		c_handle( ){ };

		const DWORD& operator=( const T* val )
		{
			auto entity = reinterpret_cast< i_handle_entity* >( val );

			if ( entity )
				*this = entity->get_ref_ehandle( );
			else
				index = INVALID_EHANDLE_INDEX;
		}

		constexpr bool is_valid( ) const
		{
			return index != INVALID_EHANDLE_INDEX;
		}

	private:
		DWORD index;
	};
} // namespace sdk
