#include "netvars.h"

typedef struct {
	const char* name;
	ptrdiff_t offset;
} netvar_offset;

typedef struct {
	const char* table;
	netvar_offset* vars;
} netvar_table;

hashmap* netvars_offsets_map = NULL;

bool netvars_init ( ) {
	if ( !cs::i::client ) {
		return false;
	}

	auto list = cs::i::client->get_all_classes ( );

	if ( !list ) {

		return false;
	}

	for ( ; list != nullptr; list = list->next )
		store_table ( list->recv_table->net_table_name, list->recv_table );

	return true;
}

void netvars_free ( ) {
	hashmap_free ( netvars_offsets_map );
}

std::unordered_map< const char*, std::unordered_map< std::string, netvars::netvar_data_t > > netvars::offsets;

std::vector< std::string > split( std::string to_split, std::string delimeter ) {
	std::vector< std::string > split;
	int start = 0;
	int end = 0;

	while ( ( end = to_split.find( delimeter, start ) ) < to_split.size( ) ) {
		std::string val = to_split.substr( start, end - start );
		split.push_back( val );
		start = end + delimeter.size( );
	}

	if ( start < to_split.size( ) ) {
		std::string val = to_split.substr( start );
		split.push_back( val );
	}

	return split;
}

void netvars_store_table( const std::string& name, recv_table_t* table, std::size_t offset ) {
	for ( int i { }; i < table->num_props; ++i ) {
		auto prop = &table->props [ i ];
		auto child = prop->data_table;

		// we have a child table, that contains props.
		if ( child && child->num_props > 0 )
			store_table( name, child, prop->offset + offset );

		// insert if not present.
		if ( !offsets [ name ][ prop->var_name ].offset ) {
			offsets [ name ][ prop->var_name ].datamap_var = false;
			offsets [ name ][ prop->var_name ].prop_ptr = prop;
			offsets [ name ][ prop->var_name ].offset = static_cast< size_t >( prop->offset + offset );
		}
	}
}

// get netvar offset.
int netvars_get( const std::string& table, const std::string& prop ) {
	return offsets [ table ][ prop ].offset;
}

int netvars_get_offset( const char* name ) {
	const auto items = split( name, "->" );
	return offsets [ items.front( ) ][ items.back( ) ].offset;
}