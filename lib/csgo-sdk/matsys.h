#ifndef SDK_MATSYS_H
#define SDK_MATSYS_H

#include "include/utils.h"

typedef uint32_t mat_handle;

typedef struct imatsys imatsys;
typedef struct matvar matvar;
typedef struct mat mat;
typedef struct key_values key_values;

VIRTUAL ( mat, const char*, get_name, cs_idx_mat_get_name, (  ) );
VIRTUAL ( mat, const char*, get_texture_group_name, cs_idx_mat_get_texture_group_name, (  ) );
VIRTUAL ( mat, matvar*, find_var, cs_idx_mat_find_var, (  var, found, complain ), const char* var, bool* found, bool complain );
VIRTUAL ( mat, void, increment_ref_count, cs_idx_mat_increment_ref_count, (  ) );
VIRTUAL ( mat, void, alpha_modulate, cs_idx_mat_alpha_modulate, (  a ), float a );
VIRTUAL ( mat, void, color_modulate, cs_idx_mat_color_modulate, (  r, g, b ), float r, float g, float b );
VIRTUAL ( mat, void, set_mat_var_flag, cs_idx_mat_set_mat_var_flag, (  flag, state ), uint32_t flag, bool state );
VIRTUAL ( mat, bool, is_error_material, cs_idx_mat_is_error_material, (  ) );

VIRTUAL ( imatsys, mat*, create_mat, cs_idx_imatsys_create_mat, (  name, kv ), const char* name, key_values* kv );
VIRTUAL ( imatsys, mat*, find_mat, cs_idx_imatsys_find_mat, (  group_name, complain, complain_prefix ), const char* group_name, bool complain, const char* complain_prefix );
VIRTUAL ( imatsys, mat_handle, get_first_mat, cs_idx_imatsys_get_first_mat, ( ) );
VIRTUAL ( imatsys, mat_handle, get_next_mat, cs_idx_imatsys_get_next_mat, (  handle ), mat_handle handle );
VIRTUAL ( imatsys, mat_handle, get_invalid_mat, cs_idx_imatsys_get_invalid_mat, (  ) );
VIRTUAL ( imatsys, mat*, get_mat_from_handle, cs_idx_imatsys_get_mat_from_handle, (  handle ), mat_handle handle );

#endif // !SDK_MATSYS_H