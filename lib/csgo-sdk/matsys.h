#ifndef SDK_MATSYS_H
#define SDK_MATSYS_H

#include "utils.h"

typedef uint32_t mat_handle;

typedef struct imatsys imatsys;
typedef struct matvar matvar;
typedef struct mat mat;
typedef struct key_values key_values;

VIRTUAL( mat, const char*, get_name, cs_idx_mat_get_name, ( this ) );
VIRTUAL( mat, const char*, get_texture_group_name, cs_idx_mat_get_texture_group_name, ( this ) );
VIRTUAL( mat, matvar*, find_var, cs_idx_mat_find_var, ( this, var, found, complain ), const char* var, bool* found, bool complain );
VIRTUAL( mat, void, increment_ref_count, cs_idx_mat_increment_ref_count, ( this ) );
VIRTUAL( mat, void, alpha_modulate, cs_idx_mat_alpha_modulate, ( this, a ), float a );
VIRTUAL( mat, void, color_modulate, cs_idx_mat_color_modulate, ( this, r, g, b ), float r, float g, float b );
VIRTUAL( mat, void, set_mat_var_flag, cs_idx_mat_set_mat_var_flag, ( this, flag, state ), uint32_t flag, bool state );
VIRTUAL( mat, bool, is_error_material, cs_idx_mat_is_error_material, ( this ) );

VIRTUAL( imatsys, mat*, create_mat, cs_idx_imatsys_create_mat, ( this, name, kv ), const char* name, key_values* kv );
VIRTUAL( imatsys, mat*, find_mat, cs_idx_imatsys_find_mat, ( this, group_name, complain, complain_prefix ), const char* group_name, bool complain, const char* complain_prefix );
VIRTUAL( imatsys, mat_handle, get_first_mat, cs_idx_imatsys_get_first_mat, ( this ) );
VIRTUAL( imatsys, mat_handle, get_next_mat, cs_idx_imatsys_get_next_mat, ( this, handle ), mat_handle handle );
VIRTUAL( imatsys, mat_handle, get_invalid_mat, cs_idx_imatsys_get_invalid_mat, ( this ) );
VIRTUAL( imatsys, mat*, get_mat_from_handle, cs_idx_imatsys_get_mat_from_handle, ( this, handle ), mat_handle handle );

#endif // !SDK_MATSYS_H