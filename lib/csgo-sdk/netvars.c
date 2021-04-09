#include "netvars.h"
#include "sdk.h"

#include "include/utils.h"

typedef struct {
	sds name;
	ptrdiff_t offset;
} netvar_offset;

hashmap* netvars_offsets_map = NULL;

static int netvar_offset_compare(const void *a, const void *b, void *udata) {
    const netvar_offset* lhs = a;
    const netvar_offset* rhs = b;
    return sdscmp(lhs->name, rhs->name);
}

static bool netvar_offset_free(const void *item, void *udata) {
    const netvar_offset* entry = item;
	sdsfree(entry->name);
    return true;
}

static uint64_t netvar_offset_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const netvar_offset* entry = item;
    return hashmap_sip(entry->name, sdslen(entry->name), seed0, seed1);
}

static void netvars_store_table( const char* table_name, recv_table* table, ptrdiff_t offset ) {
	for ( int i = 0; i < table->num_props; i++ ) {
		const recv_prop* prop = (recv_prop*)((uintptr_t)table->props + i);
		const recv_table* child = prop->data_table;

		if ( child && child->num_props > 0 )
			netvars_store_table( table_name, child, prop->offset + offset );

		sds entry_name = sdscat(sdscat(sdsnew(table_name), "->"), prop->var_name);

		/* add netvar to list if it doesnt exist already */
		if ( !hashmap_get(netvars_offsets_map, &(netvar_offset){ entry_name } ) )
			hashmap_set(netvars_offsets_map, &(netvar_offset){ entry_name, prop->offset + offset });
		/* free string if it is a duplicate netvar */
		else
			sdsfree(entry_name);
	}
}

bool netvars_init ( ) {
	/* new hashmap containing all netvars */
	netvars_offsets_map = hashmap_new(sizeof(netvar_offset), 0, 0, 0, netvar_offset_hash, netvar_offset_compare, NULL);

	client_class* list = iclient_get_classes(cs_iclient);

	if ( !list )
		return false;

	for ( ; list != NULL; list = list->next )
		store_table ( list->recv_table->net_table_name, list->recv_table, 0 );

	return true;
}

void netvars_free ( ) {
	/* free all strings first */
	hashmap_scan(netvars_offsets_map, netvar_offset_free, NULL);
	/* then free the netvar map */
	hashmap_free ( netvars_offsets_map );
}

ptrdiff_t netvars_get_offset ( const char* name ) {
	const netvar_offset* offset = hashmap_get(netvars_offsets_map, &(netvar_offset){ name } );
	return offset ? offset->offset : 0;
}