#pragma once
#include <iostream>

namespace sdk
{
	typedef std::uintptr_t c_base_handle;

	class i_handle_entity
	{
	public:
		virtual ~i_handle_entity( ){ };
		virtual void set_ref_ehandle( const c_base_handle& handle ) = 0;
		virtual const c_base_handle& get_ref_ehandle( ) const       = 0;
	};
} // namespace sdk
