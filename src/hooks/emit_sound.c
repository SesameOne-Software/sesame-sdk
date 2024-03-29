#include "hooks/hooks.h"

/* modify game sounds emitted from entities */
__attribute__( ( thiscall ) ) int hooks_emit_sound( void* this, void* filter, int ent_idx, int chan, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const vec3* origin, const vec3* dir, vec3* vec_origins, bool update_positions, float sound_time, int speaker_ent, void* sound_params ) {
	typedef __attribute__( ( thiscall ) ) int( *hooks_emit_sound_fn )( void* this, void* filter, int ent_idx, int chan, const char* sound_entry, unsigned int sound_entry_hash, const char* sample, float volume, float attenuation, int seed, int flags, int pitch, const vec3* origin, const vec3* dir, vec3* vec_origins, bool update_positions, float sound_time, int speaker_ent, void* sound_params );

	return ( ( hooks_emit_sound_fn )subhook_get_trampoline( hooks_subhooks[ subhook_emit_sound ] ) )( this, filter, ent_idx, chan, sound_entry, sound_entry_hash, sample, volume, attenuation, seed, flags, pitch, origin, dir, vec_origins, update_positions, sound_time, speaker_ent, sound_params );
}
