#ifndef SDK_MATSYS_H
#define SDK_MATSYS_H

#include "include/utils.h"

typedef uint32_t mat_handle;

typedef struct imatsys imatsys;
typedef struct matvar matvar;
typedef struct mat mat;
typedef struct key_values key_values;

VIRTUAL ( mat, const char*, get_name, 0, (  ) );
VIRTUAL ( mat, const char*, get_texture_group_name, 1, (  ) );
VIRTUAL ( mat, matvar*, find_var, 11, (  var, found, complain ), const char* var, bool* found, bool complain );
VIRTUAL ( mat, void, increment_ref_count, 12, (  ) );
VIRTUAL ( mat, void, alpha_modulate, 27, (  a ), float a );
VIRTUAL ( mat, void, color_modulate, 28, (  r, g, b ), float r, float g, float b );
VIRTUAL ( mat, void, set_mat_var_flag, 29, (  flag, state ), uint32_t flag, bool state );
VIRTUAL ( mat, bool, is_error_material, 42, (  ) );

VIRTUAL ( imatsys, mat*, create_mat, 83, (  name, kv ), const char* name, key_values* kv );
VIRTUAL ( imatsys, mat*, find_mat, 84, (  group_name, complain, complain_prefix ), const char* group_name, bool complain, const char* complain_prefix );
VIRTUAL ( imatsys, mat_handle, get_first_mat, 86, ( ) );
VIRTUAL ( imatsys, mat_handle, get_next_mat, 87, (  handle ), mat_handle handle );
VIRTUAL ( imatsys, mat_handle, get_invalid_mat, 88, (  ) );
VIRTUAL ( imatsys, mat*, get_mat_from_handle, 89, (  handle ), mat_handle handle );

#endif // !SDK_MATSYS_H