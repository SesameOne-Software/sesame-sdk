#include "game/tickbase.h"

// vec_player_cmd_info tickbase_player_cmd_info[ MAX_PLAYERS + 1 ];
// vec_player_sim_info tickbase_player_sim_info[ MAX_PLAYERS + 1 ];

int tickbase_get_dropped_cmds( void ) {

}

int tickbase_determine_sim_ticks( void ) {

}

/* NOTE: cs_iglobals->tickcount AND cs_iglobals->sim_ticks_this_frame HAVE TO BE SET CORRECTLY */ 
void tickbase_adjust_player_time_base( player* player, int simulation_ticks ) {
    assert( simulation_ticks >= 0 );

    if ( simulation_ticks < 0 )
        return;

    static cvar* cl_predict = MAKE_STATIC( cl_predict, icvar_find( cs_icvar, "cl_predict" ) ); /* default to 1 */
    static cvar* sv_clockcorrection_msecs = MAKE_STATIC( sv_clockcorrection_msecs, icvar_find( cs_icvar, "sv_clockcorrection_msecs" ) );  /* default to 30 */
    static cvar* sv_playerperfhistorycount = MAKE_STATIC( sv_playerperfhistorycount, icvar_find( cs_icvar, "sv_playerperfhistorycount" ) ); /* default to 20 */

    /* playerperfhisotry */
    player_sim_info* pi = NULL;
    
    /*
    int player_perf_history = cvar_get_int( sv_playerperfhistorycount );

    if ( player_perf_history > 0 ) {
        while ( vector_size( tickbase_player_sim_info > player_perf_history ) )
            vector_remove( tickbase_player_sim_info, 0 );

        player_sim_info new_player_sim_info;
        player_sim_info_init( &new_player_sim_info );

        vector_add( &tickbase_player_sim_info, new_player_sim_info );

        pi = &tickbase_player_sim_info[ vector_size( tickbase_adjust_player_time_base ) - 1 ]; 
    }
    */

    /* adjust tickbase below */

    /* set final simulation time */
    if ( !cvar_get_bool( cl_predict ) || cs_iglobals->max_clients == 1 ) {
        *player_tick_base( player ) = cs_iglobals->tickcount - simulation_ticks + cs_iglobals->sim_ticks_this_frame;
    }
    /* multiplayer */
    else {
        /* clock correction time */
        float correction_seconds = clamp( cvar_get_float( sv_clockcorrection_msecs ) / 1000.0f, 0.0f, 1.0f );
        int correction_ticks = cs_time_to_ticks( correction_seconds );

        /* find ideal tick */
        int ideal_final_tick = cs_iglobals->tickcount + correction_ticks;
        int estimated_final_tick = *player_tick_base( player ) + simulation_ticks;

        /* correct if client goes ahead of this */
        int too_fast_limit = ideal_final_tick + correction_ticks;

        /* correct if client falls behind this */
        int too_slow_limit = ideal_final_tick - correction_ticks;

        /* see if we are too fast */
        if ( estimated_final_tick > too_fast_limit || estimated_final_tick < too_slow_limit ) {
            int corrected_tick = ideal_final_tick - simulation_ticks + cs_iglobals->sim_ticks_this_frame;

            if ( pi )
                pi->ticks_corrected = correction_ticks;

            *player_tick_base( player ) = corrected_tick;
        }
    }

    if ( pi )
        pi->final_simtime = cs_ticks_to_time( *player_tick_base( player ) + simulation_ticks + cs_iglobals->sim_ticks_this_frame );
}
