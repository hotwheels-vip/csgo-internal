#pragma once
#include "../classes/c_utl_memory.h"
#include "../datatypes/datamap.h"

typedef void ( *fn_field_compare_t )( const char* classname, const char* fieldname, const char* fieldtype, bool networked, bool noterrorchecked,
                                      bool differs, bool withintolerance, const char* value );

class c_prediction_copy
{
public:
	typedef enum {
		differs = 0,
		identical,
		withintolerance,
	} e_difference_type;

	typedef enum {
		transferdata_copyonly = 0,      
		transferdata_errorcheck_nospew, 
		transferdata_errorcheck_spew,   
		transferdata_errorcheck_describe,
	} e_operation_type;

	c_prediction_copy( ) { }
	c_prediction_copy( int type, unsigned char* dest, bool dest_packed, const unsigned char* src, bool src_packed, e_operation_type operation_type,
	                   fn_field_compare_t func = nullptr )
	{
		this->m_operation_type    = operation_type;
		this->m_type              = type;
		this->m_dest              = dest;
		this->m_src               = src;
		this->m_dest_offset_index = dest_packed ? td_offset_packed : td_offset_normal;
		this->m_src_offset_index  = src_packed ? td_offset_packed : td_offset_normal;

		this->m_error_count = 0;
		this->m_ent_index   = -1;

		this->m_watch_field        = nullptr;
		this->m_field_compare_func = func;
	}

	int transfer_data( const char* operation, int entindex, data_map_t* dmap );

private:
	e_operation_type m_operation_type;
	int m_type;
	unsigned char* m_dest;
	const unsigned char* m_src;
	int m_dest_offset_index;
	int m_src_offset_index;
	int m_error_count;
	int m_ent_index;
	fn_field_compare_t m_field_compare_func;
	const type_description_t* m_watch_field;
	char const* m_operation;
	c_utl_stack< const type_description_t* > m_field_stack;
};