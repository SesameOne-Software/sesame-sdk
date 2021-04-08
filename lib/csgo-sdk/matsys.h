#ifndef SDK_MATSYS_H
#define SDK_MATSYS_H

#include "include/utils.h"

typedef uint32_t mat_handle;

typedef struct imatsys imatsys;
typedef struct matvar matvar;
typedef struct mat mat;
typedef struct key_values key_values;

VIRTUAL ( mat, const char*, get_name, 0, ( this, NULL ) );
VIRTUAL ( mat, const char*, get_texture_group_name, 1, ( this, NULL ) );
VIRTUAL ( mat, matvar*, find_var, 11, ( this, NULL, var, found, complain ), const char* var, bool* found, bool complain );
VIRTUAL ( mat, void, increment_ref_count, 12, ( this, NULL ) );
VIRTUAL ( mat, void, alpha_modulate, 27, ( this, NULL, a ), float a );
VIRTUAL ( mat, void, color_modulate, 28, ( this, NULL, r, g, b ), float r, float g, float b );
VIRTUAL ( mat, void, set_mat_var_flag, 29, ( this, NULL, flag, state ), uint32_t flag, bool state );
VIRTUAL ( mat, bool, is_error_material, 42, ( this, NULL ) );

VIRTUAL ( imatsys, mat*, create_mat, 83, ( this, NULL, name, kv ), const char* name, key_values* kv );
VIRTUAL ( imatsys, mat*, find_mat, 84, ( this, NULL, group_name, complain, complain_prefix ), const char* group_name, bool complain, const char* complain_prefix );
VIRTUAL ( imatsys, mat_handle, get_first_mat, 86, ( this , NULL) );
VIRTUAL ( imatsys, mat_handle, get_next_mat, 87, ( this, NULL, handle ), mat_handle handle );
VIRTUAL ( imatsys, mat_handle, get_invalid_mat, 88, ( this, NULL ) );
VIRTUAL ( imatsys, mat*, get_mat_from_handle, 89, ( this, NULL, handle ), mat_handle handle );

#endif // !SDK_MATSYS_H