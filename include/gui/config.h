#ifndef GUI_CONFIG_H
#define GUI_CONFIG_H

#include "include/utils.h"

typedef struct {
	int key, mode;
} ses_keybind;

typedef struct {
	uint8_t r, g, b, a;
} ses_color;

typedef enum {
	ses_type_bool,
	ses_type_int,
	ses_type_float,
	ses_type_string,
	ses_type_color,
	ses_type_vec3,
	ses_type_keybind,
	ses_type_vector,
	ses_type_binary
} ses_type_names;

typedef struct {
	/* group element should be categorized in */
	sds group, subgroup;
	/* name of element */
	sds name;
	/* actual data (will either be the value if it fits inside this pointer variable or allocated elsewhere if larger) */
	void* data;
	/* size of type with variable length */
	size_t size;
	/* type of variable this is an entry for */
	ses_type_names type;
} ses_cfg_entry;

/* in header, it will be defined differently, with the job of adding the variable to the struct */
/* in the implementation, we will use the same macro name, but redefine it beforehand so we can use the exact same syntax when adding an entry */
/* this makes everything easier! */
#define ses_cfg_add(group, subgroup, name, data, type, size)\
ses_cfg_entry group##_##subgroup##_##name;

typedef struct {
	ses_cfg_add ( gui, state, menu_pos, { 200.0f, 200.0f, 0.0f }, vec3, 0 );
	ses_cfg_add ( gui, state, test_int, 0, int, 0 );
	ses_cfg_add ( gui, state, test_float, 0.0f, float, 0 );
	ses_cfg_add ( gui, state, test_bool, false, bool, 0 );
} ses_cfg_data;

extern ses_cfg_data ses_cfg;

/* gets specific item from config */
#define ses_cfg_get_item (cfg, group, subgroup, name, type) *(type*)(&cfg.group##_##subgroup##_##name.data)
/* gets size of variable-size item from config */
#define ses_cfg_get_item_len(cfg, group, subgroup, name) ((cfg)->group##_##subgroup##_##name.size)
/* get amount of entries in config struct */
#define ses_cfg_get_len(cfg) (sizeof ( *(cfg) ) / sizeof ( (cfg)[0] ))

bool ses_cfg_save ( ses_cfg_data* cfg, sds file_name );
bool ses_cfg_load ( ses_cfg_data* cfg, sds file_name );
bool ses_cfg_new ( ses_cfg_data* cfg );
bool ses_cfg_free ( ses_cfg_data* cfg );

#endif // !GUI_CONFIG_H