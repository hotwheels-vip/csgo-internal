#pragma once
#include "i_client_unknown.h"
#include "../structs/i_client_class.h"

struct bf_read;

namespace sdk
{
	struct i_client_networkable {
	public:
		virtual i_client_unknown* get_i_client_unknown( )   = 0;
		virtual void release( )                             = 0;
		virtual client_class* get_client_class( )           = 0;
		virtual void notify_should_transmit( int state )    = 0;
		virtual void on_pre_data_changed( int update_type ) = 0;
		virtual void on_data_changed( int update_type )     = 0;
		virtual void pre_data_update( int update_type )     = 0;
		virtual void post_data_update( int update_type )    = 0;

	private:
		virtual void padding( ) = 0;

	public:
		virtual bool is_dormant( )                                     = 0;
		virtual int entity_index( )                                    = 0;
		virtual void receive_message( int class_id, bf_read& message ) = 0;
		virtual void* get_data_table_base_pointer( )                   = 0;
		virtual void set_destroyed_on_recreate_entities( )             = 0;
	};
} // namespace sdk
