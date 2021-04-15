#include "include/gui/config.h"

#include "lib/csgo-sdk/sdk.h"

ses_cfg_data ses_cfg;

#undef ses_cfg_add
#define ses_cfg_add(group, subgroup, name, data, type, size)\
const type tmp_##group##_##subgroup##_##name = data;\
void* tmp_ptr_##group##_##subgroup##_##name = (void*)&tmp_##group##_##subgroup##_##name;\
if (ses_type_##type >= ses_type_vec3) {\
	if(ses_type_##type >= ses_type_vector)\
		tmp_ptr_##group##_##subgroup##_##name = memcpy ( malloc ( size ), *(void**)tmp_ptr_##group##_##subgroup##_##name, size ); \
	else\
		tmp_ptr_##group##_##subgroup##_##name = memcpy ( malloc ( sizeof ( type ) ), tmp_ptr_##group##_##subgroup##_##name, sizeof ( type ) ); \
}\
cfg->group##_##subgroup##_##name = ( ses_cfg_entry ) { sdsnew(#group), sdsnew(#subgroup), sdsnew(#name), *(void**)tmp_ptr_##group##_##subgroup##_##name, size, ses_type_##type };

bool ses_cfg_save ( ses_cfg_data* cfg, sds file_name ) {
	//VM_TIGER_WHITE_START;
	STR_ENCRYPT_START;

	if ( !cfg || !ses_cfg_get_len ( cfg ) )
		return false;

	iengine_execute_cmd ( cs_iengine, "clear" );

	yyjson_mut_doc* doc = yyjson_mut_doc_new ( NULL );
	yyjson_mut_val* root = yyjson_mut_obj ( doc );
	
	yyjson_mut_doc_set_root ( doc, root );

	for ( size_t i = 0; i < ses_cfg_get_len ( cfg ); i++ ) {
		ses_cfg_entry* entry = ( ses_cfg_entry* ) cfg + i;
		
		/* create group object if doesnt exist already */
		yyjson_mut_val* group = yyjson_mut_obj_get ( root, entry->group );

		if ( !group )
			yyjson_mut_obj_add ( root, yyjson_mut_str ( doc, entry->group ), group = yyjson_mut_obj ( doc ) );

		/* create subgroup object if doesnt exist already */
		yyjson_mut_val* subgroup = yyjson_mut_obj_get ( group, entry->subgroup );

		if ( !subgroup )
			yyjson_mut_obj_add ( group, yyjson_mut_str ( doc, entry->subgroup ), subgroup = yyjson_mut_obj ( doc ) );

		switch ( entry->type ) {
		case ses_type_bool:
			yyjson_mut_obj_add_bool ( doc, subgroup, entry->name, *( bool* ) &entry->data );
			break;
		case ses_type_int:
			yyjson_mut_obj_add_int ( doc, subgroup, entry->name, *( int* ) &entry->data );
			break;
		case ses_type_float:
			yyjson_mut_obj_add_real ( doc, subgroup, entry->name, ( double ) *( float* ) &entry->data );
			break;
		case ses_type_sds:
			yyjson_mut_obj_add_str ( doc, subgroup, entry->name, ( char* ) entry->data );
			break;
		case ses_type_color:
			yyjson_mut_obj_add ( subgroup, yyjson_mut_str ( doc, entry->name ), yyjson_mut_arr_with_uint8 ( doc, ( uint8_t* ) &entry->data, 4 ) );
			break;
		case ses_type_vec3: {
			double as_dbl []= { ( double ) ( ( float* ) entry->data ) [ 0 ], ( double ) ( ( float* ) entry->data ) [ 1 ], ( double ) ( ( float* ) entry->data ) [ 2 ] };
			yyjson_mut_obj_add ( subgroup, yyjson_mut_str ( doc, entry->name ), yyjson_mut_arr_with_real ( doc, as_dbl, 3 ) );
		} break;
		case ses_type_keybind:
			yyjson_mut_obj_add ( subgroup, yyjson_mut_str ( doc, entry->name ), yyjson_mut_arr_with_sint32 ( doc, ( int* ) entry->data, 2 ) );
			break;
		case ses_type_vector:
		case ses_type_binary:
			yyjson_mut_obj_add ( subgroup, yyjson_mut_str ( doc, entry->name ), yyjson_mut_arr_with_uint8 ( doc, ( uint8_t* ) entry->data, entry->size ) );
			break;
		default:
			break;
		}
	}

	bool fail = false;

	yyjson_write_err err;
	if ( !yyjson_mut_write_file ( file_name, doc, YYJSON_WRITE_PRETTY | YYJSON_WRITE_ESCAPE_UNICODE, NULL, &err ) ) {
		utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdsnew ( "Failed to save config.\n" ) );
		fail = true;
	}

	yyjson_mut_doc_free ( doc );
	sdsfree ( file_name );

	if ( fail )
		return false;

	utils_print_console ( &( uint8_t [ ] ) { 0, 255, 0, 255 }, sdsnew ( "Saved config.\n" ) );

	return true;

	STR_ENCRYPT_END;
	//VM_TIGER_WHITE_END;
}

bool ses_cfg_load ( ses_cfg_data* cfg, sds file_name ) {
	//VM_TIGER_WHITE_START;
	STR_ENCRYPT_START;
	if ( !cfg || !ses_cfg_get_len ( cfg ) )
		return false;

	iengine_execute_cmd ( cs_iengine, "clear" );

	yyjson_read_err err;
	yyjson_read_flag flg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
	yyjson_doc* doc = yyjson_read_file ( file_name, flg, NULL, &err );
	sdsfree ( file_name );
	
	if ( doc ) {
		yyjson_val* root = yyjson_doc_get_root ( doc );

		for ( size_t i = 0; i < ses_cfg_get_len ( cfg ); i++ ) {
			ses_cfg_entry* entry = ( ses_cfg_entry* ) cfg + i;

			yyjson_val* group = yyjson_obj_get ( root, entry->group );
			yyjson_val* subgroup = yyjson_obj_get ( group, entry->subgroup );
			yyjson_val* var = yyjson_obj_get ( subgroup, entry->name );

			switch ( entry->type ) {
			case ses_type_bool:
				if ( !yyjson_is_bool ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}
				*( bool* ) &entry->data = yyjson_get_bool ( var );
				break;
			case ses_type_int:
				if ( !yyjson_is_int ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}
				*( int* ) &entry->data = yyjson_get_int ( var );
				break;
			case ses_type_float:
				if ( !yyjson_is_real ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}
				*( float* ) &entry->data = ( float ) yyjson_get_real ( var );
				break;
			case ses_type_sds:
				if ( !yyjson_is_str ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}
				sdsfree ( ( sds ) entry->data );
				entry->data = ( void* ) sdsnew ( yyjson_get_str ( var ) );
				break;
			case ses_type_color: {
				if ( !yyjson_is_arr ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}

				yyjson_val* r = yyjson_arr_get ( var, 0 );
				yyjson_val* g = yyjson_arr_get ( var, 1 );
				yyjson_val* b = yyjson_arr_get ( var, 2 );
				yyjson_val* a = yyjson_arr_get ( var, 3 );

				if ( !yyjson_is_int ( r ) || !yyjson_is_int ( g ) || !yyjson_is_int ( b ) || !yyjson_is_int ( a ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}

				*( ses_color* ) &entry->data = ( ses_color ) { ( uint8_t ) yyjson_get_int ( r ), ( uint8_t ) yyjson_get_int ( g ), ( uint8_t ) yyjson_get_int ( b ),( uint8_t ) yyjson_get_int ( a ) };
			} break;
			case ses_type_vec3:{
				if ( !yyjson_is_arr ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}
				yyjson_val* x = yyjson_arr_get ( var, 0 );
				yyjson_val* y = yyjson_arr_get ( var, 1 );
				yyjson_val* z = yyjson_arr_get ( var, 2 );

				if ( !yyjson_is_real ( x ) || !yyjson_is_real ( y ) || !yyjson_is_real ( z ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}

				*( vec3* ) entry->data = ( vec3 ) { ( float ) yyjson_get_real ( x ), ( float ) yyjson_get_real ( y ),( float ) yyjson_get_real ( z ) };
			}break;
			case ses_type_keybind:{
				if ( !yyjson_is_arr ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}
				yyjson_val* key = yyjson_arr_get ( var, 0 );
				yyjson_val* mode = yyjson_arr_get ( var, 1 );

				if (!yyjson_is_int ( key ) || !yyjson_is_int ( mode ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}

				*( ses_keybind* ) entry->data = ( ses_keybind ) { yyjson_is_int ( key ), yyjson_is_int ( mode ) };
			}break;
			case ses_type_vector:
			case ses_type_binary: {
				if ( !yyjson_is_arr ( var ) ) {
					utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
					break;
				}
				
				if ( entry->type == ses_type_vector ) {
					vector_free ( ( vector ) entry->data );
					entry->data = vector_create ( yyjson_arr_size ( var ) );
				}
				else {
					free ( entry->data );
					entry->data = malloc ( yyjson_arr_size ( var ) );
				}

				for ( int i = 0; i < yyjson_arr_size ( var ) ; i++ ) {
					yyjson_val* iter = yyjson_arr_get ( var, i );

					if ( !yyjson_is_uint ( iter ) ) {
						utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt ( sdsempty ( ), "Failed to read \"%s.%s.%s\".\n", entry->group, entry->subgroup, entry->name ) );
						break;
					}
				}
			} break;
			default:
				break;
			}
		}

		yyjson_doc_free ( doc );

		utils_print_console ( &( uint8_t [ ] ) { 0, 255, 0, 255 }, sdsnew ( "Loaded config.\n" ) );
		return true;
	}
	else {
		yyjson_doc_free ( doc );
		utils_print_console ( &( uint8_t [ ] ) { 255, 0, 0, 255 }, sdscatfmt( sdsempty ( ), "Failed to read config at line %d.\n", (int)err.pos ) );
		return false;
	}

	return true;

	STR_ENCRYPT_END;
	//VM_TIGER_WHITE_END;
}

bool ses_cfg_new ( ses_cfg_data* cfg ) {
	if ( !cfg || !ses_cfg_get_len ( cfg ) )
		return false;

	CLEAR_START;
	//VM_SHARK_BLACK_START;
	STR_ENCRYPT_START;

	ses_cfg_set_defaults

#ifdef _DEBUG
	utils_print_console ( &( uint8_t [ ] ) { 255, 255, 0, 255 }, sdsnew ( "Allocated room for new config.\n" ) );
#endif

	STR_ENCRYPT_END;
	//VM_SHARK_BLACK_END;
	CLEAR_END;

	return true;
}

bool ses_cfg_free ( ses_cfg_data* cfg ) {
	CLEAR_START;
	//VM_SHARK_BLACK_START;
	STR_ENCRYPT_START;

	if ( !cfg || !ses_cfg_get_len ( cfg ) )
		return false;

#ifdef _DEBUG
	utils_print_console ( &( uint8_t [ ] ) { 255, 255, 0, 255 }, sdscatprintf (sdsnew ( "Number of config entries: %d.\n" ), ses_cfg_get_len ( cfg ) ) );
#endif
	
	/* handle freeing and resetting any memory we need to manage when deleting a config */
	for ( size_t i = 0; i < ses_cfg_get_len ( cfg ); i++ ) {
		ses_cfg_entry* entry = ( ses_cfg_entry* ) cfg + i;

		/* free strings and null them */
		if ( entry->group )
			sdsfree ( entry->group );

		if ( entry->subgroup )
			sdsfree ( entry->subgroup );

		if ( entry->name )
			sdsfree ( entry->name );

		/* free data if it was allocated */
		if ( entry->data ) {
			switch ( entry->type ) {
			case ses_type_sds:
				sdsfree ( ( sds ) entry->data );
				break;
			case ses_type_vector:
				vector_free ( ( vector ) entry->data );
				break;
			case ses_type_vec3:
			case ses_type_keybind:
			case ses_type_binary:
				free ( entry->data );
				break;
				/* other types do not need to be freed */
			default:
				break;
			}
		}
	}

	/* zero the memory in one pass */
	memset ( cfg, 0, sizeof(*cfg) );

#ifdef _DEBUG
	utils_print_console ( &( uint8_t [ ] ) { 255, 255, 0, 255 }, sdsnew ( "Freed config.\n" ) );
#endif

	return true;

	STR_ENCRYPT_END;
	//VM_SHARK_BLACK_END;
	CLEAR_END;
}