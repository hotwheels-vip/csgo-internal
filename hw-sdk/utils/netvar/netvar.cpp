#include "netvar.h"

// sdk::recv_var_proxy_fn netvar::find_proxy_table( sdk::recv_table* table, const char* var, sdk::recv_var_proxy_fn function )
//{
//	return sdk::recv_var_proxy_fn( );
// }
//
// sdk::recv_var_proxy_fn netvar::change_proxy( const char* table, const char* var, sdk::recv_var_proxy_fn function )
//{
//	return sdk::recv_var_proxy_fn( );
// }

std::uintptr_t netvar::get_offset( sdk::recv_table* table, const char* var )
{
	for ( std::int32_t i = 0; i < table->props; i++ ) {
		auto current_prop = table->props_ptr[ i ];

		if ( HASH_RT( var ) == HASH_RT( current_prop.var_name ) )
			return current_prop.offset;

		if ( auto data_table = current_prop.data_table ) {
			std::uintptr_t offset = get_offset( data_table, var );

			if ( offset )
				return offset + current_prop.offset;
		}
	}

	return std::uintptr_t( );
}

std::uintptr_t netvar::get_table( const char* table, const char* var )
{
	auto classes = g_interfaces.client->get_all_classes( );

	for ( auto current_table = classes; current_table; current_table = current_table->next ) {
		if ( HASH_RT( table ) == HASH_RT( current_table->network_name ) )
			return get_offset( current_table->recv_table, var );
	}

	return std::uintptr_t( );
}

// std::uintptr_t netvar::get_table( sdk::data_map* table, const char* var )
//{
//	for ( auto current_map = table; current_map; current_map = current_map->base_map ) {
//		for ( auto current_field = table->fields; current_field; current_field++ ) {
//			auto table_description = table->description[ current_field ];
//
//			if ( table_description.name )
//				continue;
//
//			if ( strstr( table_description.name, var ) /*HASH( table_description.name ) == HASH( var )*/ )
//				return table_description.offset[ static_cast< unsigned int >( sdk::td_offset::NORMAL ) ]; // thanx c++
//
//			if ( table_description.type == sdk::field_type::EMBEDDED && table_description.type_description) {
//				auto offset = get_table( table_description.type_description, var );
//
//				return offset;
//			}
//		}
//	}
//
//	return std::uintptr_t( );
// }
