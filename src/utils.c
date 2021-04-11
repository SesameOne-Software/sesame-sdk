#include "include/utils.h"

#include <windows.h>
#include <psapi.h>

#define PATTERN_IN_RANGE( x, a, b ) ( x >= a && x <= b )
#define PATTERN_GET_BITS( x ) ( PATTERN_IN_RANGE( ( x & ( ~0x20 ) ), 'A', 'F' ) ? ( ( x & ( ~0x20 ) ) - 'A' + 0xA ) : ( PATTERN_IN_RANGE( x, '0', '9' ) ? x - '0' : 0 ) )
#define PATTERN_GET_BYTE( x ) ( PATTERN_GET_BITS( x[ 0 ] ) << 4 | PATTERN_GET_BITS( x[ 1 ] ) )

pattern pattern_search( const char* mod, const char* pat ) {
	const uintptr_t range_start = ( uintptr_t )GetModuleHandleA( mod );
	const char* backup_pat = pat;

	MODULEINFO mi;
	K32GetModuleInformation( GetCurrentProcess( ), ( HMODULE )range_start, &mi, sizeof( MODULEINFO ) );

	uintptr_t first_match = 0;

	for ( uintptr_t current_address = range_start; current_address < range_start + mi.SizeOfImage; current_address++ ) {
		if ( !*pat ) {
			return first_match;
		}

		if ( *( uint8_t* )pat == '\?' || *( uint8_t* )current_address == PATTERN_GET_BYTE( pat ) ) {
			if ( !first_match )
				first_match = current_address;

			if ( !pat[ 2 ] ) {
				return first_match;
			}

			if ( *( uint16_t* )pat == '\?\?' || *( uint8_t* )pat != '\?' )
				pat += 3;
			else
				pat += 2;
		}
		else {
			pat = backup_pat;
			first_match = 0;
		}
	}

	return 0;
}