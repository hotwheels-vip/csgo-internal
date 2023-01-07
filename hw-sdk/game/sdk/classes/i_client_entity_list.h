#pragma once

#include "../../../dependencies/hash/hash.h"
#include "../../../dependencies/xor/xor.h"
#include "../../../globals/macros/macros.h"
#include "../../../utils/math/types/vector.h"

#include "../classes/c_handle.h"
#include "i_client_entity.h"

namespace sdk
{
	struct i_client_entity_list {
		virtual sdk::i_client_networkable get_client_networkable( int index )                        = 0;
		virtual sdk::i_client_networkable get_client_networkable_from_handle( c_base_handle handle ) = 0;

		virtual sdk::i_client_unknown get_client_unknown_from_handle( c_base_handle handle ) = 0;

	private: // sorry for the names, cant build with it being the same name.
		virtual sdk::i_client_entity* private_get_client_entity( int index )                        = 0;
		virtual sdk::i_client_entity* private_get_client_entity_from_handle( c_base_handle handle ) = 0;

	public:
		virtual int number_of_entities( bool include_non_networkable ) = 0;
		virtual int get_highest_entity_index( )                        = 0;

		template< class T >
		__forceinline T get_client_entity( int index )
		{
			return private_get_client_entity( index )->as< T >( );
		};

		template< class T >
		__forceinline T get_client_entity_from_handle( c_base_handle handle )
		{
			return private_get_client_entity_from_handle( handle )->as< T >( );
		}
	};
} // namespace sdk
