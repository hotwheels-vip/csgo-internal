#pragma once

struct recv_prop_t;

struct recv_table_t {
	recv_prop_t* m_props_ptr;
	int m_props;
	void* m_decoder;
	char* m_net_table_name;
	bool m_initialized;
	bool m_in_main_list;
};
