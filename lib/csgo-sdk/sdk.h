#ifndef SDK_H
#define SDK_H

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "include/utils.h"

/* sdk classes */
#include "beams.h"
#include "client.h"
#include "crc32.h"
#include "cvar.h"
#include "engine.h"
#include "entity.h"
#include "entlist.h"
#include "events.h"
#include "globals.h"
#include "input.h"
#include "mat3x4.h"
#include "matsys.h"
#include "mdlcache.h"
#include "mdlinfo.h"
#include "mdlrender.h"
#include "memalloc.h"
#include "netvars.h"
#include "physics.h"
#include "player.h"
#include "pred.h"
#include "quat.h"
#include "renderview.h"
#include "surface.h"
#include "trace.h"
#include "usercmd.h"
#include "vec3.h"
#include "weapon.h"

extern isurface* cs_isurface;
extern ipanel* cs_ipanel;
extern iengine* cs_iengine;
extern iclient* cs_iclient;
extern ientlist* cs_ientlist;
extern imatsys* cs_imatsys;
extern imdlinfo* cs_imdlinfo;
extern imdlrender* cs_imdlrender;
extern irenderview* cs_irenderview;
extern iglobals* cs_iglobals;
extern iphys* cs_iphys;
extern itrace* cs_itrace;
extern iclientstate* cs_iclientstate;
extern imemalloc* cs_imemalloc;
extern ipred* cs_ipred;
extern imovehelper* cs_imovehelper;
extern imove* cs_imove;
extern imdlcache* cs_imdlcache;
extern iinput* cs_iinput;
extern icvar* cs_icvar;
extern ievents* cs_ievents;
extern ibeams* cs_ibeams;
extern IDirect3DDevice9* cs_id3ddev;
extern player** cs_local_ptr;

static inline player* cs_get_local( ) {
	return *cs_local_ptr;
}

static inline void* cs_create_interface( const char* module, const char* name ) {
	void* ( *create_interface_export )( const char*, int ) = ( void* ( * )( const char*, int ) )GetProcAddress( GetModuleHandleA( module ), "CreateInterface" );
	return create_interface_export( name, 0 );
}

static inline int cs_time2ticks( float t ) {
	return ( int )( t / cs_iglobals->tick_interval + 0.5f );
}

static inline float cs_ticks2time( int t ) {
	return ( float )t * cs_iglobals->tick_interval;
}

#define cs_for_each_player() \
for ( int idx = 1; idx <= cs_iglobals->max_clients; idx++ )

static inline void cs_rotate_movement( usercmd* cmd, float backup_side_move, float backup_forward_move, const vec3* old_angles ) {
	float dv = 0.0f;
	float f1 = 0.0f;
	float f2 = 0.0f;
	if ( old_angles->y < 0.0f )
		f1 = 360.0f + old_angles->y;
	else
		f1 = old_angles->y;
	if ( cmd->angles.y < 0.0f )
		f2 = 360.0f + cmd->angles.y;
	else
		f2 = cmd->angles.y;
	if ( f2 < f1 )
		dv = abs( f2 - f1 );
	else
		dv = 360.0f - abs( f1 - f2 );
	dv = 360.0f - dv;
	cmd->forward_move = cosf( cs_deg2rad( dv ) ) * backup_forward_move + cosf( cs_deg2rad( dv + 90.0f ) ) * backup_side_move;
	cmd->side_move = sinf( cs_deg2rad( dv ) ) * backup_forward_move + sinf( cs_deg2rad( dv + 90.0f ) ) * backup_side_move;
}

static inline mat3x4* cs_angle_matrix( const vec3* angles, mat3x4* mat ) {
	float sr, sp, sy, cr, cp, cy;

	cs_sincos( cs_deg2rad( angles->y ), &sy, &cy );
	cs_sincos( cs_deg2rad( angles->x ), &sp, &cp );
	cs_sincos( cs_deg2rad( angles->z ), &sr, &cr );

	const float crcy = cr * cy;
	const float crsy = cr * sy;
	const float srcy = sr * cy;
	const float srsy = sr * sy;

	*mat3x4_at( mat, 0, 0 ) = cp * cy;
	*mat3x4_at( mat, 1, 0 ) = cp * sy;
	*mat3x4_at( mat, 2, 0 ) = -sp;
	*mat3x4_at( mat, 0, 1 ) = sp * srcy - crsy;
	*mat3x4_at( mat, 1, 1 ) = sp * srsy + crcy;
	*mat3x4_at( mat, 2, 1 ) = sr * cp;
	*mat3x4_at( mat, 0, 2 ) = ( sp * crcy + srsy );
	*mat3x4_at( mat, 1, 2 ) = ( sp * crsy - srcy );
	*mat3x4_at( mat, 2, 2 ) = cr * cp;
	*mat3x4_at( mat, 0, 3 ) = 0.0f;
	*mat3x4_at( mat, 1, 3 ) = 0.0f;
	*mat3x4_at( mat, 2, 3 ) = 0.0f;

	return mat;
}

static inline void cs_angle_matrix_pos( const vec3* angles, const vec3* origin, mat3x4* mat ) {
	cs_angle_matrix( angles, mat );
	mat3x4_set_origin( mat, origin );
}

static inline void cs_vector_transform( const vec3* in, const mat3x4* mat, vec3* out ) {
	vec3_add( mat3x4_get_origin( mat, vec3_zero( out ) ),
		&( vec3 ) {
		vec3_dot( in, ( vec3* )mat3x4_at( mat, 0, 0 ) ),
			vec3_dot( in, ( vec3* )mat3x4_at( mat, 1, 0 ) ),
			vec3_dot( in, ( vec3* )mat3x4_at( mat, 2, 0 ) )
	} );
}

bool cs_is_valve_server( );
void cs_add_box_overlay( const vec3* origin, const vec3* mins, const vec3* maxs, const vec3* angles, int r, int g, int b, int a, float duration );

bool cs_world_to_screen( vec3* origin, vec3* screen );
bool cs_init( );
bool cs_free( );

#endif // !SDK_H