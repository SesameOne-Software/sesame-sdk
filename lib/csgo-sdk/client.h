#ifndef SDK_CLIENT_H
#define SDK_CLIENT_H

#include "utils.h"

typedef struct recv_prop recv_prop;
typedef struct recv_table recv_table;
typedef struct client_class client_class;

struct recv_table {
	recv_prop* props;
	int num_props;
	void* decoder;
	const char* net_table_name;
	PAD( 2 );
};

struct recv_prop {
	const char* var_name;
	int recv_type;
	int flags;
	int string_buffer_size;
	bool inside_array;
	const void* extra_data;
	recv_prop* array_prop;
	void* array_length_proxy;
	void* proxy_fn;
	void* data_table_proxy_fn;
	recv_table* data_table;
	int offset;
	int element_stride;
	int num_elements;
	const char* parent_array_prop_name;
};

struct client_class {
	__attribute__((cdecl)) void* ( *create_fn )( int, int );
	__attribute__((cdecl)) void* ( *create_event_fn )( );
	const char* network_name;
	recv_table* recv_table;
	client_class* next;
	int class_id;
};

typedef struct iclient iclient;

VIRTUAL( iclient, client_class*, get_classes, cs_idx_iclient_get_classes, ( this ) );

#endif // !SDK_CLIENT_H
