#ifndef SDK_WEAPON_H
#define SDK_WEAPON_H

#include "entity.h"

typedef enum {
	weapon_none = 0,
	weapon_deagle = 1,
	weapon_elite = 2,
	weapon_fiveseven = 3,
	weapon_glock = 4,
	weapon_ak47 = 7,
	weapon_aug = 8,
	weapon_awp = 9,
	weapon_famas = 10,
	weapon_g3sg1 = 11,
	weapon_galil = 13,
	weapon_m249 = 14,
	weapon_m4a4 = 16,
	weapon_mac10 = 17,
	weapon_p90 = 19,
	weapon_mp5sd = 23,
	weapon_ump45 = 24,
	weapon_xm1014 = 25,
	weapon_bizon = 26,
	weapon_mag7 = 27,
	weapon_negev = 28,
	weapon_sawedoff = 29,
	weapon_tec9 = 30,
	weapon_taser = 31,
	weapon_p2000 = 32,
	weapon_mp7 = 33,
	weapon_mp9 = 34,
	weapon_nova = 35,
	weapon_p250 = 36,
	weapon_scar20 = 38,
	weapon_sg553 = 39,
	weapon_ssg08 = 40,
	weapon_knife_ct = 42,
	weapon_flashbang = 43,
	weapon_hegrenade = 44,
	weapon_smoke = 45,
	weapon_molotov = 46,
	weapon_decoy = 47,
	weapon_firebomb = 48,
	weapon_c4 = 49,
	weapon_musickit = 58,
	weapon_knife_t = 59,
	weapon_m4a1s = 60,
	weapon_usps = 61,
	weapon_tradeup_contract = 62,
	weapon_cz75a = 63,
	weapon_revolver = 64,
	knife_bayonet = 500,
	knife_css = 503,
	knife_flip = 505,
	knife_gut = 506,
	knife_karambit = 507,
	knife_m9_bayonet = 508,
	knife_huntsman = 509,
	knife_falchion = 512,
	knife_bowie = 514,
	knife_butterfly = 515,
	knife_shadow_daggers = 516,
	knife_cord = 517,
	knife_canis = 518,
	knife_ursus = 519,
	knife_gypsy_jackknife = 520,
	knife_outdoor = 521,
	knife_stiletto = 522,
	knife_widowmaker = 523,
	knife_skeleton = 525,
	glove_studded_bloodhound = 5027,
	glove_t_side = 5028,
	glove_ct_side = 5029,
	glove_sporty = 5030,
	glove_slick = 5031,
	glove_leather_wrap = 5032,
	glove_motorcycle = 5033,
	glove_specialist = 5034,
	glove_studded_hydra = 5035
} weapon_idx;

typedef enum {
	weapontype_knife = 0,
	weapontype_pistol,
	weapontype_smg,
	weapontype_rifle,
	weapontype_shotgun,
	weapontype_sniper,
	weapontype_lmg,
	weapontype_c4,
	weapontype_grenade,
	weapontype_unknown
} weapontype;

// Generated using ReClass 2016
typedef struct {
	PAD( 20 );
	int max_clip;
	PAD( 12 );
	int max_reserved_ammo;
	PAD( 96 );
	const char* hud_name;
	const char* weapon_name;
	PAD( 56 );
	weapontype type;
	PAD( 4 );
	int price;
	int reward;
	PAD( 4 );
	float fire_rate;
	PAD( 12 );
	uint8_t full_auto;
	PAD( 3 );
	int dmg;
	float armor_ratio;
	int bullets;
	float penetration;
	PAD( 8 );
	float range;
	float range_modifier;
	float throw_velocity;
	PAD( 12 );
	bool has_silencer;
	PAD( 15 );
	float max_speed;
	float max_speed_alt;
	PAD( 76 );
	int recoil_seed;
	PAD( 32 );
} weapon_info;

typedef struct econitem econitem;

OFFSET( econitem, uint32_t, account_id, cs_offsets.econitem_account_id );
OFFSET( econitem, const char*, icon_name, cs_offsets.econitem_icon_name );
OFFSET( econitem, const char*, mdl_name, cs_offsets.econitem_mdl_name );
OFFSET( econitem, const char*, world_mdl_name, cs_offsets.econitem_world_mdl_name );
VIRTUAL( econitem, const char*, inventory_img, cs_idx_econitem_inventory_img, ( this ) );
OFFSET( econitem, uint64_t, item_id, cs_offsets.econitem_item_id );
OFFSET( econitem, uint64_t, original_id, cs_offsets.econitem_original_id );
OFFSET( econitem, uint16_t, item_definition_index, cs_offsets.econitem_item_definition_index );
OFFSET( econitem, uint32_t, inventory, cs_offsets.econitem_inventory );
OFFSET( econitem, uint8_t, flags, cs_offsets.econitem_flags );
OFFSET( econitem, int, equipped_position, cs_offsets.econitem_equipped_position );

/*
*	XREF: "Error Parsing PaintData in %s! \n" offset is in same function call
*	https://github.com/perilouswithadollarsign/cstrike15_src/blob/29e4c1fda9698d5cebcdaf1a0de4b829fa149bf8/game/shared/cstrike15/cstrike15_item_schema.cpp#L188
*/
OFFSET( econitem, const char*, definition_name, cs_offsets.econitem_definition_name );

static inline econitem* econitem_static_data( econitem* this ) {
	return ( ( __attribute__( ( thiscall ) ) econitem * ( * )( econitem* ) )cs_offsets.econitem_static_data_fn ) ( this );
}

static inline void econitem_update_equipped_state( econitem* this, bool state ) {
	( ( __attribute__( ( thiscall ) ) int( * )( econitem*, uint32_t ) )cs_offsets.econitem_update_equipped_state_fn ) ( this, state );
}

/* XREF: "resource/flash/econ/weapons/cached/*.iic" string is inside the function */
static inline void econitem_clean_inventory_image_cache_dir( econitem* this ) {
	( ( __attribute__( ( thiscall ) ) void( * )( econitem* ) )cs_offsets.econitem_clean_inventory_image_cache_dir_fn ) ( this );
}

static inline void econitem_set_or_add_attribute_by_name( econitem* this, float val, const char* attribute_name ) {
	asm volatile(
		"mov ecx, %0;"
		"movss xmm2, %1;"
		"push %2;"
		"call %3;"
		:: "g"( this ), "g"( val ), "g"( attribute_name ), "g"( cs_offsets.econitem_set_or_add_attribute_by_name_fn )
		: "ecx", "xmm2"
		);
}

static inline void econitem_set_custom_name( econitem* this, const char* name ) {
	( ( __attribute__( ( thiscall ) ) econitem * ( * )( econitem*, const char* ) )cs_offsets.econitem_set_custom_name_fn ) ( this, name );
}

static inline void econitem_set_custom_desc( econitem* this, const char* name ) {
	( ( __attribute__( ( thiscall ) ) econitem * ( * )( econitem*, const char* ) )cs_offsets.econitem_set_custom_desc_fn ) ( this, name );
}

static inline void econitem_set_attributei( econitem* this, int index, int val ) {
	uint32_t* v15 = ( uint32_t* )( ( ( __attribute__( ( cdecl ) ) void* ( * )( ) )cs_offsets.econitem_get_item_schema_fn ) ( ) );
	uint32_t v16 = *( uint32_t* )( v15[ 72 ] + 4 * index );

	( ( __attribute__( ( thiscall ) ) int( * )( econitem*, uint32_t, void* ) )cs_offsets.econitem_set_dynamic_attribute_val_fn ) ( this, v16, &val );
}

static inline void econitem_set_attributef( econitem* this, int index, float val ) {
	econitem_set_attributei( this, index, *( int* )&val );
}

static inline econitem* econitem_soc_data( econitem* this ) {
	return ( ( __attribute__( ( thiscall ) ) econitem * ( * )( econitem* ) )cs_offsets.econitem_soc_data_fn ) ( this );
}

static inline uint16_t* econitem_item_data( econitem* this ) {
	return ( uint16_t* )( ( uintptr_t )this + 0x26 );
}

static inline void econitem_set_origin( econitem* this, int origin ) {
	const uint16_t data = *econitem_item_data( this );
	*econitem_item_data( this ) = data ^ ( ( uint8_t )data ^ ( uint8_t )origin ) & 0x1F;
}

static inline void econitem_set_level( econitem* this, int level ) {
	const uint16_t data = *econitem_item_data( this );
	*econitem_item_data( this ) = data ^ ( data ^ ( level << 9 ) ) & 0x600;
}

static inline void econitem_set_in_use( econitem* this, bool in_use ) {
	const uint16_t data = *econitem_item_data( this );
	*econitem_item_data( this ) = data & 0x7FFF | ( in_use << 15 );
}

static inline void econitem_set_rarity( econitem* this, int rarity ) {
	const uint16_t data = *econitem_item_data( this );
	*econitem_item_data( this ) = ( data ^ ( rarity << 11 ) ) & 0x7800 ^ data;
}

static inline void econitem_set_quality( econitem* this, int quality ) {
	const uint16_t data = *econitem_item_data( this );
	*econitem_item_data( this ) = data ^ ( data ^ 32 * quality ) & 0x1E0;
}

static inline void econitem_add_sticker( econitem* this, int index, int kit, float wear, float scale, float rotation ) {
	econitem_set_attributef( this, 113 + 4 * index, kit );
	econitem_set_attributef( this, 114 + 4 * index, wear );
	econitem_set_attributef( this, 115 + 4 * index, scale );
	econitem_set_attributef( this, 116 + 4 * index, rotation );
}

static inline void econitem_set_stattrak( econitem* this, int val ) {
	econitem_set_attributei( this, 80, val );
	econitem_set_attributei( this, 81, 0 );
	econitem_set_quality( this, 9 );
}

static inline void econitem_set_paintkit( econitem* this, float kit ) {
	econitem_set_attributef( this, 6, kit );
}

static inline void econitem_set_paint_seed( econitem* this, float seed ) {
	econitem_set_attributef( this, 7, seed );
}

static inline void econitem_set_paint_wear( econitem* this, float wear ) {
	econitem_set_attributef( this, 8, wear );
}

typedef struct weapon weapon;

NETVAR( weapon, uint16_t, item_def_idx, "DT_BaseAttributableItem->m_iItemDefinitionIndex" );
NETVAR( weapon, int, mdl_idx, "DT_BaseAttributableItem->m_nModelIndex" );
NETVAR( weapon, float, next_primary_attack, "DT_BaseCombatWeapon->m_flNextPrimaryAttack" );
NETVAR( weapon, float, next_secondary_attack, "DT_BaseCombatWeapon->m_flNextSecondaryAttack" );
NETVAR( weapon, int, ammo, "DT_BaseCombatWeapon->m_iClip1" );
NETVAR( weapon, int, ammo2, "DT_BaseCombatWeapon->m_iClip2" );
NETVAR( weapon, float, postpone_fire_time, "DT_BaseCombatWeapon->m_flPostponeFireReadyTime" );
NETVAR( weapon, uint32_t, world_mdl_handle, "DT_BaseCombatWeapon->m_hWeaponWorldModel" );
NETVAR( weapon, float, throw_time, "DT_BaseCSGrenade->m_fThrowTime" );
NETVAR( weapon, bool, pin_pulled, "DT_BaseCSGrenade->m_bPinPulled" );
NETVAR( weapon, float, throw_strength, "DT_BaseCSGrenade->m_flThrowStrength" );
NETVAR( weapon, uint64_t, original_owner_xuid, "DT_BaseAttributableItem->m_OriginalOwnerXuidLow" );
NETVAR( weapon, uint32_t, original_owner_xuid_lo, "DT_BaseAttributableItem->m_OriginalOwnerXuidLow" );
NETVAR( weapon, uint32_t, original_owner_xuid_hi, "DT_BaseAttributableItem->m_OriginalOwnerXuidHigh" );
NETVAR( weapon, uint32_t, fallback_stattrak, "DT_BaseAttributableItem->m_nFallbackStatTrak" );
NETVAR( weapon, uint32_t, fallback_paintkit, "DT_BaseAttributableItem->m_nFallbackPaintKit" );
NETVAR( weapon, uint32_t, fallback_seed, "DT_BaseAttributableItem->m_nFallbackSeed" );
NETVAR( weapon, float, fallback_wear, "DT_BaseAttributableItem->m_flFallbackWear" );
NETVAR( weapon, uint32_t, fallback_quality, "DT_BaseAttributableItem->m_iEntityQuality" );
NETVAR( weapon, uint32_t, item_id_low, "DT_BaseAttributableItem->m_iItemIDLow" );
NETVAR( weapon, uint32_t, item_id_high, "DT_BaseAttributableItem->m_iItemIDHigh" );
NETVAR( weapon, uint32_t, account, "DT_BaseAttributableItem->m_iAccountID" );
NETVAR( weapon, const char*, name, "DT_BaseAttributableItem->m_szCustomName" );
NETVAR( weapon, float, last_shot_time, "DT_WeaponCSBase->m_fLastShotTime" );
NETVAR( weapon, int, sequence, "DT_BaseViewModel->m_nSequence" );
NETVAR( weapon, bool, initialized, "DT_BaseAttributableItem->m_bInitialized" );
NETVAR( weapon, int, zoom_level, "DT_WeaponCSBaseGun->m_zoomLevel" );
NETVAR( weapon, float, accuracy_penalty, "DT_WeaponCSBase->m_fAccuracyPenalty" );
NETVAR( weapon, float, recoil_index, "DT_WeaponCSBase->m_flRecoilIndex" );
NETVAR( weapon, float, cycle, "DT_BaseAnimating->m_flCycle" );
NETVAR( weapon, float, anim_time, "DT_BaseEntity->m_flAnimTime" );
NETVAR( weapon, econitem, econ_item, "DT_BaseAttributableItem->m_Item" );

VIRTUAL( weapon, void, update_accuracy, cs_idx_weapon_update_accuracy, ( this ) );
VIRTUAL( weapon, float, innacuracy, cs_idx_weapon_innacuracy, ( this ) );
VIRTUAL( weapon, float, spread, cs_idx_weapon_spread, ( this ) );
VIRTUAL( weapon, float, max_speed, cs_idx_weapon_max_speed, ( this ) );

static inline weapon_info* weapon_data( weapon* this ) {
	return ( ( __attribute__( ( thiscall ) ) weapon_info * ( * )( void*, uint16_t ) )utils_vfunc( ( void* )cs_offsets.weapon_sys, cs_idx_weaponsys_data ) )( ( void* )cs_offsets.weapon_sys, weapon_item_def_idx( this ) );
}

static inline weapon* weapon_world_mdl( weapon* this ) {
	extern struct ientlist* cs_ientlist;
	return ientlist_get_entity_from_handle( cs_ientlist, weapon_world_mdl_handle( this ) );
}

#endif // !SDK_WEAPON_H