#ifndef SDK_NETVARS_H
#define SDK_NETVARS_H

#include "utils.h"

#define NETVAR( _struct, t, func, name ) \
static inline t* COMBINE( COMBINE(_struct, _), func )( _struct* this ) { \
	static ptrdiff_t offset = 0; \
\
	if ( !offset )\
		offset = netvars_get_offset ( name );\
\
	return ( t* ) ( (uintptr_t) this + offset ); \
}

#define OFFSET( _struct, t, func, offset ) \
static inline t* COMBINE( COMBINE(_struct, _), func )( _struct* this ) { \
	return ( t* ) ( (uintptr_t) this + offset ); \
}

bool netvars_init( );
bool netvars_free( );
ptrdiff_t netvars_get_offset( const char* name );

#endif // !SDK_NETVARS_H