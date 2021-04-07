#ifndef SDK_CLIENT_H
#define SDK_CLIENT_H

#include "include/utils.h"

typedef struct recv_table recv_table;
typedef struct recv_prop recv_prop;

typedef struct {
	recv_prop* props;
	int num_props;
	void* decoder;
	const char* net_table_name;
	PAD( 2 );
} recv_table;

typedef struct {
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
} recv_prop;

typedef struct {
	void* ( *create_fn )( int, int );
	void* ( *create_event_fn )( );
	const char* network_name;
	recv_table* recv_table;
	client_class* next;
	int class_id;
} client_class;

typedef struct iclient iclient;

VIRTUAL ( iclient, client_class*, get_classes, 8, ( this ) );

#endif // !SDK_CLIENT_H