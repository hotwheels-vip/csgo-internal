#pragma once
#include "../../../globals/includes/includes.h"

namespace sdk
{
	struct i_localize {
	public:
		wchar_t* find( const char* token_name )
		{
			return virtual_func::call< wchar_t* >( this, 11, token_name );
		}

		const wchar_t* find_safe( const char* token_name )
		{
			return virtual_func::call< const wchar_t* >( this, 12, token_name );
		}
	};
} // namespace sdk
