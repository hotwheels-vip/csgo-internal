#pragma once

#include "dt_common.h"

namespace sdk
{
	struct recv_table;
	struct recv_prop;

	struct recv_proxy_data {
		const recv_prop* recv_prop;
		sdk::d_variant value;
		std::int32_t element;
		std::int32_t object_id;
	};

	using array_length_recv_proxy_fn   = void( __cdecl* )( void*, std::int32_t, std::int32_t );
	using data_table_recv_var_proxy_fn = void( __cdecl* )( recv_prop*, void**, void*, std::int32_t );
	using recv_var_proxy_fn            = void( __cdecl* )( recv_proxy_data*, void*, void* );

	struct std_recv_proxies {
		recv_var_proxy_fn int32_to_int8;
		recv_var_proxy_fn int32_to_int16;
		recv_var_proxy_fn int32_to_int32;
		recv_var_proxy_fn int64_to_int64;
		recv_var_proxy_fn float_to_float;
		recv_var_proxy_fn vector_to_vector;
	};

	struct recv_prop {
		char* var_name;
		sdk::prop_type recv_type;
		std::int32_t flags;
		std::int32_t string_buffer_size;
		bool inside_array;
		void* extra_data;
		recv_prop* array_prop;
		array_length_recv_proxy_fn array_length_proxy;
		recv_var_proxy_fn proxy_fn;
		data_table_recv_var_proxy_fn data_table_proxy_function;
		recv_table* data_table;
		std::int32_t offset;
		std::int32_t element_stride;
		std::int32_t elements;
		const char* parent_array_prop_name;
	};

	struct recv_table {
		recv_prop* props_ptr;
		std::int32_t props;
		void* decoder;
		char* net_table_name;
		bool initialized;
		bool in_main_list;
	};
} // namespace sdk
